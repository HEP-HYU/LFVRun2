import utils.stackhists as stackhists
import utils.CMS_lumi as CMS_lumi
import sys
import argparse

# Subplot ( Ratio plot : option -R, --ratio (default) / significance plot :  option -S, --significance )
parser = argparse.ArgumentParser()
parser.add_argument('-R', '--ratio', dest='ratio', action='store_true', default=False)
parser.add_argument('-S', '--significance', dest='significance', action='store_true', default=False)
parser.add_argument('-L', '--logstyle', dest='logstyle', action='store_true', default=False)
parser.add_argument('-Y', '--year', dest='year', type=str, default="")
parser.add_argument('-SYS', '--systematic', dest='systematic', type=str, default="norm")
args = parser.parse_args()
logstyle = args.logstyle
ratio = args.ratio
significance = args.significance
year = args.year
syst = "_"+args.systematic

# Lumi ratio dictionary for integrated Run2
rlumi={"16pre":1.,"16post":1.,"17":1.,"18":1.}
runs=[]
if year=="16pre":
    CMS_lumi.lumi_13TeV = "19.5 fb^{-1}"
    s = stackhists.Stackhists(19.5)
    runs=["16pre"]
if year=="16post":
    CMS_lumi.lumi_13TeV = "16.8 fb^{-1}"
    s = stackhists.Stackhists(16.8)
    runs=["16post"]
elif year=="17":
    CMS_lumi.lumi_13TeV = "41.5 fb^{-1}"
    s = stackhists.Stackhists(41.48)
    runs=["17"]
elif year=="18":
    CMS_lumi.lumi_13TeV = "59.8 fb^{-1}"
    s = stackhists.Stackhists(59.83)
    runs=["18"]
elif year=="run2":
    CMS_lumi.lumi_13TeV = "138 fb^{-1}"
    s = stackhists.Stackhists(137.65)
    rlumi["16pre"] = 19.5/137.65
    rlumi["16post"] = 16.8/137.65
    rlumi["17"] = 41.48/137.65
    rlumi["18"] = 59.83/137.65
    runs=["16pre","16post","17","18"]

year=str(year)
s.setupStyle(alpha=1)
CMS_lumi.extraText = ""
#CMS_lumi.extraText = "Simulation"

# DATA
if year=="run2":
    s.addChannel("Run2"+syst+"_pred.root", "data", 999, isMC=False)
    # LFV ( must be added before other MC )
    s.addChannel("18/ST_LFV_TCMuTau_Vector_18"+syst+"_pred.root", "LFV STc", 10, isMC=True, xsec=36.8, counterhistogramroot="18/ST_LFV_TCMuTau_Vector_18"+syst+"_pred.root")
    s.addChannel("18/ST_LFV_TUMuTau_Vector_18"+syst+"_pred.root", "LFV STu", 11, isMC=True, xsec=393, counterhistogramroot="18/ST_LFV_TUMuTau_Vector_18"+syst+"_pred.root")
    s.addChannel("18/TT_LFV_TToCMuTau_Vector_18"+syst+"_pred.root", "LFV TTc", 12, isMC=True, xsec=21.5, counterhistogramroot="18/TT_LFV_TToCMuTau_Vector_18"+syst+"_pred.root")
    s.addChannel("18/TT_LFV_TToUMuTau_Vector_18"+syst+"_pred.root", "LFV TTu", 13, isMC=True, xsec=21.5, counterhistogramroot="18/TT_LFV_TToUMuTau_Vector_18"+syst+"_pred.root")
