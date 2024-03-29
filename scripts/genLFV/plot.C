#include "tdrstyle.C"

void make6filePlot(TFile * f1, TFile * f2,TFile * f3,TFile * f4,TFile * f5, TFile * f6, TString name, TString ytitle, TString xtitle, float x1, float y1, float x2, float y2){

  TH1F * h1 = (TH1F*) f1->Get(Form("%s", name.Data()));
  TH1F * h2 = (TH1F*) f2->Get(Form("%s", name.Data()));
  TH1F * h3 = (TH1F*) f3->Get(Form("%s", name.Data()));
  TH1F * h4 = (TH1F*) f4->Get(Form("%s", name.Data()));
  TH1F * h5 = (TH1F*) f5->Get(Form("%s", name.Data()));
  TH1F * h6 = (TH1F*) f6->Get(Form("%s", name.Data()));

  cout<<h1<<endl;
  cout<<h2<<endl;
  cout<<h3<<endl;
  cout<<h4<<endl;
  cout<<h5<<endl;
  cout<<h6<<endl;
  TCanvas * c = new TCanvas(Form("c_%s",name.Data()),"c",1);
  h1->Sumw2();
  h2->Sumw2();
  h3->Sumw2();
  h4->Sumw2();
  h5->Sumw2();
  h6->Sumw2();
  h1->Scale(1.0/h1->Integral());
  h2->Scale(1.0/h2->Integral());
  h3->Scale(1.0/h3->Integral());
  h4->Scale(1.0/h4->Integral());
  h5->Scale(1.0/h5->Integral());
  h6->Scale(1.0/h6->Integral());
  h1->SetLineWidth(2);
  h2->SetLineWidth(2);
  h3->SetLineWidth(2);
  h4->SetLineWidth(2);
  h5->SetLineWidth(2);
  h6->SetLineWidth(2);
  
  h1->SetLineColor(kCyan);
  h2->SetLineColor(kCyan+1);
  h3->SetLineColor(kCyan+2);
  h4->SetLineColor(kCyan+3);
  h5->SetLineColor(kGreen+1);
  h6->SetLineColor(kRed+1);
  
  h1->SetStats(0);
  h1->SetTitle("");
  double max_h1 = h1->GetMaximum();
  double max_h2 = h2->GetMaximum();
  double max_h3 = h3->GetMaximum();
  double max_h4 = h4->GetMaximum();
  double max_h5 = h5->GetMaximum();
  double max_h6 = h6->GetMaximum();
  double max = std::max(std::max(std::max(std::max(std::max(max_h1,max_h2),max_h3),max_h4),max_h5),max_h6);
  h1->SetMaximum(max*1.2);
  h1->Draw("hist");
  h2->Draw("histsame");
  h3->Draw("histsame");
  h4->Draw("histsame");
  h5->Draw("histsame");
  h6->Draw("histsame");
  h1->GetXaxis()->SetTitle(Form("%s",xtitle.Data()));
  h1->GetXaxis()->SetTitleSize(0.05);
  h1->GetYaxis()->SetTitle(Form("%s",ytitle.Data()));
  h1->GetYaxis()->SetTitleSize(0.05);

  TLegend * l = new TLegend(x1,y1,x2,y2);
  l->AddEntry(h1,"Clq","L");
  l->AddEntry(h2,"Clu","L");
  l->AddEntry(h3,"Cqe","L");
  l->AddEntry(h4,"Ceu","L");
  l->AddEntry(h5,"Clequ1","L");
  l->AddEntry(h6,"Clequ3","L");
  l->SetTextSize(0.04);
  l->SetFillColor(0);
  l->SetLineColor(0);
  l->Draw();

  float xmin = h6->GetXaxis()->GetXmin();
  float xmax = h6->GetXaxis()->GetXmax();
  float xrange = xmax - xmin;
  
  TLatex latex;
  latex.SetTextAlign(11); 
  latex.SetTextSize(0.04);
  latex.DrawLatex(xmin + xrange * 0.05, max*1.2*0.9, "CMS Simulation");

  c->Print(Form("plots/tmp/%s.pdf",name.Data()));

}
void make7filePlot(TFile * f1, TFile * f2,TFile * f3,TFile * f4,TFile * f5, TFile * f6, TFile * f7, TString name, TString ytitle, TString xtitle, float x1, float y1, float x2, float y2){

  TH1F * h1 = (TH1F*) f1->Get(Form("%s", name.Data()));
  TH1F * h2 = (TH1F*) f2->Get(Form("%s", name.Data()));
  TH1F * h3 = (TH1F*) f3->Get(Form("%s", name.Data()));
  TH1F * h4 = (TH1F*) f4->Get(Form("%s", name.Data()));
  TH1F * h5 = (TH1F*) f5->Get(Form("%s", name.Data()));
  TH1F * h6 = (TH1F*) f6->Get(Form("%s", name.Data()));
  TH1F * h7 = (TH1F*) f7->Get(Form("%s", name.Data()));

  cout<<h1<<endl;
  cout<<h2<<endl;
  cout<<h3<<endl;
  cout<<h4<<endl;
  cout<<h5<<endl;
  cout<<h6<<endl;
  cout<<h7<<endl;
  
  TCanvas * c = new TCanvas(Form("c_%s",name.Data()),"c",1);
   
  h1->Sumw2();
  h2->Sumw2();
  h3->Sumw2();
  h4->Sumw2();
  h5->Sumw2();
  h6->Sumw2();
  h7->Sumw2();
  
  h1->Scale(1.0/h1->Integral());
  h2->Scale(1.0/h2->Integral());
  h3->Scale(1.0/h3->Integral());
  h4->Scale(1.0/h4->Integral());
  h5->Scale(1.0/h5->Integral());
  h6->Scale(1.0/h6->Integral());
  h7->Scale(1.0/h7->Integral());
  
  h1->SetLineWidth(2);
  h2->SetLineWidth(2);
  h3->SetLineWidth(2);
  h4->SetLineWidth(2);
  h5->SetLineWidth(2);
  h6->SetLineWidth(2);
  h7->SetLineWidth(2);
  
  h1->SetLineColor(kCyan);
  h2->SetLineColor(kCyan+1);
  h3->SetLineColor(kCyan+2);
  h4->SetLineColor(kCyan+3);
  h5->SetLineColor(kGreen+1);
  h6->SetLineColor(kRed+1);
  h7->SetLineColor(kBlack);
  
  h1->SetStats(0);
  h1->SetTitle("");
  
  double max_h1 = h1->GetMaximum();
  double max_h2 = h2->GetMaximum();
  double max_h3 = h3->GetMaximum();
  double max_h4 = h4->GetMaximum();
  double max_h5 = h5->GetMaximum();
  double max_h6 = h6->GetMaximum();
  double max_h7 = h7->GetMaximum();
  double max = std::max(std::max(std::max(std::max(std::max(std::max(max_h1,max_h2),max_h3),max_h4),max_h5),max_h6),max_h7);
  
  h7->SetMaximum(max*1.2);
  h7->Draw("hist");
  h1->Draw("histsame");
  h2->Draw("histsame");
  h3->Draw("histsame");
  h4->Draw("histsame");
  h5->Draw("histsame");
  h6->Draw("histsame");
  h7->GetXaxis()->SetTitle(Form("%s",xtitle.Data()));
  h7->GetXaxis()->SetTitleSize(0.05);
  h7->GetYaxis()->SetTitle(Form("%s",ytitle.Data()));
  h7->GetYaxis()->SetTitleSize(0.05); 

  TLegend * l = new TLegend(x1,y1,x2,y2);
  l->AddEntry(h1,"Clq","L");
  l->AddEntry(h2,"Clu","L");
  l->AddEntry(h3,"Cqe","L");
  l->AddEntry(h4,"Ceu","L");
  l->AddEntry(h5,"Clequ1","L");
  l->AddEntry(h6,"Clequ3","L");
  l->AddEntry(h7,"SM ttbar","L");
  l->SetTextSize(0.04);
  l->SetFillColor(0);
  l->SetLineColor(0);
  l->Draw();

  float xmin = h7->GetXaxis()->GetXmin();
  float xmax = h7->GetXaxis()->GetXmax();
  float xrange = xmax - xmin;
  
  TLatex latex;
  latex.SetTextAlign(11); 
  latex.SetTextSize(0.04);
  latex.DrawLatex(xmin + xrange * 0.05, max*1.2*0.9, "CMS Simulation");

  c->Print(Form("plots/tmp/%s.pdf",name.Data()));

}

