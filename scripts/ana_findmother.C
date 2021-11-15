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
using rvec_uc = RVec<unsigned char>;
using FourVector = ROOT::Math::PtEtaPhiMVector;
using FourVectorVec = std::vector<FourVector>;

template <typename T>
void plot(T hist, TString name){
  TCanvas * c = new TCanvas("c",Form("c_%s", name.Data()));
  hist->Write();
  hist->DrawClone();
  //c->Print(Form("hist_sig/%s.pdf",name.Data()));
  c->Print(Form("hist/%s.pdf",name.Data()));
  //c->Print(Form("%s.pdf",name.Data()));
}

rvec_i good_idx(rvec_i g){
  rvec_i out;
  for(int i = 0; i < g.size(); i++){
    if( g[i] ) out.push_back( i );
  }
  return out; 
}


FourVectorVec gen4vec(rvec_f &pt, rvec_f &eta, rvec_f &phi, rvec_f &mass)
{
    const int nsize = pt.size();
    FourVectorVec fourvecs;
    fourvecs.reserve(nsize);
    for (auto i=0; i<nsize; i++)
    {
        fourvecs.emplace_back(pt[i], eta[i], phi[i], fabs(mass[i]));
    }
    return fourvecs;
}


int goodtaujet_idx(rvec_i good, int goodmu, rvec_f mu_eta, rvec_f mu_phi, rvec_f jet_eta, rvec_f jet_phi){
  int idx = -1;
  for(int i = 0; i < good.size(); i++){
    float dR = DeltaR(mu_eta[goodmu],jet_eta[i],mu_phi[goodmu],jet_phi[i]);
    if(good[i] && dR > 0.5){
      idx = i;
      break;
    }
  }
  return idx;
}

rvec_i matching_genpartcjet(rvec_i goodjets, rvec_i jet_genjetidx, rvec_f genpart_eta, rvec_f genpart_phi, rvec_f genjet_eta, rvec_f genjet_phi,
                            rvec_i genpart_pdgid){
  rvec_i matched_genpartcjet_idx;
  for (int i = 0; i < goodjets.size(); i++){
    if (goodjets[i]){
      int matched_genjet = jet_genjetidx[i];
      float mindr_genpart = 999;
      int matched_genpartidx = -1;
      for (int j = 0; j < genpart_eta.size(); j++){
        float tmp_dr = DeltaR(genpart_eta[j],genjet_eta[i],genpart_phi[j],genjet_phi[i]);
        if (tmp_dr < 0.4 && ( (abs(genpart_pdgid[j])/100)%10 == 4 || (abs(genpart_pdgid[j])/1000)%10 == 4 || abs(genpart_pdgid[j]) == 4) ){
          matched_genpartidx = j;
          break;
        }
        else matched_genpartidx = -1;
      }
      matched_genpartcjet_idx.emplace_back(matched_genpartidx);
    }
    else matched_genpartcjet_idx.emplace_back(-1);
  }
  return matched_genpartcjet_idx;
}

rvec_i isgenC( rvec_i genpart_cjet ){
  rvec_i out;
  for (int i = 0; i < genpart_cjet.size(); i++){
    if (genpart_cjet[i] >= 0) out.emplace_back(1);
    else out.emplace_back(0);
  }
  return out;
}

rvec_i genmother_pdgid(rvec_i genpart_idx, rvec_i gencjets, rvec_i genpart_genpartidxmother, rvec_i genpart_pdgid){
    rvec_i out;
    for ( int i = 0; i < gencjets.size(); i++){
        int part_idx=-1;
        int mother_idx=-1;
        int part_pdgid=999;
        int mother_pdgid=999;
        int j = -1;
        if (gencjets[i]){
            //cout<<"##############"<<endl;
            while( true ){
              j++;
              if(j==0) part_idx = genpart_idx[i];
              else part_idx = mother_idx;
              mother_idx = genpart_genpartidxmother[part_idx];
              part_pdgid = genpart_pdgid[part_idx];
              mother_pdgid = genpart_pdgid[mother_idx];
              //if(abs(part_pdgid) != 4 && abs(mother_pdgid) ==6 ) mother_pdgid = -1;
              //cout<<j<<"   "<<part_pdgid<<"  "<<mother_pdgid<<endl;
              if(mother_pdgid == 0 || abs(mother_pdgid) == 6 || abs(mother_pdgid) == 24) break;
              //if(part_pdgid == 21 && abs(mother_pdgid)!=6 ) break;
            }
            if (abs(part_pdgid) == 4 && abs(mother_pdgid) == 6){
                //cout<<"I am c-jet from Top"<<endl;
                out.emplace_back(0);
            }
            else if (abs(part_pdgid) == 5 && abs(mother_pdgid) == 6){
                //cout<<"I am b-jet from Top"<<endl;
                out.emplace_back(1);
            }
            else if (abs(mother_pdgid) == 24){
                //cout<<"I am from W"<<endl;
                out.emplace_back(2);
            }
            else if (mother_pdgid == 21){
                //cout<<"I am from Gluon"<<endl;
                out.emplace_back(3);
            }
            else {
                //cout<<"Where am I from?"<<endl;
                out.emplace_back(3);
            }
        }
        else out.emplace_back(3);
    }
    //cout<<gencjets.size()<<"    "<<out.size()<<endl;
    return out;
}

