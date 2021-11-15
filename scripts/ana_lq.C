#include "ROOT/RDataFrame.hxx"
#include "ROOT/RVec.hxx"
#include "ROOT/RDF/RInterface.hxx"
#include "TCanvas.h"
#include "TH1D.h"
#include "TLatex.h"
#include "TLegend.h"
#include "Math/Vector4Dfwd.h"
#include "TStyle.h"
#include <vector>

using namespace ROOT::VecOps;
using rvec_f = RVec<float>;
using rvec_b = RVec<bool>;
using rvec_i = RVec<int>;

template <typename T>
void plot(T hist, TString name){
    TCanvas * c = new TCanvas("c",Form("c_%s", name.Data()));
    hist->Write();
    hist->DrawClone();
    c->Print(Form("%s.pdf",name.Data()));
}

vector<int> goodmuons_idx(rvec_i g){
    vector<int> out;
    for(int i = 0; i < g.size(); i++){
        if( g[i] ) out.push_back( i );
    }
    return out; 
}

int bJet_idx(rvec_i goodbJet, rvec_i goodcJet, rvec_f CvsB){
    int idx = -1;
    for( int i = 0; i<goodbJet.size(); i++){
        if( goodbJet[i] && goodcJet[i] && CvsB[i] <= 0.5){
            idx = i;
            break;
        }
        else if( goodbJet[i] && !goodcJet[i]){
            idx = i;
            break;
        }
    }
    return idx;
}

int cJet_idx(rvec_i goodbJet, rvec_i goodcJet, rvec_f CvsB){
    int idx = -1;
    for( int i = 0; i<goodbJet.size(); i++){
        if( goodbJet[i] && goodcJet[i] && CvsB[i] > 0.5){
            idx = i;
            break;
        }
        else if( goodbJet[i] && !goodcJet[i]){
            idx = i;
            break;
        }
    }
    return idx;
}

int goodleading_idx(rvec_i good){
    int idx = -1;
    for( int i = 0; i < good.size(); i++){
        if(good[i]) idx = i;
        break;
    }
    return idx;
}

