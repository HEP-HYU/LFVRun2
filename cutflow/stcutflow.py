from ROOT import *
import os
import csv
import numpy as np
import sys

# import cutflow modules
import xsec

path = sys.argv[1]
pname = path.split("/")[-2]
print(pname)
runs = ["16", "17", "18"]
flist16 = os.listdir(path+"/16")
flist17 = os.listdir(path+"/17")
flist18 = os.listdir(path+"/18")
flist = {"16":flist16, "17":flist17, "18":flist18}
for run in runs:
    tmpflist = [[j for j in i if ".root" in j] for i in flist[run]]
    flist[run] = [i for i in flist[run] if ".root" in i]

# !! Check Selection Steps !!
steps = 4

# Cross Sections (pb)
Xsec_dict = xsec.xsec
lumi=None

def run(run):
    if run=="16":
        lumi = 35.92 # fb^(-1)
    elif run=="17":
        lumi = 41.53 # fb^(-1)
    elif run=="18":
        lumi = 59.74 # fb^(-1)
    print(run)
    list_bkgs = [[0 for _ in range(steps+1)],[0 for _ in range(steps+1)]]
    LFV_STc_s = list_bkgs
    LFV_STc_v = list_bkgs
    LFV_STc_t = list_bkgs
    LFV_STu_s = list_bkgs
    LFV_STu_v = list_bkgs
    LFV_STu_t = list_bkgs
    LFV_TTc_s = list_bkgs
    LFV_TTc_v = list_bkgs
    LFV_TTc_t = list_bkgs
    LFV_TTu_s = list_bkgs
    LFV_TTu_v = list_bkgs
    LFV_TTu_t = list_bkgs
    ttdi = list_bkgs
    ttsemi = list_bkgs
    tthad = list_bkgs
    DY = list_bkgs
    ST = list_bkgs
    TTX = list_bkgs
    VV = list_bkgs
    WJets = list_bkgs
    QCD = list_bkgs
    DATA = list_bkgs
    for rfile in flist[run]:
        nevts_integral = []
        nevts_entry = []
        rname = str(rfile.split('.')[0])
        data = TFile.Open(path+"/"+run+"/"+rfile)
        h_count = data.Get("hnEvents")
        if not h_count:
            print("Not a valid file : %s"%(rfile))
            continue
        if "WJetsToLNu_inclHT100" in rfile:
            if run=="16":
                nevts_integral.append(83345650)
                nevts_entry.append(83345650)
            elif run=="17":
                nevts_integral.append(103786310)
                nevts_entry.append(103786310)
            elif run=="18":
                nevts_integral.append(67906914)
                nevts_entry.append(67906914)
        else:
            nevts_integral.append(h_count.GetBinContent(2))
            nevts_entry.append(h_count.GetBinContent(1))
        for j in range(steps):
            if j<4:
                nevts_integral.append(data.Get("hnevents_pglep_cut"+str(0)*(j+1)).Integral())
                nevts_entry.append(data.Get("hnevents_pglep_cut"+str(0)*(j+1)).GetEntries())
            else:
                nevts_integral.append(data.Get("hnevents_cut"+str(0)*(j+1)).Integral())
                nevts_entry.append(data.Get("hnevents_cut"+str(0)*(j+1)).GetEntries())

        # Rescale for conservation of event yields when btagging SF is applied.
        prehist = data.Get("hnevents_pglep_cut000")
        posthist = data.Get("hnevents_cut000")
        rescale = prehist.Integral() / posthist.Integral() if posthist.GetEntries() != 0 else 1.0
        resflist = [1.0]*4
        for _ in range(steps-3):
            resflist.append(rescale)
        data.Close() # rootfile closed

        nevts_staterr=np.sqrt(np.array(nevts_entry)).tolist()
        if not "Run" in rfile:
            rfilename = rfile.split('.')[0]
            dictname = rfilename.split('_'+run)[0]
            norm_nevts = [ Xsec_dict[dictname] * (10**3) * lumi * i / nevts_integral[0] for i in nevts_integral ]
            norm_staterr = [ Xsec_dict[dictname] * (10**3) * lumi * i / nevts_integral[0] for i in nevts_staterr ]
            norm_nevts = np.multiply(norm_nevts, resflist).tolist()
        else:
            norm_nevts = nevts_integral
            norm_staterr = nevts_staterr
        tmp_proc = []
        tmp_proc.append(norm_nevts)
        tmp_proc.append(norm_staterr)
        
        if "LFV_ST_TC" in rname:
            if "Scalar" in rname:
                LFV_STc_s = np.add(LFV_STc_s, tmp_proc).tolist()
            elif "Vector" in rname:
                LFV_STc_v = np.add(LFV_STc_v, tmp_proc).tolist()
            elif "Tensor" in rname:
                LFV_STc_t = np.add(LFV_STc_t, tmp_proc).tolist()
        elif "LFV_ST_TU" in rname:
            if "Scalar" in rname:
                LFV_STu_s = np.add(LFV_STu_s, tmp_proc).tolist()
            elif "Vector" in rname:
                LFV_STu_v = np.add(LFV_STu_v, tmp_proc).tolist()
            elif "Tensor" in rname:
                LFV_STu_t = np.add(LFV_STu_t, tmp_proc).tolist()
        elif "LFV_TT_TToC" in rname:
            if "Scalar" in rname:
                LFV_TTc_s = np.add(LFV_TTc_s, tmp_proc).tolist()
            elif "Vector" in rname:
                LFV_TTc_v = np.add(LFV_TTc_v, tmp_proc).tolist()
            elif "Tensor" in rname:
                LFV_TTc_t = np.add(LFV_TTc_t, tmp_proc).tolist()
        elif "LFV_TT_TToU" in rname:
            if "Scalar" in rname:
                LFV_TTu_s = np.add(LFV_TTu_s, tmp_proc).tolist()
            elif "Vector" in rname:
                LFV_TTu_v = np.add(LFV_TTu_v, tmp_proc).tolist()
            elif "Tensor" in rname:
                LFV_TTu_t = np.add(LFV_TTu_t, tmp_proc).tolist()
        else:
            if ("Run" in rname):
                DATA = np.add(DATA, tmp_proc).tolist()
            elif "TTTo2L2Nu" in rname:
                ttdi = np.add(ttdi, tmp_proc).tolist()
            elif "TTToSemi" in rname:
                ttsemi = np.add(ttsemi, tmp_proc).tolist()
            elif "TTToHad" in rname:
                tthad = np.add(tthad, tmp_proc).tolist()
            elif "DY" in rname:
                DY = np.add(DY, tmp_proc).tolist()
            elif "ST" in rname:
                ST = np.add(ST, tmp_proc).tolist()
            elif ("TTW" in rname) or ("TTZ" in rname):
                TTX = np.add(TTX, tmp_proc).tolist()
            elif ("WW" in rname) or ("WZ" in rname) or ("ZZ" in rname):
                VV = np.add(VV, tmp_proc).tolist()
            elif "WJets" in rname:
                WJets = np.add(WJets, tmp_proc).tolist()
            elif ("QCD" in rname):
                QCD = np.add(QCD, tmp_proc).tolist()

    LFV = [LFV_STc_s,LFV_STc_v,LFV_STc_t,LFV_STu_s,LFV_STu_v,LFV_STu_t,LFV_TTc_s,LFV_TTc_v,LFV_TTc_t,LFV_TTu_s,LFV_TTu_v,LFV_TTu_t]
    totalMC = np.add(np.add(np.add(np.add(np.add(np.add(np.add(np.add(ttdi,ttsemi),tthad),DY),ST),TTX),VV),WJets),QCD).tolist()
    #totalMC = np.add(np.add(np.add(np.add(np.add(np.add(np.add(ttdi,ttsemi),tthad),DY),ST),TTX),VV),WJets)
    dataMC = np.divide(DATA[0],totalMC[0]).tolist()
    dataMC_error = np.multiply(dataMC,np.divide(totalMC[1],totalMC[0])).tolist()
    dataMC = np.append(dataMC,dataMC_error).reshape(2,steps+1).tolist()
    snb = [0 for _ in range(len(LFV))]
    acc = [0 for _ in range(len(LFV))]
    tmpCutflow = []
    tmpCutflow = [LFV_STc_s,LFV_STc_v,LFV_STc_t,LFV_STu_s,LFV_STu_v,LFV_STu_t,
            LFV_TTc_s,LFV_TTc_v,LFV_TTc_t,LFV_TTu_s,LFV_TTu_v,LFV_TTu_t,
            ttdi, ttsemi, tthad, DY, ST, WJets, TTX, VV, QCD,
            totalMC, DATA, dataMC]
    for i in range(len(LFV)):
        if LFV[i][0][0] == 0: continue
        snb = np.divide(LFV[i][0],np.sqrt(totalMC[0]))
        snb.resize(2,steps+1)
        tmpCutflow.append(snb)

    for i in range(len(LFV)):
        if LFV[i][0][0] == 0: continue
        acc = np.array([j/LFV[i][0][0] for j in LFV[i][0]])
        acc.resize(2,steps+1)
        tmpCutflow.append(acc)
    return tmpCutflow
