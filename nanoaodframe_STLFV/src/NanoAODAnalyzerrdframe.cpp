/*
 * NanoAODAnalyzerrdframe.cpp
 *
 *  Created on: Sep 30, 2018
 *      Author: suyong
 */

#include "NanoAODAnalyzerrdframe.h"
#include <iostream>
#include <algorithm>
#include <typeinfo>

#include "TCanvas.h"
#include "Math/GenVector/VectorUtil.h"
#include <vector>
#include <fstream>
#include "utility.h"
#include <regex>
#include "ROOT/RDFHelpers.hxx"


using namespace std;

NanoAODAnalyzerrdframe::NanoAODAnalyzerrdframe(TTree *atree, std::string outfilename, std::string year, std::string syst, std::string jsonfname, std::string globaltag, int nthreads)
:_rd(*atree),_jsonOK(false), _outfilename(outfilename), _year(year), _syst(syst), _jsonfname(jsonfname), _globaltag(globaltag), _inrootfile(0),_outrootfile(0), _rlm(_rd)
	, _btagcalibreader(BTagEntry::OP_RESHAPING, "central", {"up_jes", "down_jes"})
	, _rnt(&_rlm), currentnode(0), _jetCorrector(0), _jetCorrectionUncertainty(0)
{
        // Skim switch
        if(_year.find("skim") != std::string::npos){
                _isSkim = true;
                cout << "<< Start Skim NanoAOD >>" << endl;
        }else{
                _isSkim = false;
                cout << "<< Start Process NanoAOD >>" << endl;
        }

        // Year switch
        if(_year.find("16pre") != std::string::npos){
                _isRun16pre = true;
                cout << "Run 16 pre (16 APV)" << endl;
        }else if(_year.find("16post") != std::string::npos){
                _isRun16post = true;
                cout << "Run 16 post" << endl;
        }else if(_year.find("17") != std::string::npos){
                _isRun17 = true;
                cout << "Run 17" << endl;
        }else if(_year.find("18") != std::string::npos){
                _isRun18 = true;
                cout << "Run 18" << endl;
        }
        _isRun16 = _isRun16pre || _isRun16post;

	// Data/mc switch
	if(atree->GetBranch("genWeight") == nullptr){
		_isData = true;
		cout << "input file is data" <<endl;
	}else{
		_isData = false;
		cout << "input file is MC" <<endl;
	}
	TObjArray *allbranches = atree->GetListOfBranches();
	for (int i =0; i<allbranches->GetSize(); i++)
	{
		TBranch *abranch = dynamic_cast<TBranch *>(allbranches->At(i));
		if (abranch!= nullptr){
			cout << abranch->GetName() << ", ";
			_originalvars.push_back(abranch->GetName());
		}
	}
        cout<<endl;
        
        if(_isSkim){
            // pu weight setup
            cout<<"Loading Pileup profiles"<<endl;
            if(_isRun16pre){
                pumcfile = "data/Pileup/PileupMC_UL16.root";
                pudatafile = "data/Pileup/PileupDATA_UL16pre.root";
            }else if(_isRun16post){
                pumcfile = "data/Pileup/PileupMC_UL16.root";
                pudatafile = "data/Pileup/PileupDATA_UL16post.root";
            }else if(_isRun17){
                pumcfile = "data/Pileup/PileupMC_UL17.root";
                pudatafile = "data/Pileup/PileupDATA_UL17.root";
            }
            else if(_isRun18){
                pumcfile = "data/Pileup/PileupMC_UL18.root";
                pudatafile = "data/Pileup/PileupDATA_UL18.root";
            }
            TFile tfmc(pumcfile);
            _hpumc = dynamic_cast<TH1D *>(tfmc.Get("pu_mc"));
            _hpumc->SetDirectory(0);
            tfmc.Close();

            TFile tfdata(pudatafile);
            _hpudata = dynamic_cast<TH1D *>(tfdata.Get("pileup"));
            _hpudata_plus = dynamic_cast<TH1D *>(tfdata.Get("pileup_plus"));
            _hpudata_minus = dynamic_cast<TH1D *>(tfdata.Get("pileup_minus"));

            _hpudata->SetDirectory(0);
            _hpudata_plus->SetDirectory(0);
            _hpudata_minus->SetDirectory(0);
            tfdata.Close();

            _puweightcalc = new WeightCalculatorFromHistogram(_hpumc, _hpudata);
            _puweightcalc_plus = new WeightCalculatorFromHistogram(_hpumc, _hpudata_plus);
            _puweightcalc_minus = new WeightCalculatorFromHistogram(_hpumc, _hpudata_minus);
        }else{
            cout<<"Loading jetmet Correction"<<endl;
            setupJetMETCorrection(_globaltag);

            if(!_isData){
                cout<<"Loading Btag SF"<<endl;
                if(_isRun16){
                        _btagcalib = {"DeepJet","data/btagSF/DeepJet_2016LegacySF_V1_TuneCP5.csv"};
                }else if(_isRun17){
                        _btagcalib = {"DeepJet","data/btagSF/DeepJet_106XUL17SF_V2p1.csv"};
                }else if(_isRun18){
                        _btagcalib = {"DeepJet","data/btagSF/DeepJet_106XUL18SF.csv"};
                }

                // load the formulae b flavor tagging
                _btagcalibreader.load(_btagcalib, BTagEntry::FLAV_B, "iterativefit");
                _btagcalibreader.load(_btagcalib, BTagEntry::FLAV_C, "iterativefit");
                _btagcalibreader.load(_btagcalib, BTagEntry::FLAV_UDSG, "iterativefit");

                // Loading Muon Scale Factor
                cout<<"Loading Muon SF"<<endl;
                if(_isRun16pre){
                    TFile muontrg("data/MuonSF/UL2016_preVFP/Efficiencies_muon_generalTracks_Z_Run2016_UL_HIPM_SingleMuonTriggers.root");
                    _hmuontrg = dynamic_cast<TH2F *>(muontrg.Get("NUM_IsoMu24_or_IsoTkMu24_DEN_CutBasedIdTight_and_PFIsoTight_abseta_pt"));
                    _hmuontrg->SetDirectory(0);
                    muontrg.Close();

                    TFile muonid("data/MuonSF/UL2016_preVFP/Efficiencies_muon_generalTracks_Z_Run2016_UL_HIPM_ID.root");
                    _hmuonid = dynamic_cast<TH2F *>(muonid.Get("NUM_TightID_DEN_TrackerMuons_abseta_pt"));
                    _hmuonid->SetDirectory(0);
                    muonid.Close();

                    TFile muoniso("data/MuonSF/UL2016_preVFP/Efficiencies_muon_generalTracks_Z_Run2016_UL_HIPM_ISO.root");
                    _hmuoniso = dynamic_cast<TH2F *>(muoniso.Get("NUM_TightRelIso_DEN_TightIDandIPCut_abseta_pt"));
                    _hmuoniso->SetDirectory(0);
                    muoniso.Close();

                    _muontrg = new WeightCalculatorFromHistogram(_hmuontrg);
                    _muonid = new WeightCalculatorFromHistogram(_hmuonid);
                    _muoniso = new WeightCalculatorFromHistogram(_hmuoniso);

                }else if(_isRun16post){
                    TFile muontrg("data/MuonSF/UL2016_postVFP/Efficiencies_muon_generalTracks_Z_Run2016_UL_SingleMuonTriggers.root");
                    _hmuontrg = dynamic_cast<TH2F *>(muontrg.Get("NUM_IsoMu24_or_IsoTkMu24_DEN_CutBasedIdTight_and_PFIsoTight_abseta_pt"));
                    _hmuontrg->SetDirectory(0);
                    muontrg.Close();

                    TFile muonid("data/MuonSF/UL2016_postVFP/Efficiencies_muon_generalTracks_Z_Run2016_UL_ID.root");
                    _hmuonid = dynamic_cast<TH2F *>(muonid.Get("NUM_TightID_DEN_TrackerMuons_abseta_pt"));
                    _hmuonid->SetDirectory(0);
                    muonid.Close();

                    TFile muoniso("data/MuonSF/UL2016_postVFP/Efficiencies_muon_generalTracks_Z_Run2016_UL_ISO.root");
                    _hmuoniso = dynamic_cast<TH2F *>(muoniso.Get("NUM_TightRelIso_DEN_TightIDandIPCut_abseta_pt"));
                    _hmuoniso->SetDirectory(0);
                    muoniso.Close();

                    _muontrg = new WeightCalculatorFromHistogram(_hmuontrg);
                    _muonid = new WeightCalculatorFromHistogram(_hmuonid);
                    _muoniso = new WeightCalculatorFromHistogram(_hmuoniso);

                }else if(_isRun17){
                    TFile muontrg("data/MuonSF/UL2017/Efficiencies_muon_generalTracks_Z_Run2017_UL_SingleMuonTriggers.root");
                    _hmuontrg = dynamic_cast<TH2F *>(muontrg.Get("NUM_IsoMu27_DEN_CutBasedIdTight_and_PFIsoTight_abseta_pt"));
                    _hmuontrg->SetDirectory(0);
                    muontrg.Close();

                    TFile muonid("data/MuonSF/UL2017/Efficiencies_muon_generalTracks_Z_Run2017_UL_ID.root");
                    _hmuonid = dynamic_cast<TH2F *>(muonid.Get("NUM_TightID_DEN_TrackerMuons_abseta_pt"));
                    _hmuonid->SetDirectory(0);
                    muonid.Close();

                    TFile muoniso("data/MuonSF/UL2017/Efficiencies_muon_generalTracks_Z_Run2017_UL_ISO.root");
                    _hmuoniso = dynamic_cast<TH2F *>(muoniso.Get("NUM_TightRelIso_DEN_TightIDandIPCut_abseta_pt"));
                    _hmuoniso->SetDirectory(0);
                    muoniso.Close();

                    _muontrg = new WeightCalculatorFromHistogram(_hmuontrg);
                    _muonid = new WeightCalculatorFromHistogram(_hmuonid);
                    _muoniso = new WeightCalculatorFromHistogram(_hmuoniso);

                }else if(_isRun18){
                    TFile muontrg("data/MuonSF/UL2018/Efficiencies_muon_generalTracks_Z_Run2018_UL_SingleMuonTriggers.root");
                    _hmuontrg = dynamic_cast<TH2F *>(muontrg.Get("NUM_IsoMu24_DEN_CutBasedIdTight_and_PFIsoTight_abseta_pt"));
                    _hmuontrg->SetDirectory(0);
                    muontrg.Close();

                    TFile muonid("data/MuonSF/UL2018/Efficiencies_muon_generalTracks_Z_Run2018_UL_ID.root");
                    _hmuonid = dynamic_cast<TH2F *>(muonid.Get("NUM_TightID_DEN_TrackerMuons_abseta_pt"));
                    _hmuonid->SetDirectory(0);
                    muonid.Close();

                    TFile muoniso("data/MuonSF/UL2018/Efficiencies_muon_generalTracks_Z_Run2018_UL_ISO.root");
                    _hmuoniso = dynamic_cast<TH2F *>(muoniso.Get("NUM_TightRelIso_DEN_TightIDandIPCut_abseta_pt"));
                    _hmuoniso->SetDirectory(0);
                    muoniso.Close();

                    _muontrg = new WeightCalculatorFromHistogram(_hmuontrg);
                    _muonid = new WeightCalculatorFromHistogram(_hmuonid);
                    _muoniso = new WeightCalculatorFromHistogram(_hmuoniso);
                }
                // Loading Tau Scale Factor
                cout<<"Loading Tau SF"<<endl;
                if(_isRun16pre){
                        _tauidSFjet = new TauIDSFTool("UL2016_preVFP","DeepTau2017v2p1VSjet","Medium");
                        _tauidSFele = new TauIDSFTool("UL2016_preVFP","DeepTau2017v2p1VSe","VLoose");
                        //_tauidSFmu = new TauIDSFTool("UL2016_preVFP","DeepTau2017v2p1VSmu","Tight");
                        _testool = new TauESTool("UL2016_preVFP","DeepTau2017v2p1VSjet");
                        _festool = new TauFESTool("UL2016_preVFP");
                }else if(_isRun16post){
                        _tauidSFjet = new TauIDSFTool("UL2016_postVFP","DeepTau2017v2p1VSjet","Medium");
                        _tauidSFele = new TauIDSFTool("UL2016_postVFP","DeepTau2017v2p1VSe","VLoose");
                        //_tauidSFmu = new TauIDSFTool("UL2016_postVFP","DeepTau2017v2p1VSmu","Tight");
                        _testool = new TauESTool("UL2016_postVFP","DeepTau2017v2p1VSjet");
                        _festool = new TauFESTool("UL2016_postVFP");
                }else if(_isRun17){
                        _tauidSFjet = new TauIDSFTool("UL2017","DeepTau2017v2p1VSjet","Medium");
                        _tauidSFele = new TauIDSFTool("UL2017","DeepTau2017v2p1VSe","VLoose");
                        //_tauidSFmu = new TauIDSFTool("UL2017","DeepTau2017v2p1VSmu","Tight");
                        _testool = new TauESTool("UL2017","DeepTau2017v2p1VSjet");
                        _festool = new TauFESTool("UL2017");
                }else if(_isRun18){
                        _tauidSFjet = new TauIDSFTool("UL2018","DeepTau2017v2p1VSjet","Medium");
                        _tauidSFele = new TauIDSFTool("UL2018","DeepTau2017v2p1VSe","VLoose");
                        //_tauidSFmu = new TauIDSFTool("UL2018","DeepTau2017v2p1VSmu","Tight");
                        _testool = new TauESTool("UL2018","DeepTau2017v2p1VSjet");
                        _festool = new TauFESTool("UL2018");
                }
            }
        }
}