void ana_lq(){
    ROOT::RDataFrame df("Events",{"/xrootd/store/mc/RunIIAutumn18NanoAODv6/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8/NANOAODSIM/Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/230000/0DB0F2A1-0FBF-664A-92AD-17D54F6FF027.root",
            "/xrootd/store/mc/RunIIAutumn18NanoAODv6/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8/NANOAODSIM/Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/230000/0FD0C512-77C6-BB48-96FE-A3C58550A183.root",
            "/xrootd/store/mc/RunIIAutumn18NanoAODv6/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8/NANOAODSIM/Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/230000/133425CB-5DF4-3348-A4EC-D6236C459962.root",
            "/xrootd/store/mc/RunIIAutumn18NanoAODv6/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8/NANOAODSIM/Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/230000/30AF3FC1-3D88-8040-A05D-AF0909AFEED5.root",
            "/xrootd/store/mc/RunIIAutumn18NanoAODv6/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8/NANOAODSIM/Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/230000/3A6C0C6A-845D-7E4D-A214-3E4E4B2DEE48.root"});

    //ROOT::RDataFrame df("Events",{"/xrootd/store/data/Run2018D/SingleMuon/NANOAOD/Nano25Oct2019-v1/100000/034659D3-DA7F-9C43-B604-FADBF92587EC.root","/xrootd/store/data/Run2018D/SingleMuon/NANOAOD/Nano25Oct2019-v1/100000/10F251FC-3AFA-E148-84F2-8908B099A9AB.root"});
    //ROOT::RDataFrame df("Events","/T2_KR_KISTI/store/user/jolim/LQ_2018/LQ_2018NANO_A1/200523_183528/0000/LQ_2018_nano_1.root");
//    ROOT::RDataFrame df("Events",{"/xrootd/store/mc/RunIIAutumn18NanoAODv6/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/NANOAODSIM/Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/230000/073462AF-FDD0-AD45-970A-EB97923698F3.root","/xrootd/store/mc/RunIIAutumn18NanoAODv6/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/NANOAODSIM/Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/230000/0BFDE5A9-3D64-2A4D-A24E-FFB33CCE19F5.root"});

    cout<<df.Count().GetValue()<<endl;
    
    //int year = 2016;

    //**** HLT trigger ****//
    // 2016      :  HLT_IsoMu24
    // 2017 A-D  :  HLT_IsoMu27 or HLT_IsoMu24_eta2p1
    // 2017 E-F  :  HLT_IsoMu27
    // 2018      :  HLT_IsoMu24

    auto df_s0_HLT = df.Filter("HLT_IsoMu24","SingleMuon Trigger");

    //**** Muon Selection ( tight ) ****//
    auto df_s1_Muon = df_s0_HLT.Define("goodMuon","Muon_pt > 30 && abs(Muon_eta) < 2.4 && Muon_tightId && Muon_pfRelIso04_all < 0.15")
                               .Define("n_goodMuon","Sum(goodMuon)")
                               .Filter("n_goodMuon >= 1","Muon Selection")
                               .Define("goodMuon_leading_idx",goodleading_idx,{"goodMuon"});
    
    //**** Jet Selection ( tight ) ****//
    auto df_s2_Jet = df_s1_Muon.Define("goodJet","Jet_pt > 30 && abs(Jet_eta) < 2.4 && Jet_jetId == 6")
                               .Define("n_goodJet","Sum(goodJet)")
                               .Filter("n_goodJet >= 3","Jet Selection");

    //**** Electron selection and veto ****//
//    auto df_s3_Electron = df_s2_Jet.Define("vetoElectron","Electron_pt > 10 && abs(Electron_eta) < 2.5 && Electron_pfRelIso03_all < 0.10 && Electron_mvaFall17V2noIso_WP90")
//                                   .Define("n_vetoElectron","Sum(vetoElectron)")
//                                   .Filter("n_vetoElectron == 0","Electron Veto");
    
    //**** b Tagged Jet selection ( medium )****//
    auto df_s4_bJet = df_s2_Jet.Define("goodbJet","goodJet && Jet_btagDeepFlavB > 0.277")
                               .Define("n_goodbJet","Sum(goodbJet)")
                               .Filter("n_goodbJet == 1","bJet Selection");

    //**** b Tagged Jet selection ( medium )****//
    auto df_s5_cJet = df_s4_bJet.Define("CvsL","Jet_btagDeepFlavC/(1-Jet_btagDeepFlavB)")
                                .Define("CvsB","Jet_btagDeepFlavC/(Jet_btagDeepFlavB+Jet_btagDeepFlavC)")
                                .Define("goodcJet","goodJet && CvsL > 0.085 && CvsB > 0.29")
                                .Define("n_goodcJet","Sum(goodcJet)")
                                .Filter("n_goodcJet >= 1","cJet Selection");

//    // We need to select one b jet and one c jet
//    auto df_s6_bcJet = df_s5_cJet.Define("bJet_idx",bJet_idx,{"goodbJet","goodcJet","CvsB"})
//                                 .Define("cJet_idx",cJet_idx,{"goodbJet","goodcJet","CvsB"})
//                                 .Filter("(bJet_idx != cJet_idx) && (bJet_idx != -1) && (cJet_idx != -1)","Number of b, c Jets Selection");

//    auto disp = df_s5_cJet.Display({"CvsB","CvsL","Jet_btagDeepFlavB","Jet_pt"},1000);
//    disp->Print();
    
    //**** Tau Selection ( VSe: vvloose, VSjet: medium, VSmu: loose ) ****//
    auto df_s2_Tau = df_s1_Muon.Define("goodTau","Tau_pt > 30 && abs(Tau_eta) < 2.3 && Tau_idDecayModeNewDMs && (Tau_decayMode == 0 || Tau_decayMode == 1 || Tau_decayMode == 10 || Tau_decayMode == 11)");
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 8; j++){
            for(int k = 0; k < 8; k++){
                TString id_mu = TString::Itoa(pow(2,i),10);
                TString id_jet = TString::Itoa(pow(2,j),10);
                TString id_e = TString::Itoa(pow(2,k),10);
                int n_sig = df_s2_Tau.Define("Tauid_mu",Form("Tau_idDeepTau2017v2p1VSmu & %s",id_mu.Data()))
                                     .Define("Tauid_jet",Form("Tau_idDeepTau2017v2p1VSjet & %s",id_jet.Data()))
                                     .Define("Tauid_e",Form("Tau_idDeepTau2017v2p1VSe & %s",id_e.Data()))
                                     .Define("goodtau_condition","goodTau && Tauid_mu && Tauid_jet && Tauid_e")
                                     .Filter("Sum(goodtau_condition) >= 1","Tau selection")
                                     .Count().GetValue();
                cout<<i<<" "<<j<<" "<<k<<" "<<n_sig<<endl;
            }
        }
    }
    //histograms 