# Cutflow list shape : (48, 2, 5)
# LFV       = cutflow[0:12]
# MC        = cutflow[12:21]
# totalMC   = cutflow[21]
# DATA      = cutflow[22]
# dataMC    = cutflow[23]
# Sig       = cutflow[24:36]
# Acc       = cutflow[36:48]

def formatCutflow(cutflow):
    # Format
    # LFV vec   = cutflow[0:3]
    # MC        = cutflow[3:12]
    # totalMC   = cutflow[12]
    # DATA      = cutflow[13]
    # dataMC    = cutflow[14]
    # Sig       = cutflow[15:18]
    # Acc       = cutflow[18:21]
    out=[]
    cut1=[]
    cut2=[]
    cut3=[]
    cut4=[]
    for p in cutflow[1:13:3]+cutflow[12:22]:
        l=[]
        for i in range(len(p[0][1:])):
            j = p[0][i+1]
            k = p[1][i+1]
            if(len(cut1)<22):
                l.append( '{:,.3g}'.format(j) if j < 100 else '{:,.0f}'.format(j) )
                l.append( '{:,.3f}'.format(k) if k < 1 else '{:,.3g}'.format(k) if k<100 else '{:,.0f}'.format(k) )
            elif(len(cut1)==22):
                l.append( '{:,.2f}'.format(j) if j < 1 else '{:,.3f}'.format(j) )
                l.append( '{:,.2f}'.format(k) if k < 1 else '{:,.3f}'.format(k) )
            else:
                l.append('{:.2f}'.format(j))
                l.append('{:.2f}'.format(k))
        cut1.append(l)

    cut2=[ '{:,.3g}'.format(j) if j < 100 else '{:,.0f}'.format(j) for j in cutflow[22][0][1:] ]
    for i in range(len(cutflow[23][0][1:])):
        j = cutflow[23][0][i+1]
        k = cutflow[23][1][i+1]
        cut3.append('{:.2f}'.format(j))
        cut3.append('{:.2f}'.format(k))

    for p in cutflow[24:31:3]+cutflow[36:43:3]:
        l=[ "{:.2f}".format(j) if j>1 else "{:.3f}".format(j) for j in p[0]]
        cut4.append(l[1:])
    out=np.concatenate((np.array(cut1).flatten(),np.array(cut2).flatten(),np.array(cut3).flatten(),np.array(cut4).flatten()))
    return out

