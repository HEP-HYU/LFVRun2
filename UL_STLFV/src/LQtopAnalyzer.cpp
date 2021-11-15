/*
 * LQtopAnalyzer.cpp
 *
 *  Created on: April 9, 2020
 *      Author: Tae Jeong Kim
 */

#include "LQtopAnalyzer.h"
#include "utility.h"

LQtopAnalyzer::LQtopAnalyzer(TTree *t, std::string outfilename, std::string year, std::string syst, std::string jsonfname, string globaltag, int nthreads)
:NanoAODAnalyzerrdframe(t, outfilename, year, syst, jsonfname, globaltag, nthreads)
{

}

// Define your cuts here
void LQtopAnalyzer::defineCuts()
{
	// Cuts to be applied in order
	// These will be passed to Filter method of RDF
	// check for good json event is defined earlier
        addCuts("nmuonpass == 1 && nvetoelepass == 0 && nvetomuons == 0","0");
        addCuts("ncleantaupass == 1 && mutau_mass > 150", "00");
        addCuts("ncleanjetspass >= 3 && Sel2_jet1pt > 100", "000");
        addCuts("ncleanbjetspass == 1", "0000");
}
/*
template <typename T>
void LQtopAnalyzer::defineVar(std::string varname, T function,  const RDFDetail::ColumnNames_t &columns)
{
	NanoAODAnalyzerrdframe::defineVar(varname, function, columns);
}
*/

void LQtopAnalyzer::defineMoreVars()
{
	// define your higher level variables here
        defineVar("muonvec",::select_leadingvec,{"muon4vecs"});
        defineVar("tauvec",::select_leadingvec,{"cleantau4vecs"});
        defineVar("mutau_mass",::calculate_invMass,{"muonvec","tauvec"});
        defineVar("mutau_dEta",::calculate_deltaEta,{"muonvec","tauvec"});
        addVar({"mutau_absdEta","abs(mutau_dEta)",""});
        defineVar("mutau_dPhi",::calculate_deltaPhi,{"muonvec","tauvec"});
        addVar({"mutau_absdPhi","abs(mutau_dPhi)",""});
        defineVar("mutau_dR",::calculate_deltaR,{"muonvec","tauvec"});
	
        addVar({"Sel_muon1pt", "Sel_muonpt[0]", ""});
        addVar({"Sel_muon1eta", "Sel_muoneta[0]", ""});
        addVar({"Sel_muon1abseta", "abs(Sel_muon1eta)", ""});
        addVar({"Sel_muon1mass", "Sel_muonmass[0]", ""});
	
        addVar({"Sel_tau1pt", "Sel_taupt[0]", ""});
        addVar({"Sel_tau1eta", "Sel_taueta[0]", ""});
        addVar({"Sel_tau1abseta", "abs(Sel_tau1eta)", ""});
        addVar({"Sel_tau1mass", "Sel_taumass[0]", ""});

        addVar({"Sel2_jet1pt", "(Sel2_jetpt.size()>0) ? Sel2_jetpt[0] : -1", ""});
        addVar({"Sel2_jet1eta", "Sel2_jeteta[0]", ""});
        addVar({"Sel2_jet1abseta", "abs(Sel2_jet1eta)", ""});
        addVar({"Sel2_jet1mass", "Sel2_jetmass[0]", ""});

        addVar({"Sel2_jet2pt", "Sel2_jetpt[1]", ""});
        addVar({"Sel2_jet2eta", "Sel2_jeteta[1]", ""});
        addVar({"Sel2_jet2abseta", "abs(Sel2_jet2eta)", ""});
        addVar({"Sel2_jet2mass", "Sel2_jetmass[1]", ""});

        addVar({"Sel2_jet3pt", "Sel2_jetpt[2]", ""});
        addVar({"Sel2_jet3eta", "Sel2_jeteta[2]", ""});
        addVar({"Sel2_jet3abseta", "abs(Sel2_jet3eta)", ""});
        addVar({"Sel2_jet3mass", "Sel2_jetmass[2]", ""});

        addVar({"Sel2_jet1btag","Sel2_jetbtag[0]",""});
        addVar({"Sel2_jet2btag","Sel2_jetbtag[1]",""});
        addVar({"Sel2_jet3btag","Sel2_jetbtag[2]",""});
	
        addVar({"Sel2_bjet1pt", "Sel2_bjetpt[0]", ""});
        addVar({"Sel2_bjet1eta", "Sel2_bjeteta[0]", ""});
        addVar({"Sel2_bjet1abseta", "abs(Sel2_bjet1eta)", ""});
        addVar({"Sel2_bjet1mass", "Sel2_bjetmass[0]", ""});

        defineVar("top_reco_whad", ::top_reconstruction_whad, {"cleanjet4vecs","cleanbjet4vecs","muon4vecs","cleantau4vecs"});
        addVar({"chi2","top_reco_whad[0]",""});
        addVar({"chi2_SMW_mass","top_reco_whad[1]",""});
        addVar({"chi2_SMTop_mass","top_reco_whad[2]",""});
        addVar({"chi2_wjet1_idx","top_reco_whad[3]",""});
        addVar({"chi2_wjet2_idx","top_reco_whad[4]",""});
        addVar({"chi2_SMW","top_reco_whad[5]",""});
        addVar({"chi2_SMTop","top_reco_whad[6]",""});

        defineVar("top_reco_prod", ::top_reco_products, {"cleanjet4vecs","muon4vecs","cleantau4vecs","top_reco_whad"});

        addVar({"chi2_wqq_dEta","top_reco_prod[0]",""});
        addVar({"chi2_wqq_absdEta","abs(chi2_wqq_dEta)",""});
        addVar({"chi2_wqq_dPhi","top_reco_prod[1]",""});
        addVar({"chi2_wqq_absdPhi","abs(chi2_wqq_dPhi)",""});
        addVar({"chi2_wqq_dR","top_reco_prod[2]",""});

        addVar({"Jet_rawpt","Jet_pt*(1-Jet_rawFactor)"});

        addVar({"evWeight_pglep","pugenWeight*evWeight_leptonSF"});

        // define variables that you want to store
	addVartoStore("run");
	addVartoStore("luminosityBlock");
	addVartoStore("event");
	addVartoStore("evWeight.*");
        addVartoStore("nmuonpass");
	addVartoStore("ncleanjetspass");
	addVartoStore("ncleanbjetspass");
        addVartoStore("ncleantaupass");
        addVartoStore("nJet");
        addVartoStore("Jet_pt");
        addVartoStore("Jet_pt_corr");
        addVartoStore("Jet_rawpt");
        addVartoStore("Jet_rawFactor");
        addVartoStore("MET_pt");
        addVartoStore("MET_pt_corr");
        addVartoStore("MET_phi");
        addVartoStore("MET_phi_corr");
        addVartoStore("Sel_muon1.*");
        addVartoStore("Sel2_jet1.*");
        addVartoStore("Sel2_jet2.*");
        addVartoStore("Sel2_jet3.*");
        addVartoStore("Sel2_bjet1.*");
        addVartoStore("Sel_tau1.*");
        addVartoStore("chi2.*");
        addVartoStore("btagWeight_DeepFlavBrecalc.*");
}