rvec_i genhadron_cjet( rvec_uc genjet_hadronflav, rvec_i jet_hadronflav, rvec_i jet_genjetidx, rvec_i genpart_genpartidxmother, FourVectorVec genjet4vec, FourVectorVec genpart4vec ){
    rvec_i out;
    //cout<<"Enter Genhadron_cjet"<<endl;
    for (int i = 0; i < jet_genjetidx.size(); i++){
        int genjetidx = jet_genjetidx[i];
        int genhadronflav = genjet_hadronflav[genjetidx];
        int jethadronflav = jet_hadronflav[i];
        if (genhadronflav & 4 && jethadronflav == 4){
            out.emplace_back(1);
            //cout<<"Gen "<< genhadronflav<<" Reco "<<jethadronflav<<endl;
        }
            /*
            for (int j = 0; j < genpartidxmother.size(); j++){
                float tmp_dr = DeltaR(genpart4vec[j].Eta(), genjet4vec[genjet_idx].Eta(), genpart4vec[j].Phi(), genjet4vec[genjet_idx].Phi());

                if ( tmp_dr < 0.4 ){
                    int j=-1;
                    int part_idx=-1;
                    int mother_idx=-1;
                    int part_pdgid=999;
                    int mother_pdgid=999;
                    while( true ){
                        j++;
                        if(j==0) part_idx = genpart_idx[i];
                        else part_idx = mother_idx;
                        mother_idx = genpart_genpartidxmother[part_idx];
                        part_pdgid = genpart_pdgid[part_idx];
                        mother_pdgid = genpart_pdgid[mother_idx];
                        //if(abs(part_pdgid) != 4 && abs(mother_pdgid) ==6 ) mother_pdgid = -1;
                        cout<<j<<"   "<<part_pdgid<<"  "<<mother_pdgid<<endl;
                        if(mother_pdgid == 0 || abs(mother_pdgid) == 6 || abs(mother_pdgid) == 24) break;
                        if(part_pdgid == 21 && abs(mother_pdgid)!=6 ) break;
                    }
                    if (abs(part_pdgid) == 4 && abs(mother_pdgid) == 6){
                        cout<<"I am c-jet from Top"<<endl;
                        out.emplace_back(1);
                    }
                    else if (abs(part_pdgid) == 5 && abs(mother_pdgid) == 6){
                        cout<<"I am b-jet from Top"<<endl;
                        out.emplace_back(2);
                    }
                    else if (abs(mother_pdgid) == 24){
                        cout<<"I am from W"<<endl;
                        out.emplace_back(3);
                    }
                    else if (mother_pdgid == 21){
                        cout<<"I am from Gluon"<<endl;
                        out.emplace_back(4);
                    }
                    else {
                        cout<<"Where am I from?"<<endl;
                        out.emplace_back(0);
                    }
                }
            }*/
        else out.emplace_back(0);
    }
    return out;

}