run2cutflow = np.zeros((48,2,steps+1))
cutflows = []           # Full Lists (shape (48,2,6))
formattedcutflows = []  # Formatted for Cutflow Table (shape (1,))
for year in runs:
    cutflow = run(year)
    cutflows.append(cutflow)
    formattedcutflows.append(formatCutflow(cutflow))
    run2cutflow = np.add(run2cutflow,cutflow)
run2cutflow=run2cutflow.tolist()
cutflows.append(run2cutflow)

# Calculation data/mc ratio for Integrated Run II
run2dataMC = np.divide(run2cutflow[22][0],run2cutflow[21][0]).tolist()
run2dataMC_error = np.multiply(run2dataMC,np.divide(run2cutflow[21][1],run2cutflow[21][0])).tolist()
run2dataMC = np.append(run2dataMC,run2dataMC_error).reshape(2,steps+1).tolist()
run2cutflow[23] = run2dataMC
formattedcutflows.append(formatCutflow(run2cutflow))

def texScript(run,formattedcutflow):
    h1 = "\\begin{table}[!hp]\n    \\tiny\n    \centering\n"
    h2 = "    \\renewcommand{\\arraystretch}{1.3}\n    \\begin{tabular}{c|r|r|r|r}\n"
    h3 = "    \\textbf{"+run+" (%s~\\fbinv)} & \\textbf{One muon} & \\textbf{One tau, $m_{\\mu\\tau}>150\GeV$} & \\textbf{nJets$\geq$3} & \\textbf{One b-tagged Jet}\\\\ \hline \n"%(lumi)
    s = "    LFV ST $tc\\mu\\tau$  " + " & {} $\\pm$ {}"*steps + " \\\\ \n\
    LFV ST $tu\\mu\\tau$  " + " & {} $\\pm$ {}"*steps + " \\\\ \n\
    LFV TT $tc\\mu\\tau$  " + " & {} $\\pm$ {}"*steps + " \\\\ \n\
    LFV TT $tu\\mu\\tau$  " + " & {} $\\pm$ {}"*steps + " \\\\ \n\
    \\ttbar dileptonic   " + " & {} $\\pm$ {}"*steps + " \\\\ \n\
    \\ttbar semileptonic " + " & {} $\\pm$ {}"*steps + " \\\\ \n\
    \\ttbar hadronic     " + " & {} $\\pm$ {}"*steps + " \\\\ \n\
    DY                  " + " & {} $\\pm$ {}"*steps + " \\\\ \n\
    Single Top          " + " & {} $\\pm$ {}"*steps + " \\\\ \n\
    W+Jets              " + " & {} $\\pm$ {}"*steps + " \\\\ \n\
    \\ttbar + X          " + " & {} $\\pm$ {}"*steps + " \\\\ \n\
    VV                  " + " & {} $\\pm$ {}"*steps + " \\\\ \n\
    QCD                 " + " & {} $\\pm$ {}"*steps + " \\\\ \n\
    Total MC            " + " & {} $\\pm$ {}"*steps + " \\\\ \n\
    DATA                " + " & {}"*steps + " \\\\ \n\
    DATA / MC           " + " & {} $\\pm$ {}"*steps + " \\\\ \n\
    S/$\sqrt{{B}}$~(ST $tc\\mu\\tau$)" + " & {}"*steps+" \\\\ \n\
    S/$\sqrt{{B}}$~(ST $tu\\mu\\tau$)" + " & {}"*steps+" \\\\ \n\
    S/$\sqrt{{B}}$~(TT $tc\\mu\\tau$)" + " & {}"*steps+" \\\\ \n\
    $\\varepsilon$~(ST $tc\\mu\\tau$)" + " & {}"*steps+" \\\\ \n\
    $\\varepsilon$~(ST $tu\\mu\\tau$)" + " & {}"*steps+" \\\\ \n\
    $\\varepsilon$~(TT $tc\\mu\\tau$)" + " & {}"*steps+" \\\\ \n"
    s=s.format(*formattedcutflow)
    b1 = "    \\end{tabular}\n"
    b2 = "    \\caption{Cutflow Table for %s data and MC samples}\n"%(run)
    b3 = "    \\label{tab:cutflow%s_%s}\n"%(run,pname)
    b4 = "\\end{table}"

    h=h1+h2+h3
    b=b1+b2+b3+b4
    script = h+s+b
    return script