void LQtopAnalyzer::bookHists()
{
	// _hist1dinfovector contains the information of histogram definitions (as TH1DModel)
	// the variable to be used for filling
	// and the minimum cutstep for which the histogram should be filled
	//
	// The braces are used to initalize the struct
        // Pre-set binnings for histograms
	// TH1D
        add1DHist( {"hnevents", "Number of Events", 2, -0.5, 1.5}, "one", "evWeight", "");
        add1DHist( {"hnevents_pglep", "Number of Events", 2, -0.5, 1.5}, "one", "evWeight_pglep", "");
	//add1DHist( {"hnvtx_raw", "Number of Primary Vertex", 200, 0.0, 200.0}, "PV_npvsGood", "one", "");
	//add1DHist( {"hnvtx", "Number of Primary Vertex", 200, 0.0, 200.0}, "PV_npvsGood", "evWeight", "");

        //=================== Fully weighted ===================
	add1DHist( {"hmetpt", "MET pt", 20, 0, 400}, "MET_pt_corr", "evWeight", "000");
	add1DHist( {"hsumet", "Sum ET", 50, 0.0, 5000.0}, "MET_sumEt", "evWeight", "000");
	add1DHist( {"hmetphi", "MET phi", 20, -4.0, 4.0}, "MET_phi_corr", "evWeight", "000");

//        add1DHist( {"hnpvdof", "Number of PV of DoF", 50, 0.0, 50.0}, "PV_ndof", "evWeight", "000");
//        add1DHist( {"hnpvs", "Number of PVs", 100, 0.0, 100.0}, "PV_npvs", "evWeight", "000");
//        add1DHist( {"hnpvsgood", "Number of good PVs", 100, 0.0, 100.0}, "PV_npvsGood", "evWeight", "000");

	add1DHist( {"hnmuonpass", "Passing muoncuts", 5, 0.0, 5.0}, "nmuonpass", "evWeight", "00");
	add1DHist( {"hncleantaupass", "Passing taucuts", 5, 0.0, 5.0}, "ncleantaupass", "evWeight", "00");
	add1DHist( {"hncleanjetspass", "Passing jetcuts", 10, 0.0, 10.0}, "ncleanjetspass", "evWeight", "00");
        add1DHist( {"hncleanbjetspass", "Passing bjetcuts", 5, 0.0, 5.0}, "ncleanbjetspass", "evWeight", "00");
        
        add1DHist( {"hmuon1pt", "Passing leading muon pt", 20, 0, 800}, "Sel_muon1pt", "evWeight", "000");
        add1DHist( {"hmuon1eta", "Passing leading muon eta", 18, -2.7, 2.7}, "Sel_muon1eta", "evWeight", "000");
        add1DHist( {"htau1pt", "Passing leading tau pt", 20, 0, 800}, "Sel_tau1pt", "evWeight", "000");
        add1DHist( {"htau1eta", "Passing leading tau eta", 18, -2.7, 2.7}, "Sel_tau1eta", "evWeight", "000");
        
        add1DHist( {"hmutau_dEta", "dEta of muon and tau", 25, -5, 5}, "mutau_dEta", "evWeight","0000");
        add1DHist( {"hmutau_absdEta", "abs dEta of muon and tau", 15, 0, 3.0}, "mutau_absdEta", "evWeight","0000");
        add1DHist( {"hmutau_dPhi", "dPhi of muon and tau", 20, -4, 4}, "mutau_dPhi", "evWeight","0000");
        add1DHist( {"hmutau_absdPhi", "absdPhi of muon and tau", 20, 0, 4}, "mutau_absdPhi", "evWeight","0000");
        add1DHist( {"hmutau_dR", "dR of muon and tau", 20, 0, 4.0}, "mutau_dR", "evWeight","0000");
        add1DHist( {"hmutau_mass", "Mass of muon and tau", 25, 0, 1000}, "mutau_mass", "evWeight","0000");

        add1DHist( {"hjet1pt", "Passing leading jet pt", 30, 0, 600}, "Sel2_jet1pt", "evWeight", "000");
        add1DHist( {"hjet1eta", "Passing leading jet eta", 18, -2.7, 2.7}, "Sel2_jet1eta", "evWeight", "000");
        add1DHist( {"hjet2pt", "Passing sub-leading jet pt", 30, 0, 600}, "Sel2_jet2pt", "evWeight", "000");
        add1DHist( {"hjet2eta", "Passing sub-leading jet eta", 18, -2.7, 2.7}, "Sel2_jet2eta", "evWeight", "000");
        add1DHist( {"hjet3pt", "Passing third jet pt", 30, 0, 600}, "Sel2_jet3pt", "evWeight", "000");
        add1DHist( {"hjet3eta", "Passing third jet eta", 18, -2.7, 2.7}, "Sel2_jet3eta", "evWeight", "000");
        add1DHist( {"hbjet1pt", "Passing b jet pt", 30, 0, 600}, "Sel2_bjet1pt", "evWeight", "0000");
        add1DHist( {"hbjet1eta", "Passing b jet eta", 18, -2.7, 2.7}, "Sel2_bjet1eta", "evWeight", "0000");

        add1DHist( {"hjet1btag","btag discr of leading jet", 20, 0, 1.0}, "Sel2_jet1btag", "evWeight", "000");
        add1DHist( {"hjet2btag","btag discr of sub-leading jet", 20, 0, 1.0}, "Sel2_jet2btag", "evWeight", "000");
        add1DHist( {"hjet3btag","btag discr of third jet", 20, 0, 1.0}, "Sel2_jet3btag", "evWeight", "000");
        
        //=================== PUGEN * Lepton SF ===================
	add1DHist( {"h1metpt", "MET pt", 20, 0, 600}, "MET_pt_corr", "evWeight_pglep", "0");
	add1DHist( {"h1sumet", "Sum ET", 50, 0.0, 5000.0}, "MET_sumEt", "evWeight_pglep", "0");
	add1DHist( {"h1metphi", "MET phi", 20, -4.0, 4.0}, "MET_phi_corr", "evWeight_pglep", "0");

//        add1DHist( {"h1npvdof", "Number of PV of DoF", 50, 0.0, 50.0}, "PV_ndof", "evWeight_pglep", "0");
//        add1DHist( {"h1npvs", "Number of PVs", 100, 0.0, 100.0}, "PV_npvs", "evWeight_pglep", "0");
//        add1DHist( {"h1npvsgood", "Number of good PVs", 100, 0.0, 100.0}, "PV_npvsGood", "evWeight_pglep", "0");

	add1DHist( {"h1nmuonpass", "Passing muoncuts", 5, 0.0, 5.0}, "nmuonpass", "evWeight_pglep", "0");
	add1DHist( {"h1ncleantaupass", "Passing taucuts", 5, 0.0, 5.0}, "ncleantaupass", "evWeight_pglep", "0");
	add1DHist( {"h1ncleanjetspass", "Passing jetcuts", 10, 0.0, 10.0}, "ncleanjetspass", "evWeight_pglep", "0");
        add1DHist( {"h1ncleanbjetspass", "Passing bjetcuts", 5, 0.0, 5.0}, "ncleanbjetspass", "evWeight_pglep", "0");

        add1DHist( {"h1muon1pt", "Passing leading muon pt", 20, 0, 800}, "Sel_muon1pt", "evWeight_pglep", "0");
        add1DHist( {"h1muon1eta", "Passing leading muon eta", 18, -2.7, 2.7}, "Sel_muon1eta", "evWeight_pglep", "0");
        add1DHist( {"h1tau1pt", "Passing leading tau pt", 20, 0, 800}, "Sel_tau1pt", "evWeight_pglep", "00");
        add1DHist( {"h1tau1eta", "Passing leading tau eta", 18, -2.7, 2.7}, "Sel_tau1eta", "evWeight_pglep", "00"); 

        add1DHist( {"h1mutau_dEta", "dEta of muon and tau", 25, -5, 5}, "mutau_dEta", "evWeight_pglep","00");
        add1DHist( {"h1mutau_absdEta", "abs dEta of muon and tau", 15, 0, 3.0}, "mutau_absdEta", "evWeight_pglep","00");
        add1DHist( {"h1mutau_dPhi", "dPhi of muon and tau", 20, -4, 4}, "mutau_dPhi", "evWeight_pglep","00");
        add1DHist( {"h1mutau_absdPhi", "absdPhi of muon and tau", 20, 0, 4}, "mutau_absdPhi", "evWeight_pglep","00");
        add1DHist( {"h1mutau_dR", "dR of muon and tau", 20, 0, 4.0}, "mutau_dR", "evWeight_pglep","00");
        add1DHist( {"h1mutau_mass", "Mass of muon and tau", 25, 0, 1000}, "mutau_mass", "evWeight_pglep","00");

        add1DHist( {"hchi2", "Minimum chi2 for hadronic W", 20, 0, 20000}, "chi2", "evWeight","0000");
        add1DHist( {"hchi2_SMTop_mass", "chi2 SM Top mass", 20, 0, 400}, "chi2_SMTop_mass", "evWeight","0000");
        add1DHist( {"hchi2_SMW_mass", "chi2 SM W mass", 20, 0, 400}, "chi2_SMW_mass", "evWeight","0000");

        add1DHist( {"hchi2_wqq_dEta", "dEta of jets from W", 25, -5, 5}, "chi2_wqq_dEta", "evWeight","0000");
        add1DHist( {"hchi2_wqq_absdEta", "abs dEta of jets from W", 15, 0, 3.0}, "chi2_wqq_absdEta", "evWeight","0000");
        add1DHist( {"hchi2_wqq_dPhi", "dPhi of jets from W", 20, -4, 4}, "chi2_wqq_dPhi", "evWeight","0000");
        add1DHist( {"hchi2_wqq_absdPhi", "absdPhi of jets from W", 20, 0, 4}, "chi2_wqq_absdPhi", "evWeight","0000");
        add1DHist( {"hchi2_wqq_dR", "dR of jets from W", 20, 0, 4.0}, "chi2_wqq_dR", "evWeight","0000");
}