NanoAODAnalyzerrdframe::~NanoAODAnalyzerrdframe() {
	// TODO Auto-generated destructor stub
	// ugly...

	cout << "writing histograms" << endl;
        for (auto afile:_outrootfilenames)
        {
                _outrootfile = new TFile(afile.c_str(), "UPDATE");
                for (auto &h : _th1dhistos)
                {
                        if (h.second.GetPtr() != nullptr) h.second->Write();
                }
                _outrootfile->Write(0, TObject::kOverwrite);
                _outrootfile->Close();
                delete _outrootfile;
        }
}

bool NanoAODAnalyzerrdframe::isDefined(string v)
{
	auto result = std::find(_originalvars.begin(), _originalvars.end(), v);
	if (result != _originalvars.end()) return true;
	else return false;
}

void NanoAODAnalyzerrdframe::setTree(TTree *t, std::string outfilename)
{
	_rd = ROOT::RDataFrame(*t);
	_rlm = RNode(_rd);
	_outfilename = outfilename;
	_hist1dinfovector.clear();
	_th1dhistos.clear();
	_varstostore.clear();
	_hist1dinfovector.clear();
	_selections.clear();

	this->setupAnalysis();
}

void NanoAODAnalyzerrdframe::setupAnalysis()
{
	/* Must sequentially define objects.
	 *
	 */

	if (_isData) _jsonOK = readjson();
	// Event weight for data it's always one. For MC, it depends on the sign

	_rlm = _rlm.Define("one", "1.0");
        if(_isSkim){
                if(_isData){
                        _rlm = _rlm.Define("unitGenWeight","one")
                                   .Define("pugenWeight","one");
                }else{
                        _rlm = _rlm.Define("unitGenWeight","genWeight != 0 ? genWeight/abs(genWeight) : 0")
                                   .Define("puWeight",[this](float x) {return _puweightcalc->getWeight(x);}, {"Pileup_nTrueInt"})
                                   .Define("puWeight_plus",[this](float x) {return _puweightcalc_plus->getWeight(x);}, {"Pileup_nTrueInt"})
                                   .Define("puWeight_minus",[this](float x) {return _puweightcalc_minus->getWeight(x);}, {"Pileup_nTrueInt"})
                                   .Define("pugenWeight", "unitGenWeight * puWeight");
                }
        }else if(_isData){
                _rlm = _rlm.Define("evWeight","one")
                           .Define("evWeight_tauSF","one")
                           .Define("evWeight_muonSF","one")
                           .Define("evWeight_leptonSF","one")
                           .Define("btagWeight_DeepFlavBrecalc","one");
        }


	// Object selection will be defined in sequence.
	// Selected objects will be stored in new vectors.
	selectElectrons();
	selectMuons();
        applyJetMETCorrections();
        //selectMET();
        selectJets();
        selectTaus();
        removeOverlaps();
        //selectFatJets();
        if(!_isData && !_isSkim){
            matchGenReco();
            calculateEvWeight();
        }
	defineMoreVars();
	defineCuts();
	bookHists();
	setupCuts_and_Hists();
	setupTree();
}

