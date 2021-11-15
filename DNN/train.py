import os
import sys

#os.environ["CUDA_DEVICE_ORDER"] = "PCI_BUS_ID"
os.environ["CUDA_VISIBLE_DEVICES"] = "1"

import uproot
import pandas as pd
import numpy as np
import tensorflow as tf
import matplotlib.pyplot as plt
from keras.callbacks import EarlyStopping, ModelCheckpoint
from utils.plots import *

root_dir = os.getcwd().replace("DNN","") # Upper directory
# DNN structure
epochs = 1000
nodes = 20
layers = 3
sbratio = 1 # sig:bkg events ratio (1:1, 1:1.5, 1:2)
#drop = 0.10
inputvars = ["Sel_muon1pt","Sel_muon1eta","Sel_tau1pt","Sel_tau1eta",
    "Sel2_jet1pt","Sel2_jet2pt","Sel2_jet3pt",
    "Sel2_jet1eta","Sel2_jet2eta","Sel2_jet3eta",
    "Sel2_jet1btag","Sel2_jet2btag","Sel2_jet3btag",
    "MET_pt_corr","MET_phi_corr",
    "chi2","chi2_lfvTop_mass","chi2_SMW_mass","chi2_SMTop_mass"]
class_names = ["sig", "bkg"]
#for p in ["ST","TT"]:
for p in ["TT"]:
    print("Start "+p+" LFV Training")
    inputvars = []
    if p == "ST":
        inputvars = ["Sel_muon1pt","Sel_muon1eta","Sel_tau1pt","Sel_tau1eta",
            "Sel2_jet1pt","Sel2_jet2pt","Sel2_jet3pt",
            "Sel2_jet1eta","Sel2_jet2eta","Sel2_jet3eta",
            "Sel2_jet1btag","Sel2_jet2btag","Sel2_jet3btag",
            "MET_pt_corr","MET_phi_corr",
            "chi2","chi2_SMW_mass","chi2_SMTop_mass"]
    elif p == "TT":
        inputvars = ["Sel_muon1pt","Sel_muon1eta","Sel_tau1pt","Sel_tau1eta",
            "Sel2_jet1pt","Sel2_jet2pt","Sel2_jet3pt",
            "Sel2_jet1eta","Sel2_jet2eta","Sel2_jet3eta",
            "Sel2_jet1btag","Sel2_jet2btag","Sel2_jet3btag",
            "MET_pt_corr","MET_phi_corr",
            "chi2","chi2_lfvTop_mass","chi2_SMW_mass","chi2_SMTop_mass"]

    project_dir = "nanoaodframe_"+p+"LFV/nov_01_norm/"    # MODIFY!!!
    sig1_filedir = root_dir+project_dir+"ST_LFV_norm.root"
    sig2_filedir = root_dir+project_dir+"TT_LFV_norm.root"
    bkg1_filedir = root_dir+project_dir+"TTTo2L2Nu_norm.root"
    bkg2_filedir = root_dir+project_dir+"TTToSemiLeptonic_norm.root"
    train_outdir = "./"+p+"nov_01_"+"norm_"+str(nodes)+"nodes_"+str(layers)+"layers_s1b"+str(sbratio)
    os.makedirs(train_outdir, exist_ok=True)

    sig1_tree = uproot.open(sig1_filedir)["outputTree2"]
    sig2_tree = uproot.open(sig2_filedir)["outputTree2"]
    bkg1_tree = uproot.open(bkg1_filedir)["outputTree2"]
    bkg2_tree = uproot.open(bkg2_filedir)["outputTree2"]

    df_sig1 = sig1_tree.arrays(inputvars,library="pd")
    df_sig2 = sig2_tree.arrays(inputvars,library="pd")
    df_bkg1 = bkg1_tree.arrays(inputvars,library="pd")
    df_bkg2 = bkg2_tree.arrays(inputvars,library="pd")

    df_sig = pd.concat([df_sig1,df_sig2])
    df_bkg = pd.concat([df_bkg1,df_bkg2])
    ntotsig = len(df_sig)
    ntotbkg = len(df_bkg)

    nsig = ntotsig
    nbkg = ntotbkg

    if nsig >= nbkg:
        if sbratio == 1:
            nsig = nbkg
        elif sbratio == 2:
            nsig = int(nbkg/2)
        elif sbratio == 3:
            nsig = int(nbkg/3)
    else:
        if sbratio == 1:
            nbkg = nsig
        elif nsig>=int(nbkg/2):
            if sbratio == 2:
                nsig = int(nbkg/2)
            elif sbratio == 3:
                nsig = int(nbkg/3)
        else:
            "Error::Check the number of events!"
            sys.exit()

    print("LFV : "+str(nsig)+" events")
    print("TT  : "+str(nbkg)+" events")

    df_sig = df_sig.sample(n=nsig)
    df_bkg = df_bkg.sample(n=nbkg)
    df_sig["category"] = 1
    df_bkg["category"] = 0

    pd_data = pd.concat([df_sig,df_bkg])
    pd_sig = pd_data[pd_data['category'] == 1]
    pd_bkg = pd_data[pd_data['category'] == 0]
    plot_corrMatrix(pd_data,train_outdir,"total")
    plot_corrMatrix(pd_sig,train_outdir,"sig")
    plot_corrMatrix(pd_bkg,train_outdir,"bkg")

    pd_data = pd_data.sample(frac=1).reset_index(drop=True)

    train_data = np.array(pd_data.filter(items = inputvars))
    train_out  = np.array(pd_data.filter(items = ['category']))

    numbertr = len(train_out)
    trainlen = int(0.8*numbertr) # Fraction used for training

    # Splitting between training set and cross-validation set
    valid_data=train_data[trainlen:,0::]
    valid_data_out=train_out[trainlen:]

    train_data=train_data[:trainlen,0::]
    train_data_out=train_out[:trainlen]

    es = EarlyStopping(monitor='val_loss', mode='min', verbose=1, patience=10)
    mc = ModelCheckpoint(train_outdir+'/best_model.h5', monitor='val_loss', mode='min', save_best_only=True)
    # model
    model = tf.keras.models.Sequential()
    model.add(tf.keras.layers.Flatten(input_shape = (train_data.shape[1],)))
    for i in range(layers):
        model.add(tf.keras.layers.BatchNormalization())
        #model.add(tf.keras.layers.Dropout(drop))
        model.add(tf.keras.layers.Dense(nodes, activation=tf.nn.relu, kernel_regularizer='l2'))
    model.add(tf.keras.layers.Dense(2, activation=tf.nn.softmax))
    batch_size = 1024

    model.compile(loss='sparse_categorical_crossentropy',
                  optimizer = 'adam',
                  metrics=['accuracy', 'sparse_categorical_accuracy'])

    hist = model.fit(train_data, train_data_out, batch_size=batch_size, epochs=epochs, 
                                    validation_data=(valid_data,valid_data_out), callbacks=[es, mc])

    #tf.keras.models.save_model(model,'model_{epoch:%d}.h5', overwrite=True, include_optimizer=True)

    print(hist.history.keys())
    model.summary()

    output = model.predict(train_data)
    train_result = pd.DataFrame(np.array([train_data_out.T[0], output.T[1]]).T, columns=["True", "Pred"])
    output = model.predict(valid_data)
    test_result = pd.DataFrame(np.array([valid_data_out.T[0], output.T[1]]).T, columns=["True", "Pred"])

    pred = np.argmax(output, axis=1)

    #pred = np.argmax(model.predict(valid_data), axis=1)
    comp = np.reshape(valid_data_out,(-1))

    acc = 100 * np.mean( pred == comp )

    plot_confusion_matrix(comp, pred, classes=class_names,
                        title='Confusion matrix, without normalization, acc=%.2f'%acc, savename=train_outdir+"/confusion_matrix.pdf")

    plot_confusion_matrix(comp, pred, classes=class_names, normalize=True,
                        title='Normalized confusion matrix, acc=%.2f'%acc, savename=train_outdir+"/norm_confusion_matrix.pdf")

    plot_performance(hist=hist, savedir=train_outdir)

    plot_output_dist(train_result, test_result, savedir=train_outdir)