rvec_i FindcharmOrigin( rvec_i genpart_pdgid, rvec_i genpart_genpartidxmother){
    rvec_i out;
    cout<<"#################"<<endl;
    for ( int i = 0; i < genpart_pdgid.size(); i++){
        int dau_pdgid = genpart_pdgid[i];
        int mom_idx = genpart_genpartidxmother[i];
        int mom_pdgid = genpart_pdgid[mom_idx];
        int grandmom_idx = genpart_genpartidxmother[mom_idx];
        int grandmom_pdgid = genpart_pdgid[grandmom_idx];
        int category = 4;
        //if ( abs(dau_pdgid) == 4) cout<<"Daughter : "<<dau_pdgid<<" Mother : "<<mom_pdgid<<" Grandmother : "<<grandmom_pdgid<<endl;

        if ( abs(dau_pdgid) == 4 && abs(mom_pdgid) == 6 ) category = 0;
        else if ( abs(dau_pdgid) == 4 && abs(mom_pdgid) == 24 ) category = 1;
        else if ( abs(dau_pdgid) == 4 && abs(mom_pdgid) != 4 && abs(grandmom_pdgid) == 24 ) category = 1;
        else if ( abs(dau_pdgid) == 4 && (mom_pdgid == 21 || grandmom_pdgid == 21) ) category = 2;
        else if ( abs(dau_pdgid) == 4 && abs(mom_pdgid) != 4 ){
            category = 3;
            cout<<"Daughter : "<<dau_pdgid<<" Mother : "<<mom_pdgid<<" Grandmother : "<<grandmom_pdgid<<endl;
        }

        /*if( abs(dau_pdgid) == 4 ){
            int j = 0;
            while( true ){
                cout<<j<<" Daughter : "<<dau_pdgid<<" Mother : "<<mom_pdgid<<endl;
                if ( abs(mom_pdgid) == 6 ){
                    category = 0;
                    break;
                }
                else if ( abs(mom_pdgid) == 24 ){
                    category = 1;
                    break;
                }
                else if ( abs(mom_pdgid) == 21 ){
                    category = 2;
                    break;
                }
                else{
                    dau_pdgid = mom_pdgid;
                    mom_idx = genpart_genpartidxmother[mom_idx];
                    mom_pdgid = genpart_pdgid[mom_idx];
                }
                j++;
                if(j>10) {
                    category = 3;
                    break;
                }
            }
        }*/
        out.emplace_back(category);
    }
    return out;
}