bool NanoAODAnalyzerrdframe::readjson()
{
	auto isgoodjsonevent = [this](unsigned int runnumber, unsigned int lumisection)
		{
			auto key = std::to_string(runnumber);

			bool goodeventflag = false;

			if (jsonroot.isMember(key))
			{
				Json::Value runlumiblocks = jsonroot[key];
				for (unsigned int i=0; i<runlumiblocks.size() && !goodeventflag; i++)
				{
					auto lumirange = runlumiblocks[i];
					if (lumisection >= lumirange[0].asUInt() && lumisection <= lumirange[1].asUInt()) goodeventflag = true;
				}
				return goodeventflag;
			}
			else
			{
				//cout << "Run not in json " << runnumber << endl;
				return false;
			}

		};

	if (_jsonfname != "")
	{
		std::ifstream jsoninfile;
		jsoninfile.open(_jsonfname);

		if (jsoninfile.good())
		{
			jsoninfile >> jsonroot;
			/*
			auto runlumiblocks =  jsonroot["276775"];
			for (auto i=0; i<runlumiblocks.size(); i++)
			{
				auto lumirange = runlumiblocks[i];
				cout << "lumi range " << lumirange[0] << " " << lumirange[1] << endl;
			}
			*/
			_rlm = _rlm.Define("goodjsonevent", isgoodjsonevent, {"run", "luminosityBlock"}).Filter("goodjsonevent");
			_jsonOK = true;
			return true;
		}
		else
		{
			cout << "Problem reading json file " << _jsonfname << endl;
			return false;
		}
	}
	else
	{
		cout << "no JSON file given" << endl;
		return true;
	}
}

void NanoAODAnalyzerrdframe::setupJetMETCorrection(string globaltag, string jetalgo)
{
	if (globaltag != "")
	{
		cout << "Applying new JetMET corrections. GT: "+globaltag+" on jetAlgo: "+jetalgo << endl;
		string basedirectory = "data/jme/";

		string datamcflag = "";
		if (_isData) datamcflag = "DATA";
		else datamcflag = "MC";

		// set file names that contain the parameters for corrections
		string dbfilenamel1 = basedirectory+globaltag+"_"+datamcflag+"_L1FastJet_"+jetalgo+".txt";
		string dbfilenamel2 = basedirectory+globaltag+"_"+datamcflag+"_L2Relative_"+jetalgo+".txt";
		string dbfilenamel3 = basedirectory+globaltag+"_"+datamcflag+"_L3Absolute_"+jetalgo+".txt";
		string dbfilenamel2l3 = basedirectory+globaltag+"_"+datamcflag+"_L2L3Residual_"+jetalgo+".txt";

		JetCorrectorParameters *L1JetCorrPar = new JetCorrectorParameters(dbfilenamel1);

                if (!L1JetCorrPar->isValid())
                {
                    std::cerr << "L1FastJet correction parameters not read" << std::endl;
                    exit(1);
                }
                        JetCorrectorParameters *L2JetCorrPar = new JetCorrectorParameters(dbfilenamel2);
                if (!L2JetCorrPar->isValid())
                {
                    std::cerr << "L2Relative correction parameters not read" << std::endl;
                    exit(1);
                }
                        JetCorrectorParameters *L3JetCorrPar = new JetCorrectorParameters(dbfilenamel3);
                if (!L3JetCorrPar->isValid())
                {
                    std::cerr << "L3Absolute correction parameters not read" << std::endl;
                    exit(1);
                }
                        JetCorrectorParameters *L2L3JetCorrPar = new JetCorrectorParameters(dbfilenamel2l3);
                if (!L2L3JetCorrPar->isValid())
                {
                    std::cerr << "L2L3Residual correction parameters not read" << std::endl;
                    exit(1);
                }

		// to apply all the corrections, first collect them into a vector
		std::vector<JetCorrectorParameters> jetc;
		jetc.push_back(*L1JetCorrPar);
		jetc.push_back(*L2JetCorrPar);
		jetc.push_back(*L3JetCorrPar);
		jetc.push_back(*L2L3JetCorrPar);

		// apply the various corrections
		_jetCorrector = new FactorizedJetCorrector(jetc);

		// object to calculate uncertainty
		string dbfilenameunc = basedirectory+globaltag+"_"+datamcflag+"_Uncertainty_"+jetalgo+".txt";
		_jetCorrectionUncertainty = new JetCorrectionUncertainty(dbfilenameunc);
	}
	else
	{
		cout << "Not applying new JetMET corrections. Using NanoAOD as is." << endl;
	}
}

