import os
import sys
os.environ["CUDA_VISIBLE_DEVICES"] = "3"
import uproot
import pandas as pd
import awkward as ak
import numpy as np
import tensorflow as tf
import matplotlib.pyplot as plt
import tensorflow.keras.backend as K
from keras.callbacks import EarlyStopping, ModelCheckpoint
import multiprocessing


def min_max_scaling(series):
    return (series - series.min()) / (series.max() - series.min())


base_dir = os.getcwd().replace("DNN","") # Upper directory
processed = "October2023_AfterPreAppTalk_v4"
label = "top_lfv_multiClass"

inputvars = [ "Muon1_pt","Muon1_eta",
        "Tau1_pt","Tau1_mass","Tau1_eta",
        "Jet1_pt","Jet1_mass","Jet1_eta","Jet1_btagDeepFlavB",
        "Jet2_pt","Jet2_mass","Jet2_eta","Jet2_btagDeepFlavB",
        "Jet3_pt","Jet3_mass","Jet3_eta","Jet3_btagDeepFlavB",
        "chi2","chi2_SMW_mass","chi2_SMTop_mass",
        "chi2_wqq_dEta","chi2_wqq_dPhi","chi2_wqq_dR",
        "mutau_mass","mutau_dEta","mutau_dPhi","mutau_dR",
	"MET_pt"
        ]

discriminators = {"p_st" : 2, "p_tt" : 1 , "p_bkg" : 0 , "p_st_tt" : 999, "p_st_tt_ob" : 999 }