//    auto h_muon_pt = df_s5_cJet.Define("Muon1_pt","Muon_pt[Muon1_idx]").Histo1D({"h_muon_pt", "h_muon_pt", 40, 0, 200}, "Muon1_pt");
//    auto h_muon_eta = df_s5_cJet.Define("Muon1_eta","Muon_eta[Muon1_idx]").Histo1D({"h_muon_eta", "h_muon_eta", 50, -5, 5}, "Muon1_eta");
//    
//    auto h_tau_pt = df_s5_cJet.Define("Tau1_pt","Tau_pt[Tau1_idx]").Histo1D({"h_tau_pt", "h_tau_pt", 40, 0, 200}, "Tau1_pt");
//    auto h_tau_eta = df_s5_cJet.Define("Tau1_eta","Tau_eta[Tau1_idx]").Histo1D({"h_tau_eta", "h_tau_eta", 50, -5, 5}, "Tau1_eta");
//    
//    auto h_jet_pt = df_s5_cJet.Define("goodJet_pt","Jet_pt[goodJet]").Histo1D({"h_jet_pt", "h_jet_pt", 40, 0, 200}, "goodJet_pt");
//    auto h_jet_eta = df_s5_cJet.Define("goodJet_eta","Jet_eta[goodJet]").Histo1D({"h_jet_eta", "h_jet_eta", 50, -5, 5}, "goodJet_eta");
//
//    auto h_bjet_pt = df_s5_cJet.Define("bJet_pt","Jet_pt[goodbJet]").Histo1D({"h_bjet_pt", "h_bjet_pt", 40, 0, 200}, "bJet_pt");
//    auto h_bjet_eta = df_s5_cJet.Define("bJet_eta","Jet_eta[goodbJet]").Histo1D({"h_bjet_eta", "h_bjet_eta", 50, -5, 5}, "bJet_eta");
//
//    auto h_cjet_pt = df_s5_cJet.Define("cJet_pt","Jet_pt[goodcJet]").Histo1D({"h_cjet_pt", "h_cjet_pt", 40, 0, 200}, "cJet_pt");
//    auto h_cjet_eta = df_s5_cJet.Define("cJet_eta","Jet_eta[goodcJet]").Histo1D({"h_cjet_eta", "h_cjet_eta", 50, -5, 5}, "cJet_eta");
//    
//    auto h_nMuon = df_s5_cJet.Histo1D({"h_nMuon", "h_nMuon", 5, 0, 5}, "n_goodMuon");
//    auto h_nJet = df_s5_cJet.Histo1D({"h_nJet", "h_nJet", 10, 0, 10}, "n_goodJet");
//    auto h_nbJet = df_s5_cJet.Histo1D({"h_nbJet", "h_nbJet", 5, 0, 5}, "n_goodbJet");
//    auto h_ncJet = df_s5_cJet.Histo1D({"h_ncJet", "h_ncJet", 5, 0, 5}, "n_goodcJet");
    
    //df_S1_goodmuon.Snapshot("tree", "f.root");
//    TFile f("out.root", "recreate");
//    
//    plot( h_muon_pt, "h_muon_pt");
//    plot( h_muon_eta, "h_muon_eta");
//    plot( h_tau_pt, "h_tau_pt");
//    plot( h_tau_eta, "h_tau_eta");
//    plot( h_jet_pt, "h_jet_pt");
//    plot( h_jet_eta, "h_jet_eta");
//    plot( h_bjet_pt, "h_bjet_pt");
//    plot( h_bjet_eta, "h_bjet_eta");
//    plot( h_cjet_pt, "h_cjet_pt");
//    plot( h_cjet_eta, "h_cjet_eta");
//    plot( h_nMuon, "h_nMuon");
//    plot( h_nJet, "h_nJet");
//    plot( h_nbJet, "h_nbJet");
//    plot( h_ncJet, "h_ncJet");
//
//    f.Close();

//    auto report = df_s5_cJet.Report();
//    report->Print();

}