run2s=["Run16","Run17","Run18","Run2"]
for run, cutflow in zip(run2s, formattedcutflows):
    if run=="Run16":
        lumi = 35.92 # fb^(-1)
    elif run=="Run17":
        lumi = 41.53 # fb^(-1)
    elif run=="Run18":
        lumi = 59.74 # fb^(-1)
    elif run=="Run2":
        lumi = 137.19 # fb^(-1) 
    x=texScript(run,cutflow)
    print(x)

datacard_path = "datacards/"+pname
if not os.path.isdir(datacard_path):
    os.makedirs(datacard_path)

def writeDatacard(run,cutflow):
    LFV = [i[0] for i in cutflow[0:12]]
    TT = np.add(np.add(cutflow[12][0],cutflow[13][0]),cutflow[14][0]).tolist()
    DY = cutflow[15][0]
    ST = cutflow[16][0]
    WJets = cutflow[17][0]
    Oth = np.add(np.add(cutflow[18][0],cutflow[19][0]),cutflow[20][0])
    DATA = cutflow[22][0]
    N=0
    for j in ['c','u']:
        for k in ['s','v','t']:
            name = j+'_'+k
            with open(datacard_path+'/datacard_countingLFV'+name+'_'+run+'.txt','w') as f_tmp:
                script="""
imax    1 number of bins
jmax    6 number of processes minus 1
kmax    * number of nuisance parameters
----------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------
bin             bin1
observation     {0:<12}
----------------------------------------------------------------------------------------------------
bin             bin1        bin1        bin1        bin1        bin1        bin1        bin1
process         lfv_st      lfv_tt      tt          dy          st          wjet        others
process         0           -1          1           2           3           4           5
rate            {1:<12.3f}{2:<12.3f}{3:<12.3f}{4:<12.3f}{5:<12.3f}{6:<12.3f}{7:<12.3f}
----------------------------------------------------------------------------------------------------
lumieff18 lnN   1.025       1.025       1.025       1.025       1.025       1.025       1.025
            """.format(DATA[steps], LFV[N][steps], LFV[N+6][steps], TT[steps], DY[steps], ST[steps], WJets[steps], Oth[steps])
                f_tmp.write(str(script))
                f_tmp.close()
            N=N+1
    return

for run, cutflow in zip(run2s, cutflows):
    writeDatacard(run,cutflow)
