/*
 * TopLFVAnalyzer.cpp
 *
 *  Created on: April 9, 2020
 *      Author: Tae Jeong Kim
 *      Refactored: Jiwon Park
 */

#include "TopLFVAnalyzer.h"
#include "utility.h"

TopLFVAnalyzer::TopLFVAnalyzer(TTree *t, std::string outfilename, std::string year, std::string syst, std::string jsonfname, bool applytauFF, string globaltag, int nthreads)
:NanoAODAnalyzerrdframe(t, outfilename, year, syst, jsonfname, globaltag, nthreads), _outfilename(outfilename), _syst(syst), _year(year), _applytauFF(applytauFF)
{
    if(syst.find("jes") != std::string::npos or syst.find("jer") != std::string::npos or syst.find("metUnclust") != std::string::npos or
            syst.find("tes") != std::string::npos or syst.find("hdamp") != std::string::npos or syst.find("tune") != std::string::npos or
            syst.find("muonhighscale") != std::string::npos) {
        ext_syst = true;
    }
    _syst = syst;

    if (_year == "2016pre") {
        tauYear = "UL2016_preVFP";
    } else if (_year == "2016post") {
        tauYear = "UL2016_postVFP";
    } else {
        tauYear = "UL" + _year;
    }

    if (_outfilename.find("_LFV_TUMuTau_") != std::string::npos or _outfilename.find("_LFV_TCMuTau_") != std::string::npos) {
        _isSignal = true;
        cout << "Input file is LFV signal" <<endl;
    } else {
        _isSignal = false;
    }

}

// Define your cuts here
void TopLFVAnalyzer::defineCuts() {

    addCuts("nmuonpass == 1 && nvetoelepass == 0 && nvetomuons == 0 && PV_npvsGood > 0", "0");

    // All
    addCuts("ncleantaupass == 1", "00");
    // genuine taus
    //if (_isSignal or _syst == "data") {
    //    addCuts("ncleantaupass == 1", "00");
    //} else {
    //    addCuts("ncleantaupass == 1 && Tau_pt_gen.size()>0", "00");
    //}
    // fake taus
    //addCuts("ncleantaupass == 1 && Tau_pt_gen.size()==0", "00");

    addCuts("mutau_charge < 0", "000");
    addCuts("ncleanjetspass >= 3", "0000");
    addCuts("ncleanbjetspass == 1", "00000");
}