else:
    s.addChannel(year+"/Run"+year+syst+"_pred.root", "data", 999, isMC=False)
    s.addChannel(year+"/ST_LFV_TCMuTau_Vector_"+year+syst+"_pred.root", "LFV STc", 10, isMC=True, xsec=rlumi[year]*36.8, counterhistogramroot=year+"/ST_LFV_TCMuTau_Vector_"+year+syst+"_pred.root")
    s.addChannel(year+"/ST_LFV_TUMuTau_Vector_"+year+syst+"_pred.root", "LFV STu", 11, isMC=True, xsec=rlumi[year]*393, counterhistogramroot=year+"/ST_LFV_TUMuTau_Vector_"+year+syst+"_pred.root")
    s.addChannel(year+"/TT_LFV_TToCMuTau_Vector_"+year+syst+"_pred.root", "LFV TTc", 12, isMC=True, xsec=rlumi[year]*21.5, counterhistogramroot=year+"/TT_LFV_TToCMuTau_Vector_"+year+syst+"_pred.root")
    s.addChannel(year+"/TT_LFV_TToUMuTau_Vector_"+year+syst+"_pred.root", "LFV TTu", 13, isMC=True, xsec=rlumi[year]*21.5, counterhistogramroot=year+"/TT_LFV_TToUMuTau_Vector_"+year+syst+"_pred.root")