void NanoAODAnalyzerrdframe::selectElectrons()
{
	//cout << "select electrons" << endl;
        // Run II recommendation: https://twiki.cern.ch/twiki/bin/viewauth/CMS/EgammaRunIIRecommendations
        // Run II recomendation - cutbased: https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedElectronIdentificationRun2
        // Temporary elecuts (Not to be used!)
	_rlm = _rlm.Define("elecuts", "Electron_pt>15.0 && abs(Electron_eta)<2.4 && Electron_cutBased == 1 && ((abs(Electron_deltaEtaSC<=1.479) && abs(Electron_dxy) < 0.05 && abs(Electron_dz) < 0.10 ) || (abs(Electron_deltaEtaSC>1.479) && abs(Electron_dxy) < 0.10 && abs(Electron_dz) < 0.20))");
	_rlm = _rlm.Define("vetoelecuts", "Electron_pt>15.0 && abs(Electron_eta)<2.4 && Electron_cutBased == 1");
        _rlm = _rlm.Define("Sel_elept", "Electron_pt[elecuts]") // define new variables
			.Define("Sel_eleta", "Electron_eta[elecuts]")
			.Define("Sel_elephi", "Electron_phi[elecuts]")
			.Define("Sel_elemass", "Electron_mass[elecuts]")
                        .Define("Sel_eleidx", ::good_idx, {"elecuts"})
			.Define("nelepass", "int(Sel_elept.size())")
                        .Define("nvetoelepass","Sum(vetoelecuts)");

	_rlm = _rlm.Define("ele4vecs", ::gen4vec, {"Sel_elept", "Sel_eleta", "Sel_elephi", "Sel_elemass"});
}

void NanoAODAnalyzerrdframe::selectMuons()
{
	//cout << "select muons" << endl;
	_rlm = _rlm.Define("muoncuts", "Muon_pt>50.0 && abs(Muon_eta)<2.4 && Muon_tightId && Muon_pfRelIso04_all<0.15");
	_rlm = _rlm.Define("Sel_muonpt", "Muon_pt[muoncuts]") // define new variables
			.Define("Sel_muoneta", "Muon_eta[muoncuts]")
			.Define("Sel_muonphi", "Muon_phi[muoncuts]")
			.Define("Sel_muonmass", "Muon_mass[muoncuts]")
                        .Define("Sel_muoncharge", "Muon_charge[muoncuts]")
                        .Define("Sel_muonidx", ::good_idx, {"muoncuts"})
			.Define("nmuonpass", "int(Sel_muonpt.size())");
	
        _rlm = _rlm.Define("muoncuts_woIso", "Muon_pt>30.0 && abs(Muon_eta)<2.4 && Muon_tightId")
                        .Define("nmuonpass_woIso","Sum(muoncuts_woIso)");

        _rlm = _rlm.Define("vetomuoncuts", "!muoncuts && Muon_pt>15.0 && abs(Muon_eta)<2.4 && Muon_looseId && Muon_pfRelIso04_all<0.25")
                   .Define("vetomuoncuts_woIso", "!muoncuts_woIso && Muon_pt>15.0 && abs(Muon_eta)<2.4 && Muon_looseId && Muon_pfRelIso04_all<0.25")
                   .Define("nvetomuons","Sum(vetomuoncuts)")
                   .Define("nvetomuons_woIso","Sum(vetomuoncuts_woIso)");

        _rlm = _rlm.Define("muon4vecs", ::gen4vec, {"Sel_muonpt", "Sel_muoneta", "Sel_muonphi", "Sel_muonmass"});
}

/*
void NanoAODAnalyzerrdframe::selectMET()
{
        _rlm = _rlm.Define("met4vec", ::genmet4vec, {"MET_pt","MET_phi"});

}*/


// Adapted from https://github.com/cms-nanoAOD/nanoAOD-tools/blob/master/python/postprocessing/modules/jme/jetRecalib.py
// and https://github.com/cms-nanoAOD/nanoAOD-tools/blob/master/python/postprocessing/modules/jme/JetRecalibrator.py
void NanoAODAnalyzerrdframe::applyJetMETCorrections()
{
	auto appcorrlambdaf = [this](floats jetpts, floats jetetas, floats jetAreas, floats jetrawf, float rho)->floats
	{
		floats corrfactors;
		corrfactors.reserve(jetpts.size());
		for (unsigned int i =0; i<jetpts.size(); i++)
		{
			float rawjetpt = jetpts[i]*(1.0-jetrawf[i]);
			_jetCorrector->setJetPt(rawjetpt);
			_jetCorrector->setJetEta(jetetas[i]);
			_jetCorrector->setJetA(jetAreas[i]);
			_jetCorrector->setRho(rho);
			float corrfactor = _jetCorrector->getCorrection();
			corrfactors.emplace_back(rawjetpt * corrfactor);
		}
		return corrfactors;
	};

	auto jecuncertaintylambdaf= [this](floats jetpts, floats jetetas, floats jetAreas, floats jetrawf, float rho)->floats
		{
			floats uncertainties;
			uncertainties.reserve(jetpts.size());
			for (unsigned int i =0; i<jetpts.size(); i++)
			{
				float rawjetpt = jetpts[i]*(1.0-jetrawf[i]);
				_jetCorrector->setJetPt(rawjetpt);
				_jetCorrector->setJetEta(jetetas[i]);
				_jetCorrector->setJetA(jetAreas[i]);
				_jetCorrector->setRho(rho);
				float corrfactor = _jetCorrector->getCorrection();
				_jetCorrectionUncertainty->setJetPt(corrfactor*rawjetpt);
				_jetCorrectionUncertainty->setJetEta(jetetas[i]);
				float unc = _jetCorrectionUncertainty->getUncertainty(true);
				uncertainties.emplace_back(unc);

			}
			return uncertainties;
		};

	auto metcorrlambdaf = [](float met, float metphi, floats jetptsbefore, floats jetptsafter, floats jetphis)->float
	{
		auto metx = met * cos(metphi);
		auto mety = met * sin(metphi);
		for (unsigned int i=0; i<jetphis.size(); i++)
		{
			if (jetptsafter[i]>15.0)
			{
				metx -= (jetptsafter[i] - jetptsbefore[i])*cos(jetphis[i]);
				mety -= (jetptsafter[i] - jetptsbefore[i])*sin(jetphis[i]);
			}
		}
		return float(sqrt(metx*metx + mety*mety));
	};

	auto metphicorrlambdaf = [](float met, float metphi, floats jetptsbefore, floats jetptsafter, floats jetphis)->float
	{
		auto metx = met * cos(metphi);
		auto mety = met * sin(metphi);
		for (unsigned int i=0; i<jetphis.size(); i++)
		{
			if (jetptsafter[i]>15.0)
			{
				metx -= (jetptsafter[i] - jetptsbefore[i])*cos(jetphis[i]);
				mety -= (jetptsafter[i] - jetptsbefore[i])*sin(jetphis[i]);
			}
		}
		return float(atan2(mety, metx));
	};

	if (_jetCorrector != 0)
	{
		_rlm = _rlm.Define("Jet_pt_corr", appcorrlambdaf, {"Jet_pt", "Jet_eta", "Jet_area", "Jet_rawFactor", "fixedGridRhoFastjetAll"});
		_rlm = _rlm.Define("Jet_pt_relerror", jecuncertaintylambdaf, {"Jet_pt", "Jet_eta", "Jet_area", "Jet_rawFactor", "fixedGridRhoFastjetAll"});
		_rlm = _rlm.Define("Jet_pt_corr_up", "Jet_pt_corr*(1.0f + Jet_pt_relerror)");
		_rlm = _rlm.Define("Jet_pt_corr_down", "Jet_pt_corr*(1.0f - Jet_pt_relerror)");
		_rlm = _rlm.Define("MET_pt_corr", metcorrlambdaf, {"MET_pt", "MET_phi", "Jet_pt", "Jet_pt_corr", "Jet_phi"});
		_rlm = _rlm.Define("MET_phi_corr", metphicorrlambdaf, {"MET_pt", "MET_phi", "Jet_pt", "Jet_pt_corr", "Jet_phi"});
		_rlm = _rlm.Define("MET_pt_corr_up", metcorrlambdaf, {"MET_pt", "MET_phi", "Jet_pt", "Jet_pt_corr_up", "Jet_phi"});
		_rlm = _rlm.Define("MET_phi_corr_up", metphicorrlambdaf, {"MET_pt", "MET_phi", "Jet_pt", "Jet_pt_corr_up", "Jet_phi"});
		_rlm = _rlm.Define("MET_pt_corr_down", metcorrlambdaf, {"MET_pt", "MET_phi", "Jet_pt", "Jet_pt_corr_down", "Jet_phi"});
		_rlm = _rlm.Define("MET_phi_corr_down", metphicorrlambdaf, {"MET_pt", "MET_phi", "Jet_pt", "Jet_pt_corr_down", "Jet_phi"});
	}
}