void ana_findmother(const char *inputFile){

  ROOT::RDataFrame df("Events", inputFile);
  
  auto df_hlt = df.Filter("HLT_IsoMu24","HLT IsoMu24");
  auto df_muon = df_hlt.Filter("nMuon >= 1", "Events with one lepton")
                       .Define("goodmuons","Muon_pt > 30  && abs(Muon_eta) < 2.4 && Muon_tightId && Muon_pfRelIso04_all < 0.15")
                       .Define("goodtaus","Tau_pt > 20 && abs(Tau_eta) < 2.3 && Tau_idDecayModeNewDMs && Tau_idDeepTau2017v2p1VSmu & 1 && Tau_idDeepTau2017v2p1VSe & 1 && Tau_idDeepTau2017v2p1VSjet & 16")
                       //.Define("goodjets","Jet_pt > 20 && abs(Jet_eta) < 2.4 && Jet_jetId == 6");
                       .Define("goodjets","Jet_pt > 0");
  
  auto df_goodmuon = df_muon.Filter("Sum( goodmuons ) >=1 ","Events with at least a goood muon")
                                  .Define("goodmuons_idx",good_idx,{"goodmuons"});

  auto df_goodtau = df_goodmuon.Filter("Sum( goodtaus ) >=1 ","Events with at least a goood tau");

  //auto df_goodjet = df_goodtau.Filter("Sum( goodjets ) >=3", "Events with at least three goood jets");
  auto df_goodjet = df_goodtau.Define("goodjets_idx",good_idx,{"goodjets"})
                              .Define("CvsB","Jet_btagDeepFlavC/(Jet_btagDeepFlavC+Jet_btagDeepFlavB)")
                              .Define("CvsL","Jet_btagDeepFlavC/(1-Jet_btagDeepFlavB)")
                              .Define("goodbjets","goodjets && Jet_btagDeepFlavB > 0.2770")
                              .Define("goodcjets","goodjets && CvsB > 0.29 && CvsL > 0.085")
                              .Define("ngoodjets","int(goodjets.size())");
  
  auto df_goodbtag = df_goodjet.Filter("Sum( goodbjets ) == 1", "Events with one b tagged jets");
  auto df_goodctag = df_goodbtag.Filter("Sum( goodcjets ) >= 1", "Events with one b tagged jets");
  
  auto df_genpart = df_goodjet.Define("genpart_cjetidx",matching_genpartcjet,{"goodjets","Jet_genJetIdx","GenPart_eta","GenPart_phi","GenJet_eta","GenJet_phi","GenPart_pdgId"})
                              .Define("gencjets",isgenC,{"genpart_cjetidx"})
                              .Define("matchedcjet_pt","Jet_pt[gencjets]")
                              .Define("genmother",genmother_pdgid,{"genpart_cjetidx","gencjets","GenPart_genPartIdxMother","GenPart_pdgId"})
                              .Define("genjet4vec",gen4vec,{"GenJet_pt","GenJet_eta","GenJet_phi","GenJet_mass"})
                              .Define("genpart4vec",gen4vec,{"GenPart_pt","GenPart_eta","GenPart_phi","GenPart_mass"})
                              .Define("genhadron_cjet",genhadron_cjet,{"GenJet_hadronFlavour","Jet_hadronFlavour","Jet_genJetIdx","GenPart_genPartIdxMother","genjet4vec","genpart4vec"})
                              .Define("charmorigin",FindcharmOrigin,{"GenPart_pdgId","GenPart_genPartIdxMother"});


  //auto disp = df_genpart.Display({"ngoodjets","genpart_idx","genpart_pdgid","gencjets","gen_matchedcjet_pt"},100);
  //disp->Print();


  //histograms 
  auto h_ntaus = df_goodjet.Define("ntaus","Sum( goodtaus)").Histo1D({"h_ntaus", "h_ntaus", 5, 0, 5}, "ntaus");
  auto h_nbjets = df_goodjet.Define("nbjets","Sum( goodbjets)").Histo1D({"h_nbjets", "h_nbjets", 7, 0, 7}, "nbjets");
  auto h_ncjets = df_goodbtag.Define("ncjets","Sum( goodcjets)").Histo1D({"h_ncjets", "h_ncjets", 7, 0, 7}, "ncjets");
  auto h_GenJet_hadronFlavour = df_genpart.Histo1D({"h_GenJet_hadronFlavour", "h_GenJet_hadronFlavour", 100, -50, 50}, "GenJet_hadronFlavour");
  auto h_GenJet_partonFlavour = df_genpart.Histo1D({"h_GenJet_partonFlavour", "h_GenJet_partonFlavour", 100, -50, 50}, "GenJet_partonFlavour");
  auto h_matchedcjet_pt = df_genpart.Define("matchedcjet","Jet_pt[genmother==0 || genmother==2]").Histo1D({"h_matchedcjet_pt", "h_matchedcjet_pt", 60, 0, 300}, "matchedcjet");
  
  auto h_top_cjet = df_genpart.Define("top_cjet","Jet_pt[genmother==0]").Histo1D({"h_top_cjet", "h_top_cjet", 60, 0, 300}, "top_cjet");
  auto h_top_bjet = df_genpart.Define("top_bjet","Jet_pt[genmother == 1]").Histo1D({"h_top_bjet", "h_top_bjet", 60, 0, 300}, "top_bjet");
  auto h_W_cjet = df_genpart.Define("W_cjet","Jet_pt[genmother == 2]").Histo1D({"h_W_cjet", "h_W_cjet", 60, 0, 300}, "W_cjet"); 
  auto h_other_cjet = df_genpart.Define("other_cjet","Jet_pt[genmother == 3]").Histo1D({"h_other_cjet", "h_other_cjet", 60, 0, 300}, "other_cjet");
  
  auto h_jet_genhadronC = df_genpart.Define("jet_genhadronC","Jet_pt[genhadron_cjet]").Histo1D({"h_jet_genhadronC", "h_jet_genhadronC", 60, 0, 300}, "jet_genhadronC");
  
  auto h_gen_top_c = df_genpart.Define("gen_top_c","GenPart_pt[charmorigin==0]").Histo1D({"h_gen_top_c", "h_gen_top_c", 60, 0, 300}, "gen_top_c");
  auto h_gen_W_c = df_genpart.Define("gen_W_c","GenPart_pt[charmorigin==1]").Histo1D({"h_gen_W_c", "h_gen_W_c", 60, 0, 300}, "gen_W_c");
  auto h_gen_gluon_c = df_genpart.Define("gen_gluon_c","GenPart_pt[charmorigin==2 || GenPart_pt > 0]").Histo1D({"h_gen_gluon_c", "h_gen_gluon_c", 60, 0, 300}, "gen_gluon_c");
  auto h_gen_other_c = df_genpart.Define("gen_other_c","GenPart_pt[charmorigin==3]").Histo1D({"h_gen_other_c", "h_gen_other_c", 60, 0, 300}, "gen_other_c");

  TFile f("out_1.root", "recreate");
  plot( h_ntaus, "h_ntaus");
  plot( h_nbjets, "h_nbjets");
  plot( h_ncjets, "h_ncjets");
  plot( h_GenJet_hadronFlavour, "h_GenJet_hadronFlavour");
  plot( h_GenJet_partonFlavour, "h_GenJet_partonFlavour");
  plot( h_matchedcjet_pt, "h_matchedcjet_pt");
  plot( h_top_cjet, "h_top_cjet");
  plot( h_top_bjet, "h_top_bjet");
  plot( h_W_cjet, "h_W_cjet");
  plot( h_other_cjet, "h_gen_other_cjet");
  plot( h_jet_genhadronC, "h_jet_genhadronC");
  plot( h_gen_top_c, "h_gen_top_c");
  plot( h_gen_W_c, "h_gen_W_c");
  plot( h_gen_gluon_c, "h_gen_gluon_c");
  plot( h_gen_other_c, "h_gen_other_c");
  f.Close();

  //auto report = df_tagged_jets.Report();
  auto report = df_genpart.Report();
  report->Print();

}
