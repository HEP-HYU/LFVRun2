import uproot
import numpy as np
import matplotlib as plt
inputvars = ["Sel_muon1pt","Sel_muon1eta","Sel_tau1pt","Sel_tau1eta",
            "Sel2_jet1pt","Sel2_jet2pt","Sel2_jet3pt","Sel2_jet4pt",
            "Sel2_jet1eta","Sel2_jet2eta","Sel2_jet3eta","Sel2_jet4eta",
            "Sel2_jet1btag","Sel2_jet2btag","Sel2_jet3btag","Sel2_jet4btag",
            "Sys_METpt","Sys_METphi",
            "chi2","chi2_lfvTop_mass","chi2_SMW_mass","chi2_SMTop_mass"]

def hists(tree,pred,weight):
    hists = []
    keys = []
#    keys = list(tree.keys())
#    for key in keys:
#        bins=None
#        min=0.0
#        max=-1.0
#        if "jet" in key:
#            if "pt" in key:
#                bins=30
#                min=0
#                max=600
#            elif "eta" in key:
#                bins=18
#                min=-2.7
#                max=2.7
#            elif "btag" in key:
#                bins=20
#                min=0.0
#                max=1.0
#        elif ("muon" in key) or ("tau" in key):
#            if "pt" in key:
#                bins=20
#                min=0
#                max=800
#            elif "eta":
#                bins=18
#                min=-2.7
#                max=2.7
#        elif "MET" in key:
#            if "phi" in key:
#                bins=20
#                min=-4.0
#                max=4.0
#            elif "pt" in key:
#                bins=20
#                min=0
#                max=600
#        elif "chi2" in key:
#            if "mass" in key:
#                bins=20
#                min=0
#                max=400
#            else:
#                bins=20
#                min=0
#                max=20000
#        minmax = (min,max)
#        hist = np.histogram(tree[key],bins,(min,max),weight,density=False)
#        hists.append(hist)
    hists.append(np.histogram(pred,20,(0.0,1.0),weight,density=False))
    keys.append("dnn")
    return zip(keys, hists)

