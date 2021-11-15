import ROOT
from ROOT import *
import os
import sys

if not os.path.isdir("dycompare"):
    os.mkdir("dycompare")

hnames = ["h1muon1pt_cut0","h1muon1pt_cut00","h1muon1pt_cut000","h1tau1pt_cut00","h1metpt_cut0","h1metpt_cut00","h1ncleanjetspass_cut0","h1ncleanjetspass_cut00","h1ncleantaupass_cut0","hncleanjetspass_cut0000","hncleanbjetspass_cut000"]

f1name = sys.argv[1]
f2name = sys.argv[2]
f1 = TFile(f1name)
f2 = TFile(f2name)

for hname in hnames:
    h1 = f1.Get(hname)
    h2 = f2.Get(hname)
    print(h1.GetEntries())
    print(h2.GetEntries())
    h1.SetLineColor(kBlue)
    h2.SetLineColor(kRed)
    h1.SetLineWidth(2)
    h2.SetLineWidth(2)

    h1.Scale(1/h1.Integral())
    h2.Scale(1/h2.Integral())
    m = h1.GetMaximum()*1.2 if h1.GetMaximum() > h2.GetMaximum() else h2.GetMaximum()*1.2
    c1 = TCanvas('c','c',600,600)
    c1.cd()
    c1.SetMargin(0.1,0.05,0.1,0.05)
    gStyle.SetOptStat(0)
    gStyle.SetOptTitle(0)
    h1.SetMaximum(m)
    h1.Draw("hist")
    h2.Draw("histsame")
    
    leg = TLegend(0.55,0.6,0.9,0.9)
    gStyle.SetLegendTextSize(0.04)
    leg.SetBorderSize(0)
    leg.AddEntry(h1,"amcatnlo")
    leg.AddEntry(h2,"madgraph")
    leg.Draw()
    
    c1.Print("dycompare/h_DYcomparison_"+hname+".pdf")
    c1.Close()
