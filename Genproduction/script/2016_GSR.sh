#!/bin/bash
export SCRAM_ARCH=slc6_amd64_gcc530
source /cvmfs/cms.cern.ch/cmsset_default.sh
if [ -r CMSSW_8_0_31/src ] ; then 
 echo release CMSSW_8_0_31 already exists
else
scram p CMSSW CMSSW_8_0_31
fi
cd CMSSW_8_0_31/src
eval `scram runtime -sh`

scram b
cd ../../
cmsDriver.py step1 --filein file:LQ_2016LHEGS_v1.root --fileout file:LQ_2016_step1.root  --pileup_input "dbs:/Neutrino_E-10_gun/RunIISpring15PrePremix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v2-v2/GEN-SIM-DIGI-RAW" --mc --eventcontent PREMIXRAW --datatier GEN-SIM-RAW --conditions 80X_mcRun2_asymptotic_2016_TrancheIV_v6 --step DIGIPREMIX_S2,DATAMIX,L1,DIGI2RAW,HLT:@frozen2016 --nThreads 8 --datamix PreMix --era Run2_2016 --python_filename LQ_2016_step1_cfg.py --no_exec --customise Configuration/DataProcessing/Utils.addMonitoring -n 1000 || exit $? ; 

cmsDriver.py step2 --filein file:LQ_2016_step1.root --fileout file:LQ_2016_step2.root --mc --eventcontent AODSIM --runUnscheduled --datatier AODSIM --conditions 80X_mcRun2_asymptotic_2016_TrancheIV_v6 --step RAW2DIGI,RECO,EI --nThreads 8 --era Run2_2016 --python_filename LQ_2016_step2_cfg.py --no_exec --customise Configuration/DataProcessing/Utils.addMonitoring -n 1000 || exit $? ; 