for run in runs:
    # MC ( reference twiki : https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns )
    # TTbar
    s.addChannel(run+"/TTTo2L2Nu_"+run+syst+"_pred.root", "TT-di", 0, isMC=True, xsec=rlumi[run]*88290.0, counterhistogramroot=run+"/TTTo2L2Nu_"+run+syst+"_pred.root")
    s.addChannel(run+"/TTToSemiLeptonic_"+run+syst+"_pred.root", "TT-semi", 1, isMC=True, xsec=rlumi[run]*365340.0, counterhistogramroot=run+"/TTToSemiLeptonic_"+run+syst+"_pred.root")
    s.addChannel(run+"/TTToHadronic_"+run+syst+"_pred.root", "TT-had", 2, isMC=True, xsec=rlumi[run]*377960.0, counterhistogramroot=run+"/TTToHadronic_"+run+syst+"_pred.root")

    # W+Jets
    s.addChannel(run+"/WJetsToLNu_inclHT100_"+run+syst+"_pred.root", "W+jets", 3, isMC=True, xsec=rlumi[run]*61526700*0.9645, counterhistogramroot=run+"/WJetsToLNu_inclHT100_"+run+syst+"_pred.root")
    s.addChannel(run+"/WJetsToLNu_HT-100To200_"+run+syst+"_pred.root", "W+jets", 3, isMC=True, xsec=rlumi[run]*1345000.0*0.993, counterhistogramroot=run+"/WJetsToLNu_HT-100To200_"+run+syst+"_pred.root")
    s.addChannel(run+"/WJetsToLNu_HT-200To400_"+run+syst+"_pred.root", "W+jets", 3, isMC=True, xsec=rlumi[run]*359700.0*1.002, counterhistogramroot=run+"/WJetsToLNu_HT-200To400_"+run+syst+"_pred.root")
    s.addChannel(run+"/WJetsToLNu_HT-400To600_"+run+syst+"_pred.root", "W+jets", 3, isMC=True, xsec=rlumi[run]*48910.0*1.009, counterhistogramroot=run+"/WJetsToLNu_HT-400To600_"+run+syst+"_pred.root")
    s.addChannel(run+"/WJetsToLNu_HT-600To800_"+run+syst+"_pred.root", "W+jets", 3, isMC=True, xsec=rlumi[run]*12050.0*1.120, counterhistogramroot=run+"/WJetsToLNu_HT-600To800_"+run+syst+"_pred.root")
    s.addChannel(run+"/WJetsToLNu_HT-800To1200_"+run+syst+"_pred.root", "W+jets", 3, isMC=True, xsec=rlumi[run]*5501.0*1.202, counterhistogramroot=run+"/WJetsToLNu_HT-800To1200_"+run+syst+"_pred.root")
    s.addChannel(run+"/WJetsToLNu_HT-1200To2500_"+run+syst+"_pred.root", "W+jets", 3, isMC=True, xsec=rlumi[run]*1329.0*1.332, counterhistogramroot=run+"/WJetsToLNu_HT-1200To2500_"+run+syst+"_pred.root")
    s.addChannel(run+"/WJetsToLNu_HT-2500ToInf_"+run+syst+"_pred.root", "W+jets", 3, isMC=True, xsec=rlumi[run]*32.16*4.200, counterhistogramroot=run+"/WJetsToLNu_HT-2500ToInf_"+run+syst+"_pred.root")

    # DY
    s.addChannel(run+"/DYJetsToLL_M-10to50_"+run+syst+"_pred.root", "Z+jets", 4, isMC=True, xsec=rlumi[run]*18610000.0, counterhistogramroot=run+"/DYJetsToLL_M-10to50_"+run+syst+"_pred.root")
    #s.addChannel(run+"/DYJetsToLL_M-50_"+run+syst+"_pred.root", "Z+jets", 4, isMC=True, xsec=rlumi[run]*6077220.0, counterhistogramroot=run+"/DYJetsToLL_M-50-amcatnloFXFX_"+run+syst+"_pred.root")
    s.addChannel(run+"/DYJetsToLL_M-50_madgraph_"+run+syst+"_pred.root", "Z+jets", 4, isMC=True, xsec=rlumi[run]*6077220.0, counterhistogramroot=run+"/DYJetsToLL_M-50_madgraph_"+run+syst+"_pred.root")

    # ST
    s.addChannel(run+"/ST_t-channel_top_"+run+syst+"_pred.root", "ST", 5, isMC=True, xsec=rlumi[run]*136020.0, counterhistogramroot=run+"/ST_t-channel_top_"+run+syst+"_pred.root")
    s.addChannel(run+"/ST_t-channel_antitop_"+run+syst+"_pred.root", "ST", 5, isMC=True, xsec=rlumi[run]*80950.0, counterhistogramroot=run+"/ST_t-channel_antitop_"+run+syst+"_pred.root")
    s.addChannel(run+"/ST_tW_top_"+run+syst+"_pred.root", "ST", 5, isMC=True, xsec=rlumi[run]*35850.0, counterhistogramroot=run+"/ST_tW_top_"+run+syst+"_pred.root")
    s.addChannel(run+"/ST_tW_antitop_"+run+syst+"_pred.root", "ST", 5, isMC=True, xsec=rlumi[run]*35850.0, counterhistogramroot=run+"/ST_tW_antitop_"+run+syst+"_pred.root")

    # VV
    s.addChannel(run+"/WW_"+run+syst+"_pred.root", "VV", 6, isMC=True, xsec=rlumi[run]*118700.0, counterhistogramroot=run+"/WW_"+run+syst+"_pred.root")
    s.addChannel(run+"/WZ_"+run+syst+"_pred.root", "VV", 6, isMC=True, xsec=rlumi[run]*47130.0, counterhistogramroot=run+"/WZ_"+run+syst+"_pred.root")
    s.addChannel(run+"/ZZ_"+run+syst+"_pred.root", "VV", 6, isMC=True, xsec=rlumi[run]*16523.0, counterhistogramroot=run+"/ZZ_"+run+syst+"_pred.root")

    # TTX
    s.addChannel(run+"/TTWJetsToLNu_"+run+syst+"_pred.root", "TTX", 7, isMC=True, xsec=rlumi[run]*204.3, counterhistogramroot=run+"/TTWJetsToLNu_"+run+syst+"_pred.root")
    s.addChannel(run+"/TTWJetsToQQ_"+run+syst+"_pred.root", "TTX", 7, isMC=True, xsec=rlumi[run]*406.2, counterhistogramroot=run+"/TTWJetsToQQ_"+run+syst+"_pred.root")
    s.addChannel(run+"/TTZToLLNuNu_"+run+syst+"_pred.root", "TTX", 7, isMC=True, xsec=rlumi[run]*252.9, counterhistogramroot=run+"/TTZToLLNuNu_"+run+syst+"_pred.root")
    s.addChannel(run+"/TTZToQQ_"+run+syst+"_pred.root", "TTX", 7, isMC=True, xsec=rlumi[run]*529.7, counterhistogramroot=run+"/TTZToQQ_"+run+syst+"_pred.root")

# Histograms
s.addHistogram("h_dnn_pred", "Signal prediction", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=logstyle, ymin=0.1)

subplot="R"
if ratio : subplot="R"
elif significance : subplot="S"

s.draw(subplot)
 