void NanoAODAnalyzerrdframe::selectJets()
{
	// apparently size() returns long int, which ROOT doesn't recognized for branch types
	// , so it must be cast into int if you want to save them later into a TTree
        if (_globaltag != ""){
                if (_syst=="jecup"){
                        _rlm = _rlm.Define("Sys_jetpt","Jet_pt_corr_up");
                        _rlm = _rlm.Define("Sys_METpt","MET_pt_corr_up");
                        _rlm = _rlm.Define("Sys_METphi","MET_phi_corr_up");
                } else if (_syst=="jecdown"){
                        _rlm = _rlm.Define("Sys_jetpt","Jet_pt_corr_down");
                        _rlm = _rlm.Define("Sys_METpt","MET_pt_corr_down");
                        _rlm = _rlm.Define("Sys_METphi","MET_phi_corr_down");
                } else{
                        _rlm = _rlm.Define("Sys_jetpt","Jet_pt_corr");
                        _rlm = _rlm.Define("Sys_METpt","MET_pt_corr");
                        _rlm = _rlm.Define("Sys_METphi","MET_phi_corr");
                }
        }else{
                _rlm = _rlm.Define("Sys_jetpt","Jet_pt");
                _rlm = _rlm.Define("Sys_METpt","MET_pt");
                _rlm = _rlm.Define("Sys_METphi","MET_phi");
        }
	_rlm = _rlm.Define("jetcuts", "Sys_jetpt>40.0 && abs(Jet_eta)<2.4 && Jet_jetId >= 6")
			.Define("Sel_jetpt", "Sys_jetpt[jetcuts]")
			.Define("Sel_jeteta", "Jet_eta[jetcuts]")
			.Define("Sel_jetphi", "Jet_phi[jetcuts]")
			.Define("Sel_jetmass", "Jet_mass[jetcuts]")
			.Define("Sel_jetbtag", "Jet_btagDeepFlavB[jetcuts]")
			.Define("njetspass", "int(Sel_jetpt.size())")
			.Define("jet4vecs", ::gen4vec, {"Sel_jetpt", "Sel_jeteta", "Sel_jetphi", "Sel_jetmass"});
}

void NanoAODAnalyzerrdframe::selectTaus()
{
        // Tau SF
        cout<<"Applying TauES on Genuine taus"<<endl;
        auto tauES = [this](floats &pt, floats &eta, ints &dm, uchars &genid, floats &x)->floats{
                floats xout;
                for(unsigned int i=0; i<pt.size(); i++){
                        float es = 1.0;
                        if(genid[i]==5){
                                es = _testool->getTES(pt[i],dm[i],genid[i]);
                        }else if(genid[i]==1 || genid[i]==3){
                                es = _festool->getFES(eta[i],dm[i],genid[i]);
                        }
                        xout.emplace_back(x[i]*es);
                }
                return xout;
        };
        if (!_isData) {
                _rlm = _rlm.Define("Scaled_taupt",tauES,{"Tau_pt","Tau_eta","Tau_decayMode","Tau_genPartFlav","Tau_pt"})
                           .Define("Scaled_taumass",tauES,{"Tau_pt","Tau_eta","Tau_decayMode","Tau_genPartFlav","Tau_mass"});
        }else{
                _rlm = _rlm.Define("Scaled_taupt","Tau_pt")
                           .Define("Scaled_taumass","Tau_mass");
        }

        auto overlap_removal_mutau = [](FourVectorVec &muon4vecs, FourVectorVec &tau4vecs){
                ints out;
                for (auto tau: tau4vecs){
                        int check = 0;
                        for (auto mu: muon4vecs){
                                auto dR = ROOT::Math::VectorUtil::DeltaR(mu, tau);
                                if( dR >= 0.4 ) check = 1;
                        }
                        out.emplace_back(check);
                }
                return out;
        };
        _rlm = _rlm.Define("tau4vecs", ::gen4vec, {"Scaled_taupt", "Tau_eta", "Tau_phi", "Scaled_taumass"})
                   .Define("mutauoverlap", overlap_removal_mutau, {"muon4vecs","tau4vecs"});

        // Hadronic Tau Object Selections
        _rlm = _rlm.Define("taucuts", "Scaled_taupt>50.0 && abs(Tau_eta)<2.3 && Tau_idDecayModeNewDMs")
                   .Define("deeptauidcuts","Tau_idDeepTau2017v2p1VSmu & 8 && Tau_idDeepTau2017v2p1VSe & 4 && Tau_idDeepTau2017v2p1VSjet & 16")
                   .Define("seltaucuts","taucuts && deeptauidcuts && mutauoverlap");

        // Hadronic Tau Selection
        _rlm = _rlm.Define("Sel_taupt", "Scaled_taupt[seltaucuts]")
                   .Define("Sel_taueta", "Tau_eta[seltaucuts]")
                   .Define("Sel_tauphi", "Tau_phi[seltaucuts]")
                   .Define("Sel_taumass", "Scaled_taumass[seltaucuts]")
                   .Define("Sel_taucharge", "Tau_charge[seltaucuts]")
                   .Define("Sel_taujetidx", "Tau_jetIdx[seltaucuts]")
                   .Define("ncleantaupass", "int(Sel_taupt.size())")
                   .Define("cleantau4vecs", ::gen4vec, {"Sel_taupt", "Sel_taueta", "Sel_tauphi", "Sel_taumass"});

}