void TopLFVAnalyzer::defineMoreVars() {

    defineVar("muonvec", ::select_leadingvec, {"muon4vecs"});
    defineVar("tauvec", ::select_leadingvec, {"cleantau4vecs"});
    defineVar("mutau_mass", ::calculate_invMass, {"muonvec","tauvec"});
    defineVar("mutau_dEta", ::calculate_deltaEta, {"muonvec","tauvec"});
    defineVar("mutau_dPhi", ::calculate_deltaPhi, {"muonvec","tauvec"});
    defineVar("mutau_dR", ::calculate_deltaR, {"muonvec","tauvec"});
    defineVar("muMET_mt", ::calculate_MT, {"muon4vecs","MET_pt","MET_phi"});

    // Temporary solution to blind low mutau mass
    if (_syst == "data") {
        addVar({"mutau_mass_blind", "(mutau_mass < 100) ? -1.0 : mutau_mass"});
    } else {
        addVar({"mutau_mass_blind", "mutau_mass"});
    }

    //Adding unc for muon SF to top phase space: done in plotit and combine tool
    //defineVar("muonWeightId", ::addMuonUnc, {"muonWeightId"});
    //defineVar("muonWeightIso", ::addMuonUnc, {"muonWeightIso"});
    //defineVar("muonWeightTrg", ::addMuonUnc, {"muonWeightTrg"});

    // Already object selection is done before this
    addVar({"isFakeTau", "!(Tau_pt_gen.size()>0)"});
    if (!_applytauFF) addVar({"tauFF", "1.0", ""});
    else if (_isSignal) {
        addVar({"tauFF", "1.0", ""});
        addVar({"tauFFstatup", "1.0", ""});
        addVar({"tauFFstatdown", "1.0", ""});
        addVar({"tauFFsystup", "1.0", ""});
        addVar({"tauFFsystdown", "1.0", ""});
    } else {
        auto tauFF_nom = tauFFfunctor(_year, "nom", 0);
        auto tauFF_statup = tauFFfunctor(_year, "stat", 1);
        auto tauFF_statdown = tauFFfunctor(_year, "stat", -1);
        auto tauFF_systup = tauFFfunctor(_year, "syst", 1);
        auto tauFF_systdown = tauFFfunctor(_year, "syst", -1);
        defineVar("tauFF", tauFF_nom, {"Tau_pt", "Tau_pt_gen", "Tau_decayMode"});
        defineVar("tauFFstatup", tauFF_statup, {"Tau_pt", "Tau_pt_gen", "Tau_decayMode"});
        defineVar("tauFFstatdown", tauFF_statdown, {"Tau_pt", "Tau_pt_gen", "Tau_decayMode"});
        defineVar("tauFFsystup", tauFF_systup, {"Tau_pt", "Tau_pt_gen", "Tau_decayMode"});
        defineVar("tauFFsystdown", tauFF_systdown, {"Tau_pt", "Tau_pt_gen", "Tau_decayMode"});
    }
    addVar({"unitGenWeightFF", "unitGenWeight * tauFF * UFO_reweight", ""});

    // There should be 'good' tau (or none) and exactly one muon
    addVar({"Muon1_pt", "Muon_pt[0]", ""});
    addVar({"Muon1_eta", "Muon_eta[0]", ""});
    addVar({"Muon1_mass", "Muon_mass[0]", ""});
    addVar({"Muon1_charge", "Muon_charge[0]", ""});

    addVar({"Tau1_pt", "Tau_pt[0]", ""});
    addVar({"Tau1_eta", "Tau_eta[0]", ""});
    addVar({"Tau1_mass", "Tau_mass[0]", ""});
    addVar({"Tau1_charge", "Tau_charge[0]", ""});
    addVar({"Tau1_decayMode", "Tau_decayMode[0]", ""});

    addVar({"mutau_charge", "Muon1_charge * Tau1_charge", ""});

    addVar({"Jet1_pt", "(Jet_pt.size()>0) ? Jet_pt[0] : -1", ""});
    addVar({"Jet1_eta", "Jet_eta[0]", ""});
    addVar({"Jet1_mass", "Jet_mass[0]", ""});
    addVar({"Jet1_btagDeepFlavB","Jet_btagDeepFlavB[0]",""});

    addVar({"Jet2_pt", "Jet_pt[1]", ""});
    addVar({"Jet2_eta", "Jet_eta[1]", ""});
    addVar({"Jet2_mass", "Jet_mass[1]", ""});
    addVar({"Jet2_btagDeepFlavB","Jet_btagDeepFlavB[1]",""});

    addVar({"Jet3_pt", "Jet_pt[2]", ""});
    addVar({"Jet3_eta", "Jet_eta[2]", ""});
    addVar({"Jet3_mass", "Jet_mass[2]", ""});
    addVar({"Jet3_btagDeepFlavB","Jet_btagDeepFlavB[2]",""});


    addVar({"bJet1_pt", "bJet_pt[0]", ""});
    addVar({"bJet1_eta", "bJet_eta[0]", ""});
    addVar({"bJet1_mass", "bJet_mass[0]", ""});

    //if (_syst == "data") {
    //    defineVar("tauWeightIdVsJetFIX", ::fixtausf, {"tauWeightIdVsJet", "Tau_pt"});
    //}

    // Reconstruction
    defineVar("top_reco_whad", ::top_reconstruction_STLFV, {"cleanjet4vecs","cleanbjet4vecs","muon4vecs","cleantau4vecs"});
    addVar({"chi2", "top_reco_whad[0]",""});
    addVar({"chi2_SMW_mass", "top_reco_whad[1]",""});
    addVar({"chi2_SMTop_mass", "top_reco_whad[2]",""});
    addVar({"chi2_wjet1_idx", "top_reco_whad[3]",""});
    addVar({"chi2_wjet2_idx", "top_reco_whad[4]",""});
    addVar({"chi2_SMW", "top_reco_whad[5]",""});
    addVar({"chi2_SMTop", "top_reco_whad[6]",""});
    addVar({"chi2_SMTop_pt", "top_reco_whad[7]",""});

    defineVar("top_reco_prod", ::top_reco_products_STLFV, {"cleanjet4vecs","muon4vecs","cleantau4vecs","top_reco_whad"});
    addVar({"chi2_wqq_dEta","top_reco_prod[0]",""});
    addVar({"chi2_wqq_dPhi","top_reco_prod[1]",""});
    addVar({"chi2_wqq_dR","top_reco_prod[2]",""});

    // S_T^MET: in EXO-19-016, defined by pt1+pt2+ptj1+met where pt1,2 come from taus
    defineVar("st_met", ::st_met, {"Muon_pt", "Tau_pt", "Jet_pt", "MET_pt"});


    // EventWeights
    // Calculate product of weights and store for systematic study
    // External systs: JES (+btag) (14), JER(2), TES(2), hdamp(2), TuneCP5(2)
    // Weights systs: genWeight(1), PU(2), btag(16), muon Id(2)/Iso(2)/Trg(2), tauId(18+2*2), ME scale(6), PS scale(ISR 2, FSR 2), PDF(102, or 2)
    // Not implemented: EEprefire, top pt reweighting,
    // eventWeight_xx : xxweight
    // eventWeight__xx: xx unc.

    addVar({"muonHighPtAddUncUp", "Muon1_pt > 200 ? 1.0 + (0.000125 * Muon1_pt - 0.025): 1.0", ""});
    addVar({"muonHighPtAddUncDn", "Muon1_pt > 200 ? 1.0 - (0.000125 * Muon1_pt - 0.025): 1.0", ""});


    if (_syst == "data") {
        addVar({"eventWeight", "1.0"});
        addVar({"eventWeight_notau", "1.0"});
        addVar({"eventWeight_notoppt", "1.0"});
    } else {
        addVar({"eventWeight_genpu", "unitGenWeightFF * TopPtWeight[0] * puWeight[0] * L1PreFiringWeight_Nom"});
        addVar({"eventWeight_mu", "muonWeightId[0] * muonWeightIso[0] * muonWeightTrg[0]"});
        addVar({"eventWeight_tau", "tauWeightIdVsJet[0][0] * tauWeightIdVsEl[0][0] * tauWeightIdVsMu[0][0]"});
        addVar({"eventWeight_genpumu", "eventWeight_genpu * eventWeight_mu"});
        addVar({"eventWeight_notau_nobtag", "eventWeight_genpumu"}); //didn't want to duplicate entry...
        addVar({"eventWeight_genputau", "eventWeight_genpu * eventWeight_tau"});
        addVar({"eventWeight_nobtag", "eventWeight_genpu * eventWeight_mu * eventWeight_tau"});
        addVar({"eventWeight_nopu", "unitGenWeightFF * TopPtWeight[0] * L1PreFiringWeight_Nom * eventWeight_mu * eventWeight_tau * btagWeight_DeepFlavB[0]"});
        addVar({"eventWeight_noprefire", "unitGenWeightFF * TopPtWeight[0] * puWeight[0] * eventWeight_mu * eventWeight_tau * btagWeight_DeepFlavB[0]"});
        addVar({"eventWeight_notoppt", "unitGenWeightFF * puWeight[0] * L1PreFiringWeight_Nom * eventWeight_mu * eventWeight_tau * btagWeight_DeepFlavB_jes[0]"});

        if (_syst == "" or _syst == "nosyst" or ext_syst) {
            // for external syst, we only need nominal weight
            if (_syst.find("jesAbsoluteup") != std::string::npos) {
                addVar({"eventWeight", "eventWeight_nobtag * btagWeight_DeepFlavB_jes[0]"});
                addVar({"eventWeight_notau", "eventWeight_genpumu * btagWeight_DeepFlavB_jes[0]"});
            } else if (_syst.find("jesAbsolutedown") != std::string::npos) {
                addVar({"eventWeight", "eventWeight_nobtag * btagWeight_DeepFlavB_jes[1]"});
                addVar({"eventWeight_notau", "eventWeight_genpumu * btagWeight_DeepFlavB_jes[1]"});
            } else if (_syst.find("jesAbsolute_" + _year.substr(0,4) + "up") != std::string::npos) {
                addVar({"eventWeight", "eventWeight_nobtag * btagWeight_DeepFlavB_jes[2]"});
                addVar({"eventWeight_notau", "eventWeight_genpumu * btagWeight_DeepFlavB_jes[2]"});
            } else if (_syst.find("jesAbsolute_" + _year.substr(0,4) + "down") != std::string::npos) {
                addVar({"eventWeight", "eventWeight_nobtag * btagWeight_DeepFlavB_jes[3]"});
                addVar({"eventWeight_notau", "eventWeight_genpumu * btagWeight_DeepFlavB_jes[3]"});
            } else if (_syst.find("jesBBEC1up") != std::string::npos) {
                addVar({"eventWeight", "eventWeight_nobtag * btagWeight_DeepFlavB_jes[4]"});
                addVar({"eventWeight_notau", "eventWeight_genpumu * btagWeight_DeepFlavB_jes[4]"});
            } else if (_syst.find("jesBBEC1down") != std::string::npos) {
                addVar({"eventWeight", "eventWeight_nobtag * btagWeight_DeepFlavB_jes[5]"});
                addVar({"eventWeight_notau", "eventWeight_genpumu * btagWeight_DeepFlavB_jes[5]"});
            } else if (_syst.find("jesBBEC1_" + _year.substr(0,4) + "up") != std::string::npos) {
                addVar({"eventWeight", "eventWeight_nobtag * btagWeight_DeepFlavB_jes[6]"});
                addVar({"eventWeight_notau", "eventWeight_genpumu * btagWeight_DeepFlavB_jes[6]"});
            } else if (_syst.find("jesBBEC1_" + _year.substr(0,4) + "down") != std::string::npos) {
                addVar({"eventWeight", "eventWeight_nobtag * btagWeight_DeepFlavB_jes[7]"});
                addVar({"eventWeight_notau", "eventWeight_genpumu * btagWeight_DeepFlavB_jes[7]"});
            } else if (_syst.find("jesFlavorQCDup") != std::string::npos or
                       _syst.find("jesFlavorPureGluonup") != std::string::npos or
                       _syst.find("jesFlavorPureQuarkup") != std::string::npos or
                       _syst.find("jesFlavorPureCharmup") != std::string::npos or
                       _syst.find("jesFlavorPureBottomup") != std::string::npos
                      ) {
                addVar({"eventWeight", "eventWeight_nobtag * btagWeight_DeepFlavB_jes[8]"});
                addVar({"eventWeight_notau", "eventWeight_genpumu * btagWeight_DeepFlavB_jes[8]"});
            } else if (_syst.find("jesFlavorQCDdown") != std::string::npos or
                       _syst.find("jesFlavorPureGluondown") != std::string::npos or
                       _syst.find("jesFlavorPureQuarkdown") != std::string::npos or
                       _syst.find("jesFlavorPureCharmdown") != std::string::npos or
                       _syst.find("jesFlavorPureBottomdown") != std::string::npos
                      ) {
                addVar({"eventWeight", "eventWeight_nobtag * btagWeight_DeepFlavB_jes[9]"});
                addVar({"eventWeight_notau", "eventWeight_genpumu * btagWeight_DeepFlavB_jes[9]"});
            } else if (_syst.find("jesRelativeBalup") != std::string::npos) {
                addVar({"eventWeight", "eventWeight_nobtag * btagWeight_DeepFlavB_jes[10]"});
                addVar({"eventWeight_notau", "eventWeight_genpumu * btagWeight_DeepFlavB_jes[10]"});
            } else if (_syst.find("jesRelativeBaldown") != std::string::npos) {
                addVar({"eventWeight", "eventWeight_nobtag * btagWeight_DeepFlavB_jes[11]"});
                addVar({"eventWeight_notau", "eventWeight_genpumu * btagWeight_DeepFlavB_jes[11]"});
            } else if (_syst.find("jesRelativeSample_" + _year.substr(0,4) + "up") != std::string::npos) {
                addVar({"eventWeight", "eventWeight_nobtag * btagWeight_DeepFlavB_jes[12]"});
                addVar({"eventWeight_notau", "eventWeight_genpumu * btagWeight_DeepFlavB_jes[12]"});
            } else if (_syst.find("jesRelativeSample_" + _year.substr(0,4) + "down") != std::string::npos) {
                addVar({"eventWeight", "eventWeight_nobtag * btagWeight_DeepFlavB_jes[13]"});
                addVar({"eventWeight_notau", "eventWeight_genpumu * btagWeight_DeepFlavB_jes[13]"});
            } else if (_syst.find("jesHEMup") != std::string::npos && _year == "2018") { //HEM
                addVar({"eventWeight", "eventWeight_nobtag * btagWeight_DeepFlavB[0]"});
                addVar({"eventWeight_notau", "eventWeight_genpumu * btagWeight_DeepFlavB[0]"});
            } else if (_syst.find("jesHEMdown") != std::string::npos && _year == "2018") { //HEM down - dummy
                addVar({"eventWeight", "eventWeight_nobtag * btagWeight_DeepFlavB[0]"});
                addVar({"eventWeight_notau", "eventWeight_genpumu * btagWeight_DeepFlavB[0]"});
            } else {
                addVar({"eventWeight", "eventWeight_nobtag * btagWeight_DeepFlavB[0]"});
                addVar({"eventWeight_notau", "eventWeight_genpumu * btagWeight_DeepFlavB[0]"});
            }
        } else {
            addVar({"eventWeight", "eventWeight_nobtag * btagWeight_DeepFlavB[0]"});
            addVar({"eventWeight_notau", "eventWeight_genpu * eventWeight_mu * btagWeight_DeepFlavB[0]"});
            addVar({"eventWeight__puup", "eventWeight_nopu * puWeight[1]"});
            addVar({"eventWeight__pudown", "eventWeight_nopu * puWeight[2]"});
            addVar({"eventWeight__topptup", "eventWeight_nobtag * btagWeight_DeepFlavB[0] * TopPtWeight[1] / TopPtWeight[0]"});
            addVar({"eventWeight__topptdown", "eventWeight_nobtag * btagWeight_DeepFlavB[0] * TopPtWeight[2] / TopPtWeight[0]"});
            addVar({"eventWeight__prefireup", "eventWeight_noprefire * L1PreFiringWeight_Up"});
            addVar({"eventWeight__prefiredown", "eventWeight_noprefire * L1PreFiringWeight_Dn"});
            addVar({"eventWeight__muidup", "eventWeight_genputau * muonWeightId[1] * muonWeightIso[0] * muonWeightTrg[0] * btagWeight_DeepFlavB[0]"});
            addVar({"eventWeight__muiddown", "eventWeight_genputau * muonWeightId[2] * muonWeightIso[0] * muonWeightTrg[0] * btagWeight_DeepFlavB[0]"});
            addVar({"eventWeight__muisoup", "eventWeight_genputau * muonWeightId[0] * muonWeightIso[1] * muonWeightTrg[0] * btagWeight_DeepFlavB[0]"});
            addVar({"eventWeight__muisodown", "eventWeight_genputau * muonWeightId[0] * muonWeightIso[2] * muonWeightTrg[0] * btagWeight_DeepFlavB[0]"});
            addVar({"eventWeight__mutrgup", "eventWeight_genputau * muonWeightId[0] * muonWeightIso[0] * muonWeightTrg[1] * btagWeight_DeepFlavB[0]"});
            addVar({"eventWeight__mutrgdown", "eventWeight_genputau * muonWeightId[0] * muonWeightIso[0] * muonWeightTrg[2] * btagWeight_DeepFlavB[0]"});
            addVar({"eventWeight__muhighptup", "eventWeight * muonHighPtAddUncUp"});
            addVar({"eventWeight__muhighptdown", "eventWeight * muonHighPtAddUncDn"});
            //addVar({"eventWeight__tauidjetup", "eventWeight_notau * tauWeightIdVsJet[0][1] * tauWeightIdVsEl[0][0] * tauWeightIdVsMu[0][0]"});
            //addVar({"eventWeight__tauidjetdown", "eventWeight_notau * tauWeightIdVsJet[0][2] * tauWeightIdVsEl[0][0] * tauWeightIdVsMu[0][0]"});
            addVar({"eventWeight__tauidjetUncert0up", "eventWeight_notau * tauWeightIdVsJet[0][1] * tauWeightIdVsEl[0][0] * tauWeightIdVsMu[0][0]"});
            addVar({"eventWeight__tauidjetUncert0down", "eventWeight_notau * tauWeightIdVsJet[0][2] * tauWeightIdVsEl[0][0] * tauWeightIdVsMu[0][0]"});
            addVar({"eventWeight__tauidjetUncert1up", "eventWeight_notau * tauWeightIdVsJet[0][3] * tauWeightIdVsEl[0][0] * tauWeightIdVsMu[0][0]"});
            addVar({"eventWeight__tauidjetUncert1down", "eventWeight_notau * tauWeightIdVsJet[0][4] * tauWeightIdVsEl[0][0] * tauWeightIdVsMu[0][0]"});
            addVar({"eventWeight__tauidjetSystallerasup", "eventWeight_notau * tauWeightIdVsJet[0][5] * tauWeightIdVsEl[0][0] * tauWeightIdVsMu[0][0]"});
            addVar({"eventWeight__tauidjetSystallerasdown", "eventWeight_notau * tauWeightIdVsJet[0][6] * tauWeightIdVsEl[0][0] * tauWeightIdVsMu[0][0]"});
            addVar({"eventWeight__tauidjetSyst"+tauYear+"up", "eventWeight_notau * tauWeightIdVsJet[0][7] * tauWeightIdVsEl[0][0] * tauWeightIdVsMu[0][0]"});
            addVar({"eventWeight__tauidjetSyst"+tauYear+"down", "eventWeight_notau * tauWeightIdVsJet[0][8] * tauWeightIdVsEl[0][0] * tauWeightIdVsMu[0][0]"});
            addVar({"eventWeight__tauidjetSystdm0"+tauYear+"up", "eventWeight_notau * tauWeightIdVsJet[0][9] * tauWeightIdVsEl[0][0] * tauWeightIdVsMu[0][0]"});
            addVar({"eventWeight__tauidjetSystdm0"+tauYear+"down", "eventWeight_notau * tauWeightIdVsJet[0][10] * tauWeightIdVsEl[0][0] * tauWeightIdVsMu[0][0]"});
            addVar({"eventWeight__tauidjetSystdm1"+tauYear+"up", "eventWeight_notau * tauWeightIdVsJet[0][11] * tauWeightIdVsEl[0][0] * tauWeightIdVsMu[0][0]"});
            addVar({"eventWeight__tauidjetSystdm1"+tauYear+"down", "eventWeight_notau * tauWeightIdVsJet[0][12] * tauWeightIdVsEl[0][0] * tauWeightIdVsMu[0][0]"});
            addVar({"eventWeight__tauidjetSystdm10"+tauYear+"up", "eventWeight_notau * tauWeightIdVsJet[0][13] * tauWeightIdVsEl[0][0] * tauWeightIdVsMu[0][0]"});
            addVar({"eventWeight__tauidjetSystdm10"+tauYear+"down", "eventWeight_notau * tauWeightIdVsJet[0][14] * tauWeightIdVsEl[0][0] * tauWeightIdVsMu[0][0]"});
            addVar({"eventWeight__tauidjetSystdm11"+tauYear+"up", "eventWeight_notau * tauWeightIdVsJet[0][15] * tauWeightIdVsEl[0][0] * tauWeightIdVsMu[0][0]"});
            addVar({"eventWeight__tauidjetSystdm11"+tauYear+"down", "eventWeight_notau * tauWeightIdVsJet[0][16] * tauWeightIdVsEl[0][0] * tauWeightIdVsMu[0][0]"});
            addVar({"eventWeight__tauidjetHighptstatup", "eventWeight_notau * tauWeightIdVsJet[0][17] * tauWeightIdVsEl[0][0] * tauWeightIdVsMu[0][0]"});
            addVar({"eventWeight__tauidjetHighptstatdown", "eventWeight_notau * tauWeightIdVsJet[0][18] * tauWeightIdVsEl[0][0] * tauWeightIdVsMu[0][0]"});
            addVar({"eventWeight__tauidjetHighptstat_bin1up", "eventWeight_notau * tauWeightIdVsJet[0][19] * tauWeightIdVsEl[0][0] * tauWeightIdVsMu[0][0]"});
            addVar({"eventWeight__tauidjetHighptstat_bin1down", "eventWeight_notau * tauWeightIdVsJet[0][20] * tauWeightIdVsEl[0][0] * tauWeightIdVsMu[0][0]"});
            addVar({"eventWeight__tauidjetHighptstat_bin2up", "eventWeight_notau * tauWeightIdVsJet[0][21] * tauWeightIdVsEl[0][0] * tauWeightIdVsMu[0][0]"});
            addVar({"eventWeight__tauidjetHighptstat_bin2down", "eventWeight_notau * tauWeightIdVsJet[0][22] * tauWeightIdVsEl[0][0] * tauWeightIdVsMu[0][0]"});
            addVar({"eventWeight__tauidjetHighptsystup", "eventWeight_notau * tauWeightIdVsJet[0][23] * tauWeightIdVsEl[0][0] * tauWeightIdVsMu[0][0]"});
            addVar({"eventWeight__tauidjetHighptsystdown", "eventWeight_notau * tauWeightIdVsJet[0][24] * tauWeightIdVsEl[0][0] * tauWeightIdVsMu[0][0]"});
            addVar({"eventWeight__tauidjetHighptextrapup", "eventWeight_notau * tauWeightIdVsJet[0][25] * tauWeightIdVsEl[0][0] * tauWeightIdVsMu[0][0]"});
            addVar({"eventWeight__tauidjetHighptextrapdown", "eventWeight_notau * tauWeightIdVsJet[0][26] * tauWeightIdVsEl[0][0] * tauWeightIdVsMu[0][0]"});
            addVar({"eventWeight__tauidelup", "eventWeight_notau * tauWeightIdVsJet[0][0] * tauWeightIdVsEl[0][1] * tauWeightIdVsMu[0][0]"});
            addVar({"eventWeight__tauideldown", "eventWeight_notau * tauWeightIdVsJet[0][0] * tauWeightIdVsEl[0][2] * tauWeightIdVsMu[0][0]"});
            addVar({"eventWeight__tauidmuup", "eventWeight_notau * tauWeightIdVsJet[0][0] * tauWeightIdVsEl[0][0] * tauWeightIdVsMu[0][1]"});
            addVar({"eventWeight__tauidmudown", "eventWeight_notau * tauWeightIdVsJet[0][0] * tauWeightIdVsEl[0][0] * tauWeightIdVsMu[0][2]"});
            addVar({"eventWeight__btaghfup", "eventWeight_nobtag * btagWeight_DeepFlavB[1]"});
            addVar({"eventWeight__btaghfdown", "eventWeight_nobtag * btagWeight_DeepFlavB[2]"});
            addVar({"eventWeight__btaglfup", "eventWeight_nobtag * btagWeight_DeepFlavB[3]"});
            addVar({"eventWeight__btaglfdown", "eventWeight_nobtag * btagWeight_DeepFlavB[4]"});
            addVar({"eventWeight__btaghfstats1up", "eventWeight_nobtag * btagWeight_DeepFlavB[5]"});
            addVar({"eventWeight__btaghfstats1down", "eventWeight_nobtag * btagWeight_DeepFlavB[6]"});
            addVar({"eventWeight__btaghfstats2up", "eventWeight_nobtag * btagWeight_DeepFlavB[7]"});
            addVar({"eventWeight__btaghfstats2down", "eventWeight_nobtag * btagWeight_DeepFlavB[8]"});
            addVar({"eventWeight__btaglfstats1up", "eventWeight_nobtag * btagWeight_DeepFlavB[9]"});
            addVar({"eventWeight__btaglfstats1down", "eventWeight_nobtag * btagWeight_DeepFlavB[10]"});
            addVar({"eventWeight__btaglfstats2up", "eventWeight_nobtag * btagWeight_DeepFlavB[11]"});
            addVar({"eventWeight__btaglfstats2down", "eventWeight_nobtag * btagWeight_DeepFlavB[12]"});
            addVar({"eventWeight__btagcferr1up", "eventWeight_nobtag * btagWeight_DeepFlavB[13]"});
            addVar({"eventWeight__btagcferr1down", "eventWeight_nobtag * btagWeight_DeepFlavB[14]"});
            addVar({"eventWeight__btagcferr2up", "eventWeight_nobtag * btagWeight_DeepFlavB[15]"});
            addVar({"eventWeight__btagcferr2down", "eventWeight_nobtag * btagWeight_DeepFlavB[16]"});

            if (_applytauFF) {
                addVar({"eventWeight__tauFFstatup", "eventWeight * tauFFstatup"});
                addVar({"eventWeight__tauFFstatdown", "eventWeight * tauFFstatdown"});
                addVar({"eventWeight__tauFFsystup", "eventWeight * tauFFsystup"});
                addVar({"eventWeight__tauFFsystdown", "eventWeight * tauFFsystdown"});
            }

            // no tau - nominal is eventWeight_notau
            addVar({"eventWeight_notau_nopu", "unitGenWeightFF * TopPtWeight[0] * L1PreFiringWeight_Nom * eventWeight_mu * btagWeight_DeepFlavB[0]"});
            addVar({"eventWeight_notau__puup", "unitGenWeightFF * TopPtWeight[0] * puWeight[1] * L1PreFiringWeight_Nom * eventWeight_mu * btagWeight_DeepFlavB[0]"});
            addVar({"eventWeight_notau__pudown", "unitGenWeightFF * TopPtWeight[0] * puWeight[2] * L1PreFiringWeight_Nom * eventWeight_mu * btagWeight_DeepFlavB[0]"});
            addVar({"eventWeight_notau__topptup", "unitGenWeightFF * TopPtWeight[1] * puWeight[0] * L1PreFiringWeight_Nom * eventWeight_mu * btagWeight_DeepFlavB[0]"});
            addVar({"eventWeight_notau__topptdown", "unitGenWeightFF * TopPtWeight[2] * puWeight[0] * L1PreFiringWeight_Nom * eventWeight_mu * btagWeight_DeepFlavB[0]"});
            addVar({"eventWeight_notau__prefireup", "unitGenWeightFF * TopPtWeight[0] * puWeight[0] * L1PreFiringWeight_Up * eventWeight_mu * btagWeight_DeepFlavB[0]"});
            addVar({"eventWeight_notau__prefiredown", "unitGenWeightFF * TopPtWeight[0] * puWeight[0] * L1PreFiringWeight_Dn * eventWeight_mu * btagWeight_DeepFlavB[0]"});
            addVar({"eventWeight_notau__muidup", "eventWeight_genpu * muonWeightId[1] * muonWeightIso[0] * muonWeightTrg[0] * btagWeight_DeepFlavB[0]"});
            addVar({"eventWeight_notau__muiddown", "eventWeight_genpu * muonWeightId[2] * muonWeightIso[0] * muonWeightTrg[0] * btagWeight_DeepFlavB[0]"});
            addVar({"eventWeight_notau__muisoup", "eventWeight_genpu * muonWeightId[0] * muonWeightIso[1] * muonWeightTrg[0] * btagWeight_DeepFlavB[0]"});
            addVar({"eventWeight_notau__muisodown", "eventWeight_genpu * muonWeightId[0] * muonWeightIso[2] * muonWeightTrg[0] * btagWeight_DeepFlavB[0]"});
            addVar({"eventWeight_notau__mutrgup", "eventWeight_genpu * muonWeightId[0] * muonWeightIso[0] * muonWeightTrg[1] * btagWeight_DeepFlavB[0]"});
            addVar({"eventWeight_notau__mutrgdown", "eventWeight_genpu * muonWeightId[0] * muonWeightIso[0] * muonWeightTrg[2] * btagWeight_DeepFlavB[0]"});
            addVar({"eventWeight_notau__muhighptup", "eventWeight_notau * muonHighPtAddUncUp"});
            addVar({"eventWeight_notau__muhighptdown", "eventWeight_notau * muonHighPtAddUncDn"});
            addVar({"eventWeight_notau__btaghfup", "eventWeight_genpumu * btagWeight_DeepFlavB[1]"});
            addVar({"eventWeight_notau__btaghfdown", "eventWeight_genpumu * btagWeight_DeepFlavB[2]"});
            addVar({"eventWeight_notau__btaglfup", "eventWeight_genpumu * btagWeight_DeepFlavB[3]"});
            addVar({"eventWeight_notau__btaglfdown", "eventWeight_genpumu * btagWeight_DeepFlavB[4]"});
            addVar({"eventWeight_notau__btaghfstats1up", "eventWeight_genpumu * btagWeight_DeepFlavB[5]"});
            addVar({"eventWeight_notau__btaghfstats1down", "eventWeight_genpumu * btagWeight_DeepFlavB[6]"});
            addVar({"eventWeight_notau__btaghfstats2up", "eventWeight_genpumu * btagWeight_DeepFlavB[7]"});
            addVar({"eventWeight_notau__btaghfstats2down", "eventWeight_genpumu * btagWeight_DeepFlavB[8]"});
            addVar({"eventWeight_notau__btaglfstats1up", "eventWeight_genpumu * btagWeight_DeepFlavB[9]"});
            addVar({"eventWeight_notau__btaglfstats1down", "eventWeight_genpumu * btagWeight_DeepFlavB[10]"});
            addVar({"eventWeight_notau__btaglfstats2up", "eventWeight_genpumu * btagWeight_DeepFlavB[11]"});
            addVar({"eventWeight_notau__btaglfstats2down", "eventWeight_genpumu * btagWeight_DeepFlavB[12]"});
            addVar({"eventWeight_notau__btagcferr1up", "eventWeight_genpumu * btagWeight_DeepFlavB[13]"});
            addVar({"eventWeight_notau__btagcferr1down", "eventWeight_genpumu * btagWeight_DeepFlavB[14]"});
            addVar({"eventWeight_notau__btagcferr2up", "eventWeight_genpumu * btagWeight_DeepFlavB[15]"});
            addVar({"eventWeight_notau__btagcferr2down", "eventWeight_genpumu * btagWeight_DeepFlavB[16]"});

            if (_syst == "theory") {
                // ME: [0] is renscfact=0.5d0 facscfact=0.5d0 ; [1] is renscfact=0.5d0 facscfact=1d0 ; [3] is renscfact=1d0 facscfact=0.5d0 ;
                //     [5] is renscfact=1d0 facscfact=2d0 ; [7] is renscfact=2d0 facscfact=1d0 ; [8] is renscfact=2d0 facscfact=2d0
                addVar({"eventWeight__mescaledown", "eventWeight * LHEScaleWeight[0]"});
                addVar({"eventWeight__renscaledown", "eventWeight * LHEScaleWeight[1]"});
                addVar({"eventWeight__facscaledown", "eventWeight * LHEScaleWeight[3]"});
                addVar({"eventWeight__facscaleup", "eventWeight * LHEScaleWeight[5]"});
                addVar({"eventWeight__renscaleup", "eventWeight * LHEScaleWeight[7]"});
                addVar({"eventWeight__mescaleup", "eventWeight * LHEScaleWeight[8]"});
                // notau
                addVar({"eventWeight_notau__mescaledown", "eventWeight_notau * LHEScaleWeight[0]"});
                addVar({"eventWeight_notau__renscaledown", "eventWeight_notau * LHEScaleWeight[1]"});
                addVar({"eventWeight_notau__facscaledown", "eventWeight_notau * LHEScaleWeight[3]"});
                addVar({"eventWeight_notau__facscaleup", "eventWeight_notau * LHEScaleWeight[5]"});
                addVar({"eventWeight_notau__renscaleup", "eventWeight_notau * LHEScaleWeight[7]"});
                addVar({"eventWeight_notau__mescaleup", "eventWeight_notau * LHEScaleWeight[8]"});
                // PS: [0] is ISR=2 FSR=1; [1] is ISR=1 FSR=2; [2] is ISR=0.5 FSR=1; [3] is ISR=1 FSR=0.5;
                addVar({"eventWeight__isrup", "eventWeight * PSWeight[0]"});
                addVar({"eventWeight__fsrup", "eventWeight * PSWeight[1]"});
                addVar({"eventWeight__isrdown", "eventWeight * PSWeight[2]"});
                addVar({"eventWeight__fsrdown", "eventWeight * PSWeight[3]"});
                // notau
                addVar({"eventWeight_notau__isrup", "eventWeight_notau * PSWeight[0]"});
                addVar({"eventWeight_notau__fsrup", "eventWeight_notau * PSWeight[1]"});
                addVar({"eventWeight_notau__isrdown", "eventWeight_notau * PSWeight[2]"});
                addVar({"eventWeight_notau__fsrdown", "eventWeight_notau * PSWeight[3]"});
                // PDF: LHA IDs 306000 - 306102. 306000 = nominal.
                // TODO Check if always 103 entries are stored. unless code will crash.
                for (int i=1; i<=102; i++) {
                    addVar({"eventWeight__pdf" + std::to_string(i), "eventWeight * LHEPdfWeight[" + std::to_string(i) + "]"});
                    addVar({"eventWeight_notau__pdf" + std::to_string(i), "eventWeight_notau * LHEPdfWeight[" + std::to_string(i) + "]"});
                }
                addVar({"eventWeight__pdfalphasup", "eventWeight * LHEPdfWeight[102]"}); //approx.
                addVar({"eventWeight__pdfalphasdown", "eventWeight * LHEPdfWeight[101]"});
                addVar({"eventWeight_notau__pdfalphasup", "eventWeight_notau * LHEPdfWeight[102]"});
                addVar({"eventWeight_notau__pdfalphasdown", "eventWeight_notau * LHEPdfWeight[101]"});
            }
        }
    }

    // define variables that you want to store
    addVartoStore("run");
    addVartoStore("event");
    addVartoStore("eventWeight.*");
    addVartoStore("nmuonpass");
    addVartoStore("ncleanjetspass");
    addVartoStore("ncleanbjetspass");
    addVartoStore("ncleantaupass");
    addVartoStore("Jet_pt");
    addVartoStore("Jet1_.*");
    addVartoStore("Jet2_.*");
    addVartoStore("Jet3_.*");
    addVartoStore("bJet1_.*");
    addVartoStore("Tau1.*");
    addVartoStore("Muon1.*");
    addVartoStore("mutau.*");
    addVartoStore("MET_pt");
    addVartoStore("MET_phi");
    addVartoStore("chi2.*");
    addVartoStore("btagWeight_DeepFlavB");
    addVartoStore("btagWeight_DeepFlavB_jes");
    addVartoStore("eventWeight.*");
    addVartoStore("GenPart_top_pt");
    addVartoStore("TopPtWeight");
    addVartoStore("LHEPart_pt");
    addVartoStore("LHEPart_pdgId");
    addVartoStore("tauFF.*");
    addVartoStore("isFakeTau");
}