void make3LFVfilePlot(TFile * f1, TFile * f2,TFile * f3, TString name, TString fname, TString ytitle, TString xtitle, float x1, float y1, float x2, float y2){

  TH1D * h1 = (TH1D*) f1->Get(Form("%s", name.Data()));
  TH1D * h2 = (TH1D*) f2->Get(Form("%s", name.Data()));
  TH1D * h3 = (TH1D*) f3->Get(Form("%s", name.Data()));

  cout<<h1<<endl;
  cout<<h2<<endl;
  cout<<h3<<endl;
  
  TCanvas * c = new TCanvas(Form("c_%s",name.Data()),"c",1);
   
  h1->Sumw2();
  h2->Sumw2();
  h3->Sumw2();
  h1->Scale(1.0/h1->Integral());
  h2->Scale(1.0/h2->Integral());
  h3->Scale(1.0/h3->Integral());
  h1->SetLineWidth(2);
  h2->SetLineWidth(2);
  h3->SetLineWidth(2);
  h1->SetLineColor(kGreen+1);
  h2->SetLineColor(kCyan+2);
  h3->SetLineColor(kRed+1);
  h1->SetStats(0);
  h1->SetTitle("");
  double max_h1 = h1->GetMaximum();
  double max_h2 = h2->GetMaximum();
  double max_h3 = h3->GetMaximum();
  double max = std::max(std::max(max_h1,max_h2),max_h3);
  //double max = std::max(max_h1,max_h2);
  h1->SetMaximum(max*1.2);
//  h1->Draw("E");
  h1->Draw("HIST");
  h2->Draw("SameHIST");
  h3->Draw("SameHIST");
  h1->GetXaxis()->SetTitle(Form("%s",xtitle.Data()));
  h1->GetXaxis()->SetTitleSize(0.05);
  h1->GetYaxis()->SetTitle(Form("%s",ytitle.Data()));
  h1->GetYaxis()->SetTitleSize(0.05);
  

  TLegend * l = new TLegend(x1,y1,x2,y2);
  l->AddEntry(h1,"Scalar","L");
  l->AddEntry(h2,"Vector","L");
  l->AddEntry(h3,"Tensor","L");
  l->SetTextSize(0.045);
  l->SetFillColor(0);
  l->SetLineColor(0);
  l->Draw();

  float xmin = h1->GetXaxis()->GetXmin();
  float xmax = h1->GetXaxis()->GetXmax();
  float xrange = xmax - xmin;
  
  TLatex latex;
  latex.SetTextAlign(11); 
  latex.SetTextSize(0.04);
  latex.DrawLatex(xmin + xrange * 0.05, max*1.2*0.9, "CMS Simulation");

  c->Print(Form("plots/tmp/%s_%s.pdf",name.Data(),fname.Data()));

}
void make5LFVfilePlot(TFile * f, TFile * f1, TFile * f2,TFile * f3,TFile * f4, TString kind, TString name, TString ytitle, TString xtitle, float x1, float y1, float x2, float y2){

  TH1D * h = (TH1D*) f->Get(Form("%s", name.Data()));
  TH1D * h1 = (TH1D*) f1->Get(Form("%s", name.Data()));
  TH1D * h2 = (TH1D*) f2->Get(Form("%s", name.Data()));
  TH1D * h3 = (TH1D*) f3->Get(Form("%s", name.Data()));
  TH1D * h4 = (TH1D*) f4->Get(Form("%s", name.Data()));

  cout<<h<<endl;
  cout<<h1<<endl;
  cout<<h2<<endl;
  cout<<h3<<endl;
  cout<<h4<<endl;
  
  TCanvas * c = new TCanvas(Form("c_%s",name.Data()),"c",1);
   
  h->Sumw2();
  h1->Sumw2();
  h2->Sumw2();
  h3->Sumw2();
  h4->Sumw2();
  h->Scale(1.0/h->Integral());
  h1->Scale(1.0/h1->Integral());
  h2->Scale(1.0/h2->Integral());
  h3->Scale(1.0/h3->Integral());
  h4->Scale(1.0/h4->Integral());
  h->SetLineWidth(2);
  h1->SetLineWidth(2);
  h2->SetLineWidth(2);
  h3->SetLineWidth(2);
  h4->SetLineWidth(2);
  h->SetLineColor(kBlack);
  h1->SetLineColor(kGreen+1);
  h2->SetLineColor(kCyan+2);
  h3->SetLineColor(kRed+1);
  h4->SetLineColor(kMagenta+1);
  h->SetStats(0);
  h->SetTitle("");
  double max_h = h->GetMaximum();
  double max_h1 = h1->GetMaximum();
  double max_h2 = h2->GetMaximum();
  double max_h3 = h3->GetMaximum();
  double max_h4 = h4->GetMaximum();
  double max = std::max(std::max(std::max(std::max(max_h,max_h1),max_h2),max_h3),max_h4);
  //double max = std::max(max_h1,max_h2);
  h->SetMaximum(max*1.2);
//  h1->Draw("E");
  h->Draw("HIST");
  h1->Draw("SameHIST");
  h2->Draw("SameHIST");
  h3->Draw("SameHIST");
  h4->Draw("SameHIST");
  h->GetXaxis()->SetTitle(Form("%s",xtitle.Data()));
  h->GetXaxis()->SetTitleSize(0.05);
  h->GetYaxis()->SetTitle(Form("%s",ytitle.Data()));
  h->GetYaxis()->SetTitleSize(0.05);
  

  TLegend * l = new TLegend(x1,y1,x2,y2);
  l->AddEntry(h,"SM #it{t#bar{t}}","L");
  l->AddEntry(h1,"ST #it{tc#mu#tau}","L");
  l->AddEntry(h2,"ST #it{tu#mu#tau}","L");
  l->AddEntry(h3,"TT #it{tc#mu#tau}","L");
  l->AddEntry(h4,"TT #it{tu#mu#tau}","L");
  l->SetTextSize(0.045);
  l->SetFillColor(0);
  l->SetLineColor(0);
  l->Draw();

  float xmin = h->GetXaxis()->GetXmin();
  float xmax = h->GetXaxis()->GetXmax();
  float xrange = xmax - xmin;
  
  TLatex latex;
  latex.SetTextAlign(11); 
  latex.SetTextSize(0.04);
  latex.DrawLatex(xmin + xrange * 0.05, max*1.2*0.9, "CMS Simulation");

  c->Print(Form("plots/tmp/%s_%s.pdf",name.Data(),kind.Data()));

}
void plot(){

  setTDRStyle();

  TFile * f = new TFile("rootfiles/SM_ttbar.root");
  TFile * fops = new TFile("rootfiles/SM_ttbar_ops.root");

  TFile * f11 = new TFile("rootfiles/LFV_ST_TCMuTau_Scalar.root");
  TFile * f12 = new TFile("rootfiles/LFV_ST_TUMuTau_Scalar.root");
  TFile * f13 = new TFile("rootfiles/LFV_TT_TToCMuTau_Scalar.root");
  TFile * f14 = new TFile("rootfiles/LFV_TT_TToUMuTau_Scalar.root");

  TFile * f21 = new TFile("rootfiles/LFV_ST_TCMuTau_Vector.root");
  TFile * f22 = new TFile("rootfiles/LFV_ST_TUMuTau_Vector.root");
  TFile * f23 = new TFile("rootfiles/LFV_TT_TToCMuTau_Vector.root");
  TFile * f24 = new TFile("rootfiles/LFV_TT_TToUMuTau_Vector.root");

  TFile * f31 = new TFile("rootfiles/LFV_ST_TCMuTau_Tensor.root");
  TFile * f32 = new TFile("rootfiles/LFV_ST_TUMuTau_Tensor.root");
  TFile * f33 = new TFile("rootfiles/LFV_TT_TToCMuTau_Tensor.root");
  TFile * f34 = new TFile("rootfiles/LFV_TT_TToUMuTau_Tensor.root");

  TFile * f1 = new TFile("rootfiles/LFV_TT_to_cmutau_Clq.root");
  TFile * f2 = new TFile("rootfiles/LFV_TT_to_cmutau_Clu.root");
  TFile * f3 = new TFile("rootfiles/LFV_TT_to_cmutau_Cqe.root");
  TFile * f4 = new TFile("rootfiles/LFV_TT_to_cmutau_Ceu.root");
  TFile * f5 = new TFile("rootfiles/LFV_TT_to_cmutau_Clequ1.root");
  TFile * f6 = new TFile("rootfiles/LFV_TT_to_cmutau_Clequ3.root");

  make5LFVfilePlot(f,f11,f12,f13,f14,"Scalar","h_bpt","NormalizedEntries","#it{p_{T,b}}",0.70,0.65,0.85,0.9);
  make5LFVfilePlot(f,f11,f12,f13,f14,"Scalar","h_mupt","NormalizedEntries","#it{p_{T,#mu}}",0.70,0.65,0.85,0.9);
  make5LFVfilePlot(f,f11,f12,f13,f14,"Scalar","h_taupt","NormalizedEntries","#it{p_{T,#tau}}",0.70,0.65,0.85,0.9);
  make5LFVfilePlot(f,f11,f12,f13,f14,"Scalar","h_beta","NormalizedEntries","#it{#eta_{b}}",0.70,0.65,0.85,0.9);
  make5LFVfilePlot(f,f11,f12,f13,f14,"Scalar","h_mueta","NormalizedEntries","#it{#eta_{#mu}}",0.70,0.65,0.85,0.9);
  make5LFVfilePlot(f,f11,f12,f13,f14,"Scalar","h_taueta","NormalizedEntries","#it{#eta_{#tau}}",0.70,0.65,0.85,0.9);
  make5LFVfilePlot(f,f11,f12,f13,f14,"Scalar","h_lepdR","NormalizedEntries","#it{#DeltaR_{#mu,#tau}}",0.70,0.65,0.85,0.9);
  make5LFVfilePlot(f,f11,f12,f13,f14,"Scalar","h_lepmass","NormalizedEntries","#it{M_{#mu,#tau}}",0.70,0.65,0.85,0.9);
  make5LFVfilePlot(f,f21,f22,f23,f24,"Vector","h_bpt","NormalizedEntries","#it{p_{T,b}}",0.70,0.65,0.85,0.9);
  make5LFVfilePlot(f,f21,f22,f23,f24,"Vector","h_mupt","NormalizedEntries","#it{p_{T,#mu}}",0.70,0.65,0.85,0.9);
  make5LFVfilePlot(f,f21,f22,f23,f24,"Vector","h_taupt","NormalizedEntries","#it{p_{T,#tau}}",0.70,0.65,0.85,0.9);
  make5LFVfilePlot(f,f21,f22,f23,f24,"Vector","h_beta","NormalizedEntries","#it{#eta_{b}}",0.70,0.65,0.85,0.9);
  make5LFVfilePlot(f,f21,f22,f23,f24,"Vector","h_mueta","NormalizedEntries","#it{#eta_{#mu}}",0.70,0.65,0.85,0.9);
  make5LFVfilePlot(f,f21,f22,f23,f24,"Vector","h_taueta","NormalizedEntries","#it{#eta_{#tau}}",0.70,0.65,0.85,0.9);
  make5LFVfilePlot(f,f21,f22,f23,f24,"Vector","h_lepdR","NormalizedEntries","#it{#DeltaR_{#mu,#tau}}",0.70,0.65,0.85,0.9);
  make5LFVfilePlot(f,f21,f22,f23,f24,"Vector","h_lepmass","NormalizedEntries","#it{M_{#mu,#tau}}",0.70,0.65,0.85,0.9);
  make5LFVfilePlot(f,f31,f32,f33,f34,"Tensor","h_bpt","NormalizedEntries","#it{p_{T,b}}",0.70,0.65,0.85,0.9);
  make5LFVfilePlot(f,f31,f32,f33,f34,"Tensor","h_mupt","NormalizedEntries","#it{p_{T,#mu}}",0.70,0.65,0.85,0.9);
  make5LFVfilePlot(f,f31,f32,f33,f34,"Tensor","h_taupt","NormalizedEntries","#it{p_{T,#tau}}",0.70,0.65,0.85,0.9);
  make5LFVfilePlot(f,f31,f32,f33,f34,"Tensor","h_beta","NormalizedEntries","#it{#eta_{b}}",0.70,0.65,0.85,0.9);
  make5LFVfilePlot(f,f31,f32,f33,f34,"Tensor","h_mueta","NormalizedEntries","#it{#eta_{#mu}}",0.70,0.65,0.85,0.9);
  make5LFVfilePlot(f,f31,f32,f33,f34,"Tensor","h_taueta","NormalizedEntries","#it{#eta_{#tau}}",0.70,0.65,0.85,0.9);
  make5LFVfilePlot(f,f31,f32,f33,f34,"Tensor","h_lepdR","NormalizedEntries","#it{#DeltaR_{#mu,#tau}}",0.70,0.65,0.85,0.9);
  make5LFVfilePlot(f,f31,f32,f33,f34,"Tensor","h_lepmass","NormalizedEntries","#it{M_{#mu,#tau}}",0.70,0.65,0.85,0.9);
  
  make3LFVfilePlot(f11,f21,f31,"h_lepdR","LFV_ST_TCMuTau","NormalizedEntries","#it{#DeltaR_{#mu,#tau}}",0.70,0.65,0.85,0.9);
  make3LFVfilePlot(f11,f21,f31,"h_lepmass","LFV_ST_TCMuTau","NormalizedEntries","#it{M_{#mu,#tau}}",0.70,0.65,0.85,0.9);
  make3LFVfilePlot(f11,f21,f31,"h_bpt","LFV_ST_TCMuTau","NormalizedEntries","#it{p_{T,b}}",0.70,0.65,0.85,0.9);
  make3LFVfilePlot(f11,f21,f31,"h_mupt","LFV_ST_TCMuTau","NormalizedEntries","#it{p_{T,#mu}}",0.70,0.65,0.85,0.9);
  make3LFVfilePlot(f11,f21,f31,"h_taupt","LFV_ST_TCMuTau","NormalizedEntries","#it{p_{T,#tau}}",0.70,0.65,0.85,0.9);
  make3LFVfilePlot(f11,f21,f31,"h_beta","LFV_ST_TCMuTau","NormalizedEntries","#it{#eta_{b}}",0.70,0.65,0.85,0.9);
  make3LFVfilePlot(f11,f21,f31,"h_mueta","LFV_ST_TCMuTau","NormalizedEntries","#it{#eta_{#mu}}",0.70,0.65,0.85,0.9);
  make3LFVfilePlot(f11,f21,f31,"h_taueta","LFV_ST_TCMuTau","NormalizedEntries","#it{#eta_{#tau}}",0.70,0.65,0.85,0.9);
  
  make3LFVfilePlot(f12,f22,f32,"h_lepdR","LFV_ST_TUMuTau","NormalizedEntries","#it{#DeltaR_{#mu,#tau}}",0.70,0.65,0.85,0.9);
  make3LFVfilePlot(f12,f22,f32,"h_lepmass","LFV_ST_TUMuTau","NormalizedEntries","#it{M_{#mu,#tau}}",0.70,0.65,0.85,0.9);
  make3LFVfilePlot(f12,f22,f32,"h_bpt","LFV_ST_TUMuTau","NormalizedEntries","#it{p_{T,b}}",0.70,0.65,0.85,0.9);
  make3LFVfilePlot(f12,f22,f32,"h_mupt","LFV_ST_TUMuTau","NormalizedEntries","#it{p_{T,#mu}}",0.70,0.65,0.85,0.9);
  make3LFVfilePlot(f12,f22,f32,"h_taupt","LFV_ST_TUMuTau","NormalizedEntries","#it{p_{T,#tau}}",0.70,0.65,0.85,0.9);
  make3LFVfilePlot(f12,f22,f32,"h_beta","LFV_ST_TUMuTau","NormalizedEntries","#it{#eta_{b}}",0.70,0.65,0.85,0.9);
  make3LFVfilePlot(f12,f22,f32,"h_mueta","LFV_ST_TUMuTau","NormalizedEntries","#it{#eta_{#mu}}",0.70,0.65,0.85,0.9);
  make3LFVfilePlot(f11,f21,f31,"h_taueta","LFV_ST_TUMuTau","NormalizedEntries","#it{#eta_{#tau}}",0.70,0.65,0.85,0.9);
  
  make3LFVfilePlot(f13,f23,f33,"h_lepdR","LFV_TT_TCMuTau","NormalizedEntries","#it{#DeltaR_{#mu,#tau}}",0.70,0.65,0.85,0.9);
  make3LFVfilePlot(f13,f23,f33,"h_lepmass","LFV_TT_TCMuTau","NormalizedEntries","#it{M_{#mu,#tau}}",0.70,0.65,0.85,0.9);
  make3LFVfilePlot(f13,f23,f33,"h_bpt","LFV_TT_TCMuTau","NormalizedEntries","#it{p_{T,b}}",0.70,0.65,0.85,0.9);
  make3LFVfilePlot(f13,f23,f33,"h_mupt","LFV_TT_TCMuTau","NormalizedEntries","#it{p_{T,#mu}}",0.70,0.65,0.85,0.9);
  make3LFVfilePlot(f13,f23,f33,"h_taupt","LFV_TT_TCMuTau","NormalizedEntries","#it{p_{T,#tau}}",0.70,0.65,0.85,0.9);
  make3LFVfilePlot(f13,f23,f33,"h_beta","LFV_TT_TCMuTau","NormalizedEntries","#it{#eta_{b}}",0.70,0.65,0.85,0.9);
  make3LFVfilePlot(f13,f23,f33,"h_mueta","LFV_TT_TCMuTau","NormalizedEntries","#it{#eta_{#mu}}",0.70,0.65,0.85,0.9);
  make3LFVfilePlot(f11,f21,f31,"h_taueta","LFV_TT_TCMuTau","NormalizedEntries","#it{#eta_{#tau}}",0.70,0.65,0.85,0.9);
  
  make3LFVfilePlot(f14,f24,f34,"h_lepdR","LFV_TT_TUMuTau","NormalizedEntries","#it{#DeltaR_{#mu,#tau}}",0.70,0.65,0.85,0.9);
  make3LFVfilePlot(f14,f24,f34,"h_lepmass","LFV_TT_TUMuTau","NormalizedEntries","#it{M_{#mu,#tau}}",0.70,0.65,0.85,0.9);
  make3LFVfilePlot(f14,f24,f34,"h_bpt","LFV_TT_TUMuTau","NormalizedEntries","#it{p_{T,b}}",0.70,0.65,0.85,0.9);
  make3LFVfilePlot(f14,f24,f34,"h_mupt","LFV_TT_TUMuTau","NormalizedEntries","#it{p_{T,#mu}}",0.70,0.65,0.85,0.9);
  make3LFVfilePlot(f14,f24,f34,"h_taupt","LFV_TT_TUMuTau","NormalizedEntries","#it{p_{T,#tau}}",0.70,0.65,0.85,0.9);
  make3LFVfilePlot(f14,f24,f34,"h_beta","LFV_TT_TUMuTau","NormalizedEntries","#it{#eta_{b}}",0.70,0.65,0.85,0.9);
  make3LFVfilePlot(f14,f24,f34,"h_mueta","LFV_TT_TUMuTau","NormalizedEntries","#it{#eta_{#mu}}",0.70,0.65,0.85,0.9);
  make3LFVfilePlot(f11,f21,f31,"h_taueta","LFV_TT_TUMuTau","NormalizedEntries","#it{#eta_{#tau}}",0.70,0.65,0.85,0.9);
  
  make7filePlot(f1,f2,f3,f4,f5,f6,fops,"h_lepdR","NormalizedEntries","#it{#DeltaR_{#mu,#tau}}",0.72,0.55,0.85,0.9);
  make7filePlot(f1,f2,f3,f4,f5,f6,fops,"h_lepmass","NormalizedEntries","#it{M_{#mu,#tau}}",0.72,0.55,0.85,0.9);
  make7filePlot(f1,f2,f3,f4,f5,f6,fops,"h_bpt","NormalizedEntries","#it{p_{T,b}}",0.72,0.55,0.85,0.9);
  make7filePlot(f1,f2,f3,f4,f5,f6,fops,"h_mupt","NormalizedEntries","#it{p_{T,#mu}}",0.72,0.55,0.85,0.9);
  make7filePlot(f1,f2,f3,f4,f5,f6,fops,"h_taupt","NormalizedEntries","#it{p_{T,#tau}}",0.72,0.55,0.85,0.9);
  make7filePlot(f1,f2,f3,f4,f5,f6,fops,"h_beta","NormalizedEntries","#it{#eta_{b}}",0.72,0.55,0.85,0.9);
  make7filePlot(f1,f2,f3,f4,f5,f6,fops,"h_mueta","NormalizedEntries","#it{#eta_{#mu}}",0.72,0.55,0.85,0.9);
  make7filePlot(f1,f2,f3,f4,f5,f6,fops,"h_taueta","NormalizedEntries","#it{#eta_{#tau}}",0.72,0.55,0.85,0.9);
}