void NanoAODAnalyzerrdframe::removeOverlaps()
{
	// for checking overlapped jets with leptons
        auto checkoverlap = [](FourVectorVec &seljets, FourVectorVec &sellep)
        {
                doubles mindrlepton;
                for (auto ajet: seljets)
                {
                        auto mindr = 6.0;
                        for ( auto alepton : sellep )
                        {
                                auto dr = ROOT::Math::VectorUtil::DeltaR(ajet, alepton);
                                if (dr < mindr) mindr = dr;
                        }
                        int out = mindr > 0.4 ? 1 : 0;
                        mindrlepton.emplace_back(out);
                }
                return mindrlepton;
        };

	// Overlap removal with muon (used for btagging SF)
        _rlm = _rlm.Define("muonjetoverlap", checkoverlap, {"jet4vecs","muon4vecs"});
	_rlm =	_rlm.Define("Selmu_jetpt", "Sel_jetpt[muonjetoverlap]")
                    .Define("Selmu_jeteta", "Sel_jeteta[muonjetoverlap]")
                    .Define("Selmu_jetphi", "Sel_jetphi[muonjetoverlap]")
                    .Define("Selmu_jetmass", "Sel_jetmass[muonjetoverlap]")
                    .Define("Selmu_jetbtag", "Sel_jetbtag[muonjetoverlap]")
                    .Define("ncleanmuonjetspass", "int(Selmu_jetpt.size())")
                    .Define("cleanmuonjet4vecs", ::gen4vec, {"Selmu_jetpt", "Selmu_jeteta", "Selmu_jetphi", "Selmu_jetmass"})
                    .Define("Selmu_jetHT", "Sum(Selmu_jetpt)");


        // Again overlap removal with hadronic tau
        _rlm = _rlm.Define("taujetoverlap", checkoverlap, {"cleanmuonjet4vecs","cleantau4vecs"});
        _rlm = _rlm.Define("Sel2_jetpt", "Selmu_jetpt[taujetoverlap]")
                   .Define("Sel2_jeteta", "Selmu_jeteta[taujetoverlap]")
                   .Define("Sel2_jetphi", "Selmu_jetphi[taujetoverlap]")
                   .Define("Sel2_jetmass", "Selmu_jetmass[taujetoverlap]")
                   .Define("Sel2_jetbtag", "Selmu_jetbtag[taujetoverlap]")
                   .Define("ncleanjetspass", "int(Sel2_jetpt.size())")
                   .Define("cleanjet4vecs", ::gen4vec, {"Sel2_jetpt", "Sel2_jeteta", "Sel2_jetphi", "Sel2_jetmass"})
                   .Define("Sel2_jetHT", "Sum(Sel2_jetpt)");

        if(_isRun16){
                //https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation2016Legacy
                _rlm = _rlm.Define("btagcuts", "Sel2_jetbtag>0.7221"); //l: 0.0614, m: 0.3093, t: 0.7221
        }else if(_isRun17){
                //https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation106XUL17
                _rlm = _rlm.Define("btagcuts", "Sel2_jetbtag>0.7476"); //l: 0.0532, m: 0.3040, t: 0.7476
        }else if(_isRun18){
                //https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation106XUL18
                _rlm = _rlm.Define("btagcuts", "Sel2_jetbtag>0.7100"); //l: 0.0490, m: 0.2783, t: 0.7100
        }
	
        _rlm = _rlm.Define("Sel2_bjetpt", "Sel2_jetpt[btagcuts]")
                   .Define("Sel2_bjeteta", "Sel2_jeteta[btagcuts]")
                   .Define("Sel2_bjetphi", "Sel2_jetphi[btagcuts]")
                   .Define("Sel2_bjetmass", "Sel2_jetmass[btagcuts]")
                   .Define("Sel2_bjetbtag", "Sel2_jetbtag[btagcuts]")
                   .Define("ncleanbjetspass", "int(Sel2_bjetpt.size())")
                   .Define("Sel2_bjetHT", "Sum(Sel2_bjetpt)")
                   .Define("cleanbjet4vecs", ::gen4vec, {"Sel2_bjetpt", "Sel2_bjeteta", "Sel2_bjetphi", "Sel2_bjetmass"});

}

void NanoAODAnalyzerrdframe::matchGenReco()
{
        _rlm = _rlm.Define("FinalGenPart_idx", ::FinalGenPart_idx, {"GenPart_pdgId", "GenPart_genPartIdxMother"})
                   .Define("GenPart_LFVup_idx", "FinalGenPart_idx[0]")
                   .Define("GenPart_LFVmuon_idx", "FinalGenPart_idx[1]")
                   .Define("GenPart_LFVtau_idx", "FinalGenPart_idx[2]")
                   .Define("GenPart_SMb_idx", "FinalGenPart_idx[3]")
                   .Define("GenPart_SMW1_idx", "FinalGenPart_idx[4]")
                   .Define("GenPart_SMW2_idx", "FinalGenPart_idx[5]")
                   .Define("GenPart_LFVtop_idx", "FinalGenPart_idx[6]")
                   .Define("GenPart_SMtop_idx", "FinalGenPart_idx[7]");

        _rlm = _rlm.Define("drmax1", "float(0.15)")
                   .Define("drmax2", "float(0.4)")
                   .Define("Muon_matched", ::dRmatching_binary,{"GenPart_LFVmuon_idx","drmax1","GenPart_pt","GenPart_eta","GenPart_phi","GenPart_mass","Muon_pt","Muon_eta","Muon_phi","Muon_mass"})
                   .Define("Tau_matched",::dRmatching_binary,{"GenPart_LFVtau_idx","drmax2", "GenPart_pt","GenPart_eta","GenPart_phi","GenPart_mass","Tau_pt","Tau_eta","Tau_phi","Tau_mass"})
                   .Define("Jet_LFVup_matched",::dRmatching_binary,{"GenPart_LFVup_idx","drmax2","GenPart_pt","GenPart_eta","GenPart_phi","GenPart_mass","Jet_pt","Jet_eta","Jet_phi","Jet_mass"})
                   .Define("Jet_SMb_matched",::dRmatching_binary,{"GenPart_SMb_idx","drmax2","GenPart_pt","GenPart_eta","GenPart_phi","GenPart_mass","Jet_pt","Jet_eta","Jet_phi","Jet_mass"})
                   .Define("Jet_SMW1_matched",::dRmatching_binary,{"GenPart_SMW1_idx","drmax2","GenPart_pt","GenPart_eta","GenPart_phi","GenPart_mass","Jet_pt","Jet_eta","Jet_phi","Jet_mass"})
                   .Define("Jet_SMW2_matched",::dRmatching_binary,{"GenPart_SMW2_idx","drmax2", "GenPart_pt","GenPart_eta","GenPart_phi","GenPart_mass","Jet_pt","Jet_eta","Jet_phi","Jet_mass"})
                   .Define("Sel_muon_matched","Muon_matched[muoncuts]")
                   .Define("Sel_tau_matched","Tau_matched[seltaucuts]")
                   .Define("Sel2_LFVupjet_matched","Jet_LFVup_matched[jetcuts][muonjetoverlap][taujetoverlap]")
                   .Define("Sel2_SMbjet_matched","Jet_SMb_matched[jetcuts][muonjetoverlap][taujetoverlap]")
                   .Define("Sel2_SMW1jet_matched","Jet_SMW1_matched[jetcuts][muonjetoverlap][taujetoverlap]")
                   .Define("Sel2_SMW2jet_matched","Jet_SMW2_matched[jetcuts][muonjetoverlap][taujetoverlap]")
                   .Define("Sel_muonmatched_idx","ArgMax(Sel_muon_matched)")
                   .Define("Sel_taumatched_idx","ArgMax(Sel_tau_matched)")
                   .Define("Sel2_LFVupjet_matched_idx","ArgMax(Sel2_LFVupjet_matched)")
                   .Define("Sel2_SMbjet_matched_idx","ArgMax(Sel2_SMbjet_matched)")
                   .Define("Sel2_SMW1jet_matched_idx","ArgMax(Sel2_SMW1jet_matched)")
                   .Define("Sel2_SMW2jet_matched_idx","ArgMax(Sel2_SMW2jet_matched)")
                   .Define("nmuonmatched", "Sum(Muon_matched)")
                   .Define("ntaumatched", "Sum(Tau_matched)")
                   .Define("nJet_LFVup_matched", "Sum(Jet_LFVup_matched)")
                   .Define("nJet_SMb_matched", "Sum(Jet_SMb_matched)")
                   .Define("nJet_SMW1_matched", "Sum(Jet_SMW1_matched)")
                   .Define("nJet_SMW2_matched", "Sum(Jet_SMW2_matched)");
}