def run(inputs):
    year = inputs[0]
    input_file = inputs[1]
    discriminator_key = inputs[2]
    alpha = inputs[3]
    #print(year, discriminator_key , alpha)

    binedges = [0,1,2,5,10,30,60] 
    
    model_dir = label+"_"+processed+"/nom/best_model.h5"
    model = tf.keras.models.load_model(model_dir)
    
    eval_dir = label+"_"+processed+"/"
    
    #hists_path = eval_dir+"/"+year+"/preds/"+discriminator_key+"/"+str(alpha).replace(".","p")+"/"
    hists_path = "/data1/users/ecasilar/Oct31/"+year+"/"
    if not os.path.isdir(hists_path):
        os.makedirs(hists_path)
    
    eventWeight = "eventWeight"
    weights = [eventWeight] 
    
    outf_dir = hists_path+input_file.split("/")[-1:][0]

    #print("IN PATH : ", input_file) 
    #print("OUT PATH : ", outf_dir) 

    infile = uproot.open(input_file)
    tree = infile["Events"]
    branch_names = tree.keys()
    syst_list = [syst.split("__")[1] for syst in branch_names if "eventWeight__" in syst]
    hcounter = infile["hcounter"]
    nEvents = len(tree)
    syst_hist_dnn_dict = {}
    syst_hist_dnn_entries_dict = {}
    hist_nevents_S4_dict = {}
    hist_nevents_S5_dict = {}
        
    if not "SingleMuon" in input_file :
        infile_forS = uproot.open(input_file.replace("_FF",""))
        h_nevents_S4_nobtag = infile_forS["h_nevents_S4_nobtag"]  ### get it from removed FF 
        h_nevents_S4 = infile_forS["h_nevents_S4"]	
        h_nevents_S2_nobtag = infile_forS["h_nevents_S2_nobtag"]
        h_nevents_S2 = infile_forS["h_nevents_S2"]	
        if any(string in input_file for string in ["_LFV","TTT","_ST_t"]) and "__" not in input_file:
           ScaleWeightSum = infile['ScaleWeightSum']
           PSWeightSum = infile['PSWeightSum']
           LHEPdfWeightSum = infile['LHEPdfWeightSum']

    if nEvents == 0:
        #print("No events : "+input_file)
        #Need to add empth histograms for technical reasons ....
        pred = []
        pd_weight = []
        dnnhist_nom = np.histogram(pred,bins=binedges,weights=pd_weight,density=False)
        dnnhist_entries_nom = np.histogram(pred,bins=binedges,density=False)
        for syst in syst_list:
            syst_hist_dnn_dict["h_dnn_pred_S5__"+syst] = dnnhist_nom
            syst_hist_dnn_entries_dict["h_dnn_entries_S5__"+syst] = dnnhist_entries_nom
            if 'btag' in syst:
              hist_nevents_S4_dict["h_nevents_S4__"+syst] = infile_forS["h_nevents_S4__"+syst]  
              hist_nevents_S5_dict["h_nevents_S5__"+syst] = infile["h_nevents_S5__"+syst]
    else:
        #print("Good file :",input_file)
        pd_data = tree.arrays(inputvars,library="pd")
        pd_weight = tree.arrays(weights,library="np")
        pred_data = np.array(pd_data.filter(items = inputvars))

        #print("prediction peformed only once per file")
        pred = model.predict(pred_data,batch_size=128, workers=1, use_multiprocessing=False)

        rmzeros = pred[:,0] 
        rmzeros[rmzeros<=0.00] = 0.001
        pred[:,0] = rmzeros
        pred = (((1-alpha)*pred[:,2]+alpha*pred[:,1])/pred[:,0]).tolist()
        pred = np.array(pred)
        pred[pred>=50] = 50
        pred = pred.tolist()
        pd_weight = pd_weight[eventWeight].tolist()
        dnnhist_nom = np.histogram(pred,bins=binedges,weights=pd_weight,density=False)
        dnnhist_entries_nom = np.histogram(pred,bins=binedges,density=False)
        

        #print("starting syst weighted hists ")

        for syst in syst_list:
            pd_weight = tree.arrays(["eventWeight__"+syst],library="np")
            pd_weight = pd_weight["eventWeight__"+syst].tolist()
            dnnhist_Wsyst = np.histogram(pred,bins=binedges,weights=pd_weight,density=False)
            dnnhist_entries_Wsyst = np.histogram(pred,bins=binedges,density=False)
            syst_hist_dnn_dict["h_dnn_pred_S5__"+syst] = dnnhist_Wsyst
            syst_hist_dnn_entries_dict["h_dnn_entries_S5__"+syst] = dnnhist_entries_Wsyst
            if 'btag' in syst:
               hist_nevents_S4_dict["h_nevents_S4__"+syst] = infile_forS["h_nevents_S4__"+syst]  
               hist_nevents_S5_dict["h_nevents_S5__"+syst] = infile["h_nevents_S5__"+syst]

    with uproot.recreate(outf_dir) as outf:
       outf["h_dnn_pred_S5"] = dnnhist_nom
       outf["h_dnn_entries_S5"] = dnnhist_entries_nom
       outf["hcounter"] = hcounter
       if not "SingleMuon" in input_file : 
           outf["h_nevents_S4_nobtag"] = h_nevents_S4_nobtag
           outf["h_nevents_S4"] = h_nevents_S4
           outf["h_nevents_S2_nobtag"] = h_nevents_S2_nobtag
           outf["h_nevents_S2"] = h_nevents_S2
           if any(string in input_file for string in ["_LFV","TTT","_ST_t"]) and "__" not in input_file:
              #print("YEAYAYYY  SCALESUM ZIMBIRTISI")
              outf["ScaleWeightSum"] = ScaleWeightSum
              outf["PSWeightSum"] = PSWeightSum
              outf["LHEPdfWeightSum"] = LHEPdfWeightSum

       for syst in syst_list:
           if 'pdf' in syst and not 'alphas' in syst:
              #print("YES syst PDF")
              systnum = int(syst.split("pdf")[1])
              if systnum > 100: continue
              if systnum%2 !=0 : 
                 systupdated="pdf"+str(int((systnum/2.)+0.5))+"up"
                 outf["h_dnn_pred_S5__"+systupdated] = syst_hist_dnn_dict["h_dnn_pred_S5__"+syst] 
                 outf["h_dnn_entries_S5__"+systupdated] = syst_hist_dnn_entries_dict["h_dnn_entries_S5__"+syst]
              if systnum%2 ==0 :
                 systupdated="pdf"+str(int((systnum/2.)+0.5))+"down"
                 outf["h_dnn_pred_S5__"+systupdated] = syst_hist_dnn_dict["h_dnn_pred_S5__"+syst] 
                 outf["h_dnn_entries_S5__"+systupdated] = syst_hist_dnn_entries_dict["h_dnn_entries_S5__"+syst]
              #print("YES syst NEW PDF" , syst)
           else:
              outf["h_dnn_pred_S5__"+syst] = syst_hist_dnn_dict["h_dnn_pred_S5__"+syst] 
              outf["h_dnn_entries_S5__"+syst] = syst_hist_dnn_entries_dict["h_dnn_entries_S5__"+syst]
           if 'btag' in syst:
              outf["h_nevents_S4__"+syst] = hist_nevents_S4_dict["h_nevents_S4__"+syst]
              outf["h_nevents_S5__"+syst] = hist_nevents_S5_dict["h_nevents_S5__"+syst]

    K.clear_session()

if __name__ == '__main__':

    print("Start Multi LFV Evaluation")
    discriminator = "p_st_tt_ob"
    alpha=0.1
    parameters = []
    #for year in ["2016pre","2016post","2017","2018"]:
    for year in ["2016pre"]: #,"2016post","2017","2018"]:
       project_dir = "/data1/users/minerva1993/work/lfv_production/LFVRun2/nanoaodframe/v9_0714_1010_FF/"+year+"/"
       flist = os.listdir(project_dir)
       flist = [i for i in flist if (".root" in i)]
       #print(len(flist))
       for curfile in flist:
          parameters.append((year, project_dir+curfile ,discriminator,alpha))

    pool = multiprocessing.get_context("spawn").Pool(12)
    pool.map(run, parameters)
    pool.close()
    pool.join()