void TopLFVAnalyzer::bookHists() {

    std::vector<std::string> init_weight = {""};
    std::vector<std::string> sf_weight = {"", "_nobtag", "_nopu", "_notau", "_notoppt",
                   "__puup", "__pudown", "__topptup", "__topptdown", "__prefireup", "__prefiredown",
                   "__muidup", "__muiddown", "__muisoup", "__muisodown", "__mutrgup", "__mutrgdown",
                   "__muhighptup", "__muhighptdown",
                   "__btaghfup", "__btaghfdown", "__btaglfup", "__btaglfdown",
                   "__btaghfstats1up", "__btaghfstats1down", "__btaghfstats2up", "__btaghfstats2down",
                   "__btaglfstats1up", "__btaglfstats1down", "__btaglfstats2up", "__btaglfstats2down",
                   "__btagcferr1up", "__btagcferr1down", "__btagcferr2up", "__btagcferr2down",
                   };

    std::vector<std::string> sf_weight_tau = {"__tauidjetUncert0up", "__tauidjetUncert0down",
                                              "__tauidjetUncert1up", "__tauidjetUncert1down",
                                              "__tauidjetSystallerasup", "__tauidjetSystallerasdown",
                                              "__tauidjetSyst"+tauYear+"up", "__tauidjetSyst"+tauYear+"down",
                                              "__tauidjetSystdm0"+tauYear+"up", "__tauidjetSystdm0"+tauYear+"down",
                                              "__tauidjetSystdm1"+tauYear+"up", "__tauidjetSystdm1"+tauYear+"down",
                                              "__tauidjetSystdm10"+tauYear+"up", "__tauidjetSystdm10"+tauYear+"down",
                                              "__tauidjetSystdm11"+tauYear+"up", "__tauidjetSystdm11"+tauYear+"down",
                                              "__tauidjetHighptstatup", "__tauidjetHighptstatdown",
                                              "__tauidjetHighptstat_bin1up", "__tauidjetHighptstat_bin1down",
                                              "__tauidjetHighptstat_bin2up", "__tauidjetHighptstat_bin2down",
                                              "__tauidjetHighptsystup", "__tauidjetHighptsystdown",
                                              "__tauidjetHighptextrapup", "__tauidjetHighptextrapdown",
                                              "__tauidelup", "__tauideldown", "__tauidmuup", "__tauidmudown"};

    std::vector<std::string> sf_weight_FF ={"__tauFFstatup", "__tauFFstatdown", "__tauFFsystup", "__tauFFsystdown"};

    std::vector<std::string> theory_weight = {"__isrup", "__fsrup", "__isrdown", "__fsrdown",
                   "__mescaleup", "__mescaledown", "__renscaleup", "__renscaledown", "__facscaleup", "__facscaledown",
                   "__pdfalphasup", "__pdfalphasdown"};
                   //Not including pdf eigenvectors for plots


    //Step definition. To replace for tauFF
    //This implies histos w/o FF must be drawn, for bSF and other weights renormalization
    std::string minstep_S1 = "0";
    std::string minstep_S2 = "00";
    //std::string minstep_S3 = "000";
    std::string minstep_S4 = "0000";
    std::string minstep_S5 = "00000";

    if (_applytauFF) {
        minstep_S1 = "00000";
        minstep_S2 = "00000";
        //minstep_S3 = "00000";
        minstep_S4 = "00000";
        minstep_S5 = "00000";
    }

    // TODO refine this. Too heavy? we will see.
    std::vector<std::string> syst_weight;
    if (_syst == "" or _syst == "nosyst" or _syst == "data" or ext_syst) {
        syst_weight = init_weight;
        if (_syst != "data") {
            //We anyway need this for bSF rescaling
            add1DHist({"h_nevents", ";Number of events w/o b SF;Events", 2, -0.5, 1.5}, "one", "eventWeight", "_nobtag", minstep_S1, "");
            add1DHist({"h_nevents_notausf", ";Number of events w/o b and tau SF;Events", 2, -0.5, 1.5}, "one", "eventWeight_notau", "_nobtag", minstep_S1, "00");
            add1DHist({"h_jet_ht", ";Jet HT w/o b SF (GeV);Events", 48, 40, 1000}, "Jet_HT", "eventWeight", "_nobtag", minstep_S1, "");
        }
    }
    else {
        syst_weight = sf_weight;
        if (_syst == "theory") syst_weight.insert(syst_weight.end(), theory_weight.begin(), theory_weight.end());
    }

    // S1 w/o tau SF
    maxstep = "00"; //Must be +1 step than its cut
    for (std::string weightstr : syst_weight) {
        if (weightstr.find("notau") != std::string::npos) continue;
        if (weightstr.find("notoppt") != std::string::npos) continue;

        add1DHist({"h_nevents_notausf", ";Number of events;Events", 2, -0.5, 1.5}, "one", "eventWeight_notau", weightstr, minstep_S1, maxstep);
        add1DHist({"h_nvtx_notausf", ";Number of primary vertex;Events", 70, 0.0, 70.0}, "PV_npvsGood", "eventWeight_notau", weightstr, minstep_S1, maxstep);

        add1DHist({"h_met_pt_notausf", ";MET (GeV);Events", 20, 0, 400}, "MET_pt", "eventWeight_notau", weightstr, minstep_S1, maxstep);
        add1DHist({"h_sum_et_notausf", ";Sum ET;Events", 50, 0.0, 5000.0}, "MET_sumEt", "eventWeight_notau", weightstr, minstep_S1, maxstep);
        add1DHist({"h_met_phi_notausf", ";MET #phi;Events", 20, -3.2, 3.2}, "MET_phi", "eventWeight_notau", weightstr, minstep_S1, maxstep);

        add1DHist({"h_nmuonpass_notausf", ";Muon multiplicity;Events", 5, 0.0, 5.0}, "nmuonpass", "eventWeight_notau", weightstr, minstep_S1, maxstep);
        add1DHist({"h_ncleantaupass_notausf", ";#tau_{h} multiplicity;Events", 5, 0.0, 5.0}, "ncleantaupass", "eventWeight_notau", weightstr, minstep_S1, maxstep);
        add1DHist({"h_ncleanjetspass_notausf", ";Jet multiplicity;Events", 10, 0.0, 10.0}, "ncleanjetspass", "eventWeight_notau", weightstr, minstep_S1, maxstep);
        add1DHist({"h_ncleanbjetspass_notausf", ";b-tagged jet multiplicity;Events", 5, 0.0, 5.0}, "ncleanbjetspass", "eventWeight_notau", weightstr, minstep_S1, maxstep);

        add1DHist({"h_muon1_pt_notausf", ";Muon p_{T} (GeV);Events", 30, 0, 600}, "Muon1_pt", "eventWeight_notau", weightstr, minstep_S1, maxstep);
        add1DHist({"h_muon1_eta_notausf", ";Muon #eta;Events", 20, -2.4, 2.4}, "Muon1_eta", "eventWeight_notau", weightstr, minstep_S1, maxstep);
        add1DHist({"h_muMET_mt_notausf", ";m_{T}(#mu, MET) (GeV);Events", 20, 0, 400}, "muMET_mt", "eventWeight_notau", weightstr, minstep_S1, maxstep);

        add1DHist({"h_jet1_pt_notausf", ";Leading jet p_{T} (GeV);Events", 20, 0, 400}, "Jet1_pt", "eventWeight_notau", weightstr, minstep_S1, maxstep);
        add1DHist({"h_jet1_eta_notausf", ";Leading jet #eta;Events", 20, -2.4, 2.4}, "Jet1_eta", "eventWeight_notau", weightstr, minstep_S1, maxstep);
        add1DHist({"h_jet1_mass_notausf", ";Leading jet mass (GeV);Events", 20, 0, 100}, "Jet1_mass", "eventWeight_notau", weightstr, minstep_S1, maxstep);
        add1DHist({"h_jet1_btag_notausf",";DeepFlavour score of leading jet;Events", 20, 0, 1.0}, "Jet1_btagDeepFlavB", "eventWeight_notau", weightstr, minstep_S1, maxstep);
    }

    //for all the other nominal histograms with tauSF
    if (_syst == "all" or _syst == "theory") {
        syst_weight.insert(syst_weight.end(), sf_weight_tau.begin(), sf_weight_tau.end());
        if (_applytauFF) syst_weight.insert(syst_weight.end(), sf_weight_FF.begin(), sf_weight_FF.end());
    }

    cout << "Variations to take care :";
    for (auto i : syst_weight) cout << i << " ";
    cout << endl;

    maxstep = "";

    for (std::string weightstr : syst_weight) {
        if (weightstr.find("notoppt") != std::string::npos) continue;

        add1DHist({"h_nevents", ";Number of events;Events", 2, -0.5, 1.5}, "one", "eventWeight", weightstr, minstep_S1, maxstep);
        add1DHist({"h_nvtx", ";Number of primary vertex;Events", 70, 0.0, 70.0}, "PV_npvsGood", "eventWeight", weightstr, minstep_S1, maxstep);

        add1DHist({"h_met_pt", ";MET (GeV);Events", 20, 0, 400}, "MET_pt", "eventWeight", weightstr, minstep_S1, maxstep);
        add1DHist({"h_sum_et", ";Sum ET;Events", 50, 0.0, 5000.0}, "MET_sumEt", "eventWeight", weightstr, minstep_S1, maxstep);
        add1DHist({"h_met_phi", ";MET #phi;Events", 20, -3.2, 3.2}, "MET_phi", "eventWeight", weightstr, minstep_S1, maxstep);

        add1DHist({"h_nmuonpass", ";Muon multiplicity;Events", 5, 0.0, 5.0}, "nmuonpass", "eventWeight", weightstr, minstep_S1, maxstep);
        add1DHist({"h_ncleantaupass", ";#tau_{h} multiplicity;Events", 5, 0.0, 5.0}, "ncleantaupass", "eventWeight", weightstr, minstep_S1, maxstep);
        add1DHist({"h_ncleanjetspass", ";Jet multiplicity;Events", 10, 0.0, 10.0}, "ncleanjetspass", "eventWeight", weightstr, minstep_S1, maxstep);
        add1DHist({"h_ncleanbjetspass", ";b-tagged jet multiplicity;Events", 5, 0.0, 5.0}, "ncleanbjetspass", "eventWeight", weightstr, minstep_S1, maxstep);

        add1DHist({"h_muon1_pt", ";Muon p_{T} (GeV);Events", 30, 0, 600}, "Muon1_pt", "eventWeight", weightstr, minstep_S1, maxstep);
        add1DHist({"h_muon1_eta", ";Muon #eta;Events", 20, -2.4, 2.4}, "Muon1_eta", "eventWeight", weightstr, minstep_S1, maxstep);
        add1DHist({"h_muMET_mt", ";m_{T}(#mu, MET) (GeV);Events", 20, 0, 400}, "muMET_mt", "eventWeight", weightstr, minstep_S1, maxstep);

        add1DHist({"h_tau1_pt", ";#tau_{h} p_{T} (GeV);Events", 20, 0, 400}, "Tau1_pt", "eventWeight", weightstr, minstep_S2, maxstep);
        add1DHist({"h_tau1_eta", ";#tau_{h} #eta;Events", 20, -2.3, 2.3}, "Tau1_eta", "eventWeight", weightstr, minstep_S2, maxstep);
        add1DHist({"h_tau1_mass", ";m_{#tau_{h}} (GeV);Events", 20, 0, 100}, "Tau1_mass", "eventWeight", weightstr, minstep_S2, maxstep);
        add1DHist({"h_tau1_decayMode", ";Decaymode of #tau_{h};Events", 15, 0, 15}, "Tau1_decayMode", "eventWeight", weightstr, minstep_S2, maxstep);

        add1DHist({"h_mutau_dEta", ";#Delta#eta(#mu, #tau_{h});Events", 25, -5, 5}, "mutau_dEta", "eventWeight", weightstr, minstep_S2, maxstep);
        add1DHist({"h_mutau_dPhi", ";#Delta#phi(#mu, #tau_{h});Events", 20, -3.2, 3.2}, "mutau_dPhi", "eventWeight", weightstr, minstep_S2, maxstep);
        add1DHist({"h_mutau_dR", ";#Delta R(#mu, #tau_{h});Events", 20, 0, 4.0}, "mutau_dR", "eventWeight", weightstr, minstep_S2, maxstep);
        add1DHist({"h_mutau_mass", ";Mass of #mu + #tau_{h} (GeV);Events", 40, 0, 1000}, "mutau_mass", "eventWeight", weightstr, minstep_S2, maxstep);
        add1DHist({"h_mutau_mass_blind", ";Mass of #mu + #tau_{h} (GeV);Events", 40, 0, 1000}, "mutau_mass_blind", "eventWeight", weightstr, minstep_S2, maxstep);

        add1DHist({"h_jet1_pt", ";Leading jet p_{T} (GeV);Events", 20, 0, 400}, "Jet1_pt", "eventWeight", weightstr, minstep_S1, maxstep);
        add1DHist({"h_jet1_eta", ";Leading jet #eta;Events", 20, -2.4, 2.4}, "Jet1_eta", "eventWeight", weightstr, minstep_S1, maxstep);
        add1DHist({"h_jet1_mass", ";Leading jet mass (GeV);Events", 20, 0, 100}, "Jet1_mass", "eventWeight", weightstr, minstep_S1, maxstep);
        add1DHist({"h_jet1_btag",";DeepFlavour score of leading jet;Events", 20, 0, 1.0}, "Jet1_btagDeepFlavB", "eventWeight", weightstr, minstep_S1, maxstep);

        add1DHist({"h_jet2_pt", ";Subleading jet p_{T} (GeV);Events", 20, 0, 400}, "Jet2_pt", "eventWeight", weightstr, minstep_S4, maxstep);
        add1DHist({"h_jet2_eta", ";Subleading jet #eta;Events", 20, -2.4, 2.4}, "Jet2_eta", "eventWeight", weightstr, minstep_S4, maxstep);
        add1DHist({"h_jet2_mass", ";Subleading jet mass (GeV);Events", 20, 0, 100}, "Jet2_mass", "eventWeight", weightstr, minstep_S4, maxstep);
        add1DHist({"h_jet2_btag",";DeepFlavour score of subleading jet;Events", 20, 0, 1.0}, "Jet2_btagDeepFlavB", "eventWeight", weightstr, minstep_S4, maxstep);

        add1DHist({"h_jet3_pt", ";Subsubleading jet p_{T} (GeV);Events", 20, 0, 400}, "Jet3_pt", "eventWeight", weightstr, minstep_S4, maxstep);
        add1DHist({"h_jet3_eta", ";Subsubleading jet #eta;Events", 20, -2.4, 2.4}, "Jet3_eta", "eventWeight", weightstr, minstep_S4, maxstep);
        add1DHist({"h_jet3_mass", ";Subsubleading jet mass (GeV);Events", 20, 0, 100}, "Jet3_mass", "eventWeight", weightstr, minstep_S4, maxstep);
        add1DHist({"h_jet3_btag",";DeepFlavour score of subsubleading jet;Events", 20, 0, 1.0}, "Jet3_btagDeepFlavB", "eventWeight", weightstr, minstep_S4, maxstep);

        add1DHist({"h_bjet1_pt", ";b-tagged jet p_{T} (GeV);Events", 20, 0, 400}, "bJet1_pt", "eventWeight", weightstr, minstep_S5, maxstep);
        add1DHist({"h_bjet1_eta", ";b-tagged jet #eta;Events", 20, -2.4, 2.4}, "bJet1_eta", "eventWeight", weightstr, minstep_S5, maxstep);
        add1DHist({"h_bjet1_mass", ";b-tagged jet mass (GeV);Events", 20, 0, 100}, "bJet1_mass", "eventWeight", weightstr, minstep_S5, maxstep);

        add1DHist({"h_jet_ht", ";Jet HT (GeV);Events", 48, 40, 1000}, "Jet_HT", "eventWeight", weightstr, minstep_S1, maxstep);
        add1DHist({"h_st_met", ";S_{T}^{MET} (GeV);Events", 28, 100, 1500}, "st_met", "eventWeight", weightstr, minstep_S4, maxstep);
        // Fiilld with the same st_met, to be drawn with "WIDTH" option.
        add1DHist({"h_st_met_binwidth", ";S_{T}^{MET};Events / GeV", 28, 100, 1500}, "st_met", "eventWeight", weightstr, minstep_S4, maxstep);

        // Histogram of Top mass reconstruction
        add1DHist({"h_chi2", ";Minimum #chi^{2};Events", 20, 0, 1000}, "chi2", "eventWeight", weightstr, minstep_S5, maxstep);
        add1DHist({"h_chi2_SMTop_mass", ";SM top quark mass (GeV);Events", 20, 80, 880}, "chi2_SMTop_mass", "eventWeight", weightstr, minstep_S5, maxstep);
        add1DHist({"h_chi2_SMTop_pt", ";SM top quark pt (GeV);Events", 20, 0, 400}, "chi2_SMTop_pt", "eventWeight", weightstr, minstep_S5, maxstep);
        add1DHist({"h_chi2_SMTop_pt_notoppt", ";SM top quark pt (GeV);Events", 20, 0, 400}, "chi2_SMTop_pt", "eventWeight_notoppt", "", minstep_S5, maxstep);
        add1DHist({"h_chi2_SMW_mass", ";SM W_{had} mass (GeV);Events", 20, 0, 400}, "chi2_SMW_mass", "eventWeight", weightstr, minstep_S5, maxstep);
        add1DHist({"h_chi2_wqq_dEta", ";#Delta#eta of jets from W;Events", 25, -5, 5}, "chi2_wqq_dEta", "eventWeight", weightstr, minstep_S5, maxstep);
        add1DHist({"h_chi2_wqq_dPhi", ";#Delta#phi of jets from W;Events;", 20, -4, 4}, "chi2_wqq_dPhi", "eventWeight", weightstr, minstep_S5, maxstep);
        add1DHist({"h_chi2_wqq_dR", ";#Delta R of jets from W;Events", 20, 0, 4.0}, "chi2_wqq_dR", "eventWeight", weightstr, minstep_S5, maxstep);
    }

}