void NanoAODAnalyzerrdframe::selectFatJets()
{
	_rlm = _rlm.Define("fatjetcuts", "FatJet_pt>400.0 && abs(FatJet_eta)<2.4 && FatJet_tau1>0.0 && FatJet_tau2>0.0 && FatJet_tau3>0.0 && FatJet_tau3/FatJet_tau2<0.5")
				.Define("Sel_fatjetpt", "FatJet_pt[fatjetcuts]")
				.Define("Sel_fatjeteta", "FatJet_eta[fatjetcuts]")
				.Define("Sel_fatjetphi", "FatJet_phi[fatjetcuts]")
				.Define("Sel_fatjetmass", "FatJet_mass[fatjetcuts]")
				.Define("nfatjetspass", "int(Sel_fatjetpt.size())")
				//.Define("Sel_fatjetweight", "std::vector<double>(nfatjetspass, evWeight)")
				.Define("Sel_fatjet4vecs", ::gen4vec, {"Sel_fatjetpt", "Sel_fatjeteta", "Sel_fatjetphi", "Sel_fatjetmass"});
}

// This function is newly added for getting event weight with selected objects
void NanoAODAnalyzerrdframe::calculateEvWeight()
{
        
        // Muon SF
        cout<<"Getting Muon Scale Factors"<<endl;
        auto muonSF = [this](floats &pt, floats &eta)->float {
            float weight = 1.0;
            if(pt.size() > 0){
                for(unsigned int i=0; i<pt.size(); i++){
                    float trg_SF = _muontrg->getWeight(std::abs(eta[i]),pt[i]);
                    float ID_SF = _muonid->getWeight(std::abs(eta[i]),pt[i]);
                    float Iso_SF = _muoniso->getWeight(std::abs(eta[i]),pt[i]);
                    weight *= trg_SF * ID_SF * Iso_SF;
                }
            }
            return weight;
        };
        _rlm = _rlm.Define("evWeight_muonSF",muonSF,{"Sel_muonpt","Sel_muoneta"});

        // Tau SF
        cout<<"Getting Tau ID Scale Factors"<<endl;
        auto tauSF = [this](floats &pt, floats &eta, uchars &genid)->float {
            float weight = 1.0;
            if(pt.size() > 1){
                //cout<<pt.size()<<" taus"<<endl;
                for(unsigned int i=0; i<pt.size(); i++){
                    float tauidsfVSjet = _tauidSFjet->getSFvsPT(pt[i],int(genid[i]));
                    float tauidsfVSele = _tauidSFele->getSFvsEta(eta[i],int(genid[i]));
                    //float tauidsfVSmu = _tauidSFmu->getSFvsEta(eta[i],int(genid[i]));
                    //weight *= tauidsfVSjet*tauidsfVSele*tauidsfVSmu;
                    weight *= tauidsfVSjet*tauidsfVSele;
                }
            }
            return weight;
        };
        
        _rlm = _rlm.Define("Sel_tauflav","Tau_genPartFlav[seltaucuts]")
                   .Define("evWeight_tauSF", tauSF, {"Sel_taupt","Sel_taueta","Sel_tauflav"})
                   .Define("evWeight_leptonSF","evWeight_muonSF*evWeight_tauSF");

        // B tagging SF
        _rlm = _rlm.Define("Sel_jethadflav","Jet_hadronFlavour[jetcuts]")
                   .Define("Selmu_jethadflav","Sel_jethadflav[muonjetoverlap]");

        // function to calculate event weight for MC events based on DeepJet algorithm
        auto btagweightgenerator= [this](floats &pts, floats &etas, ints &hadflav, floats &btags)->float
        {
            double bweight=1.0;
            BTagEntry::JetFlavor hadfconv;
            for (unsigned int i=0; i<pts.size(); i++)
            {
                if (hadflav[i]==5) hadfconv=BTagEntry::FLAV_B;
                else if (hadflav[i]==4) hadfconv=BTagEntry::FLAV_C;
                else hadfconv=BTagEntry::FLAV_UDSG;

                double w = _btagcalibreader.eval_auto_bounds("central", hadfconv, fabs(etas[i]), pts[i], btags[i]);
                bweight *= w;
            }
            return bweight;
        };

        cout<<"Generate b-tagging weight"<<endl;
        _rlm = _rlm.Define("btagWeight_DeepFlavBrecalc", btagweightgenerator, {"Selmu_jetpt", "Selmu_jeteta", "Selmu_jethadflav", "Selmu_jetbtag"});
        _rlm = _rlm.Define("evWeight", "pugenWeight * btagWeight_DeepFlavBrecalc * evWeight_leptonSF");
}