double TopLFVAnalyzer::tauFF(std::string year_, std::string unc_, int direction_, floats &tau_pt_, floats &tau_gen_pt_, ints &tau_dm_) {

    double val = 1.0;

    // For geniune tau, unc and SF are always 1.0
    if (tau_gen_pt_.size() > 0) return 1.0;

    //Assume exactly one tau for FF (S5)
    //if (tau_pt_.size() != 1) return 9999999.;

    if (abs(direction_) != 1 and direction_ != 0) return val;
    std::map<std::string, std::map<std::string, double>> map_ff;

    // unc: ratio to nominal
    //map_ff["2016pre"]["nom"]   = 0.5555 ;
    //map_ff["2016pre"]["stat"]  = 0.06578;
    //map_ff["2016pre"]["syst"]  = 0.224  ;
    //map_ff["2016post"]["nom"]  = 0.6094 ;
    //map_ff["2016post"]["stat"] = 0.07114;
    //map_ff["2016post"]["syst"] = 0.04278;
    //map_ff["2017"]["nom"]      = 0.7233 ;
    //map_ff["2017"]["stat"]     = 0.03818;
    //map_ff["2017"]["syst"]     = 0.1292 ;
    //map_ff["2018"]["nom"]      = 0.7393 ;
    //map_ff["2018"]["stat"]     = 0.03239;
    //map_ff["2018"]["syst"]     = 0.08293;
    //cleaned
    //map_ff["2018"]["nom"]      = 0.5954 ;
    //map_ff["2018"]["stat"]     = 0.03254;
    //map_ff["2018"]["syst"]     = 0.2057 ;

    //if (tau_pt_[0] < 140) {
    //    map_ff["2016pre"]["nom"]   = 0.4889 ;
    //    map_ff["2016pre"]["stat"]  = 0.06907;
    //    map_ff["2016pre"]["syst"]  = 0.2267 ;
    //    map_ff["2016post"]["nom"]  = 0.5870 ;
    //    map_ff["2016post"]["stat"] = 0.07535;
    //    map_ff["2016post"]["syst"] = 0.08154;
    //    map_ff["2017"]["nom"]      = 0.6909 ;
    //    map_ff["2017"]["stat"]     = 0.03997;
    //    map_ff["2017"]["syst"]     = 0.1499 ;
    //    map_ff["2018"]["nom"]      = 0.7232 ;
    //    map_ff["2018"]["stat"]     = 0.0331 ;
    //    map_ff["2018"]["syst"]     = 0.08565;
    //} else if (tau_pt_[0] >= 140) {
    //    map_ff["2016pre"]["nom"]   = 0.9194 ;
    //    map_ff["2016pre"]["stat"]  = 0.2231 ;
    //    map_ff["2016pre"]["syst"]  = 0.3434 ;
    //    map_ff["2016post"]["nom"]  = 0.6079 ;
    //    map_ff["2016post"]["stat"] = 0.2227 ;
    //    map_ff["2016post"]["syst"] = 0.1087 ;
    //    map_ff["2017"]["nom"]      = 0.7810 ;
    //    map_ff["2017"]["stat"]     = 0.13   ;
    //    map_ff["2017"]["syst"]     = 0.05533;
    //    map_ff["2018"]["nom"]      = 0.8236 ;
    //    map_ff["2018"]["stat"]     = 0.1362 ;
    //    map_ff["2018"]["syst"]     = 0.1879 ;
    //}

    if (tau_dm_[0] == 0) {
        map_ff["2016pre"]["nom"]  = 0.3029;
        map_ff["2016pre"]["stat"] = 0.1235;
        map_ff["2016pre"]["syst"] = 0.3515;
        map_ff["2016post"]["nom"]  = 0.6584;
        map_ff["2016post"]["stat"] = 0.1212;
        map_ff["2016post"]["syst"] = 0.3563;
        map_ff["2017"]["nom"]  = 0.6122;
        map_ff["2017"]["stat"] = 0.06591;
        map_ff["2017"]["syst"] = 0.2855;
        map_ff["2018"]["nom"]  = 0.5042;
        map_ff["2018"]["stat"] = 0.05853;
        map_ff["2018"]["syst"] = 0.2186;
    } else if (tau_dm_[0] == 1) {
        map_ff["2016pre"]["nom"]  = 0.4542;
        map_ff["2016pre"]["stat"] = 0.07787;
        map_ff["2016pre"]["syst"] = 0.2936;
        map_ff["2016post"]["nom"]  = 0.4853;
        map_ff["2016post"]["stat"] = 0.08549;
        map_ff["2016post"]["syst"] = 0.1426;
        map_ff["2017"]["nom"]  = 0.5715;
        map_ff["2017"]["stat"] = 0.04472;
        map_ff["2017"]["syst"] = 0.2421;
        map_ff["2018"]["nom"]  = 0.5274;
        map_ff["2018"]["stat"] = 0.03891;
        map_ff["2018"]["syst"] = 0.1718;
    } else if (tau_dm_[0] == 10) {
        map_ff["2016pre"]["nom"]  = 0.5634;
        map_ff["2016pre"]["stat"] = 0.147;
        map_ff["2016pre"]["syst"] = 0.3097;
        map_ff["2016post"]["nom"]  = 0.4488;
        map_ff["2016post"]["stat"] = 0.159;
        map_ff["2016post"]["syst"] = 0.2005;
        map_ff["2017"]["nom"]  = 0.7205;
        map_ff["2017"]["stat"] = 0.08742;
        map_ff["2017"]["syst"] = 0.2134;
        map_ff["2018"]["nom"]  = 0.9003;
        map_ff["2018"]["stat"] = 0.06959;
        map_ff["2018"]["syst"] = 0.224;
    } else if (tau_dm_[0] == 11) {
        map_ff["2016pre"]["nom"]  = 0.5191;
        map_ff["2016pre"]["stat"] = 0.1276;
        map_ff["2016pre"]["syst"] = 0.2643;
        map_ff["2016post"]["nom"]  = 0.6425;
        map_ff["2016post"]["stat"] = 0.1324;
        map_ff["2016post"]["syst"] = 0.1778;
        map_ff["2017"]["nom"]  = 0.5626;
        map_ff["2017"]["stat"] = 0.07437;
        map_ff["2017"]["syst"] = 0.2383;
        map_ff["2018"]["nom"]  = 0.7733;
        map_ff["2018"]["stat"] = 0.05948;
        map_ff["2018"]["syst"] = 0.2118;
    } else {
        std::cout << "fatal: wrong decay mode detected" << std::endl;
    }

    if      (unc_ == "nom") val = map_ff[year_][unc_];
    else if (direction_ == 1) val  = 1.0 + map_ff[year_][unc_];
    else if (direction_ == -1) val = 1.0 - map_ff[year_][unc_];

    return val;
}