/*
bool NanoAODAnalyzerrdframe::helper_1DHistCreator(std::string hname, std::string title, const int nbins, const double xlow, const double xhi, std::string rdfvar, std::string evWeight)
{
	RDF1DHist histojets = _rlm.Histo1D({hname.c_str(), title.c_str(), nbins, xlow, xhi}, rdfvar, evWeight); // Fill with weight given by evWeight
	_th1dhistos[hname] = histojets;
}
*/

void NanoAODAnalyzerrdframe::helper_1DHistCreator(std::string hname, std::string title, const int nbins, const double xlow, const double xhi, std::string rdfvar, std::string evWeight, RNode *anode)
{
	RDF1DHist histojets = anode->Histo1D({hname.c_str(), title.c_str(), nbins, xlow, xhi}, rdfvar, evWeight); // Fill with weight given by evWeight
	_th1dhistos[hname] = histojets;
}
;

// Automatically loop to create
void NanoAODAnalyzerrdframe::setupCuts_and_Hists()
{
	cout << "setting up definitions, cuts, and histograms" <<endl;

	for ( auto &c : _varinfovector)
	{
		if (c.mincutstep.length()==0) _rlm = _rlm.Define(c.varname, c.vardefinition);
	}

	for (auto &x : _hist1dinfovector)
	{
		std::string hpost = "_nocut";

		if (x.mincutstep.length()==0)
		{
			helper_1DHistCreator(std::string(x.hmodel.fName)+hpost,  std::string(x.hmodel.fTitle)+hpost, x.hmodel.fNbinsX, x.hmodel.fXLow, x.hmodel.fXUp, x.varname, x.weightname, &_rlm);
		}
	}

	_rnt.setRNode(&_rlm);

	for (auto acut : _cutinfovector)
	{
		std::string cutname = "cut"+ acut.idx;
		std::string hpost = "_"+cutname;
		RNode *r = _rnt.getParent(acut.idx)->getRNode();
		auto rnext = new RNode(r->Define(cutname, acut.cutdefinition));
		*rnext = rnext->Filter(cutname);

		for ( auto &c : _varinfovector)
		{
			if (acut.idx.compare(c.mincutstep)==0) *rnext = rnext->Define(c.varname, c.vardefinition);
		}
		for (auto &x : _hist1dinfovector)
		{
			if (acut.idx.compare(0, x.mincutstep.length(), x.mincutstep)==0)
			{
				helper_1DHistCreator(std::string(x.hmodel.fName)+hpost,  std::string(x.hmodel.fTitle)+hpost, x.hmodel.fNbinsX, x.hmodel.fXLow, x.hmodel.fXUp, x.varname, x.weightname, rnext);
			}
		}
		_rnt.addDaughter(rnext, acut.idx);

		/*
		_rlm = _rlm.Define(cutname, acut.cutdefinition);
		_rlm = _rlm.Filter(cutname);

		for ( auto &c : _varinfovector)
		{
			if (acut.idx.compare(c.mincutstep)==0) _rlm = _rlm.Define(c.varname, c.vardefinition);
		}
		for (auto &x : _hist1dinfovector)
		{
			if (acut.idx.compare(0, x.mincutstep.length(), x.mincutstep)==0)
			{
				helper_1DHistCreator(std::string(x.hmodel.fName)+hpost,  std::string(x.hmodel.fTitle)+hpost, x.hmodel.fNbinsX, x.hmodel.fXLow, x.hmodel.fXUp, x.varname, x.weightname);
			}
		}
		_rnt.addDaughter(&_rlm, acut.idx);
		*/
	}
}

void NanoAODAnalyzerrdframe::add1DHist(TH1DModel histdef, std::string variable, std::string weight,string mincutstep)
{
	_hist1dinfovector.push_back({histdef, variable, weight, mincutstep});
}


void NanoAODAnalyzerrdframe::drawHists(RNode t)
{
	cout << "processing" <<endl;
	t.Count();
}

void NanoAODAnalyzerrdframe::addVar(varinfo v)
{
	_varinfovector.push_back(v);
}

void NanoAODAnalyzerrdframe::addVartoStore(string varname)
{
	// varname is assumed to be a regular expression.
	// e.g. if varname is "Muon_eta" then "Muon_eta" will be stored
	// if varname=="Muon_.*", then any branch name that starts with "Muon_" string will
	// be saved
	_varstostore.push_back(varname);
	/*
	std::regex b(varname);
	bool foundmatch = false;
	for (auto a: _rlm.GetColumnNames())
	{
		if (std::regex_match(a, b)) {
			_varstostore.push_back(a);
			foundmatch = true;
		}
	}
	*/

}

void NanoAODAnalyzerrdframe::setupTree()
{
	vector<RNodeTree *> rntends;
	_rnt.getRNodeLeafs(rntends);
	for (auto arnt: rntends)
	{
		RNode *arnode = arnt->getRNode();
		string nodename = arnt->getIndex();
		vector<string> varforthistree;
		std::map<string, int> varused;

		for (auto varname: _varstostore)
		{
			bool foundmatch = false;
			std::regex b(varname);
			for (auto a: arnode->GetColumnNames())
			{
				if (std::regex_match(a, b) && varused[a]==0)
				{
					varforthistree.push_back(a);
					varused[a]++;
					foundmatch = true;
				}
			}
			if (!foundmatch)
			{
				cout << varname << " not found at "<< nodename << endl;
			}

		}
		_varstostorepertree[nodename]  = varforthistree;
	}

}

void NanoAODAnalyzerrdframe::addCuts(string cut, string idx)
{
	_cutinfovector.push_back({cut, idx});
}


void NanoAODAnalyzerrdframe::run(bool saveAll, string outtreename)
{
	/*
	if (saveAll) {
		_rlm.Snapshot(outtreename, _outfilename);
	}
	else {
		// use the following if you want to store only a few variables
		_rlm.Snapshot(outtreename, _outfilename, _varstostore);
	}
	*/

	vector<RNodeTree *> rntends;
	_rnt.getRNodeLeafs(rntends);
	_rnt.Print();
        cout << rntends.size() << endl;
	// on master, regex_replace doesn't work somehow
	//std::regex rootextension("\\.root");

	for (auto arnt: rntends)
	{
		string nodename = arnt->getIndex();
		//string outname = std::regex_replace(_outfilename, rootextension, "_"+nodename+".root");
		string outname = _outfilename;
		// if producing many root files due to branched selection criteria,  each root file will get a different name
		if (rntends.size()>1) outname.replace(outname.find(".root"), 5, "_"+nodename+".root");
		_outrootfilenames.push_back(outname);
		RNode *arnode = arnt->getRNode();
                cout << arnt->getIndex();
		//cout << ROOT::RDF::SaveGraph(_rlm) << endl;
		if (saveAll) {
			arnode->Snapshot(outtreename, outname);
		}
		else {
			// use the following if you want to store only a few variables
			//arnode->Snapshot(outtreename, outname, _varstostore);
			cout << " writing branches" << endl;
			for (auto bname: _varstostorepertree[nodename])
			{
				cout << bname << ", ";
			}
                        cout<<endl;
			arnode->Snapshot(outtreename, outname, _varstostorepertree[nodename]);
		}
	}



}
