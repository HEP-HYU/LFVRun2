#!/bin/bash
#### SETTINGS ####
version=skim_LFVv7
njobs=25

#### PATHs ####
mcdir19ul17=/data1/common/NanoAOD/v8_UL/mc/RunIISummer19UL17NanoAODv2
mcdir19ul18=/data1/common/NanoAOD/v8_UL/mc/RunIISummer19UL18NanoAODv2
mcdir20ul17=/data1/common/NanoAOD/v8_UL/mc/RunIISummer20UL17NanoAODv2
mcdir20ul18=/data1/common/NanoAOD/v8_UL/mc/RunIISummer20UL18NanoAODv2
tgdir17=/data1/common/skimmed_NanoAOD/${version}/mc/17
tgdir18=/data1/common/skimmed_NanoAOD/${version}/mc/18
logdir=/data1/common/skimmed_NanoAOD/${version}/log
#mkdir -p "${tgdir}"
#mkdir -p "${logdir}"
# 17
#./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir19ul17}/QCD_Pt-15to20_MuEnrichedPt5_TuneCP5_13TeV_pythia8 ${tgdir17}/QCD_Pt-15to20_MuEnrichedPt5 &> ${logdir}/QCD_Pt-15to20_MuEnrichedPt5_skim17.out
#./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir19ul17}/QCD_Pt-20to30_MuEnrichedPt5_TuneCP5_13TeV_pythia8 ${tgdir17}/QCD_Pt-20to30_MuEnrichedPt5 &> ${logdir}/QCD_Pt-20to30_MuEnrichedPt5_skim17.out
#./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir19ul17}/QCD_Pt-30to50_MuEnrichedPt5_TuneCP5_13TeV_pythia8 ${tgdir17}/QCD_Pt-30to50_MuEnrichedPt5 &> ${logdir}/QCD_Pt-30to50_MuEnrichedPt5_skim17.out
#./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir19ul17}/QCD_Pt-50to80_MuEnrichedPt5_TuneCP5_13TeV_pythia8 ${tgdir17}/QCD_Pt-50to80_MuEnrichedPt5 &> ${logdir}/QCD_Pt-50to80_MuEnrichedPt5_skim17.out
#./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir19ul17}/QCD_Pt-80to120_MuEnrichedPt5_TuneCP5_13TeV_pythia8 ${tgdir17}/QCD_Pt-80to120_MuEnrichedPt5 &> ${logdir}/QCD_Pt-80to120_MuEnrichedPt5_skim17.out
#./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir19ul17}/QCD_Pt-120to170_MuEnrichedPt5_TuneCP5_13TeV_pythia8 ${tgdir17}/QCD_Pt-120to170_MuEnrichedPt5 &> ${logdir}/QCD_Pt-120to170_MuEnrichedPt5_skim17.out
#./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir19ul17}/QCD_Pt-170to300_MuEnrichedPt5_TuneCP5_13TeV_pythia8 ${tgdir17}/QCD_Pt-170to300_MuEnrichedPt5 &> ${logdir}/QCD_Pt-170to300_MuEnrichedPt5_skim17.out
#./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir19ul17}/QCD_Pt-300to470_MuEnrichedPt5_TuneCP5_13TeV_pythia8 ${tgdir17}/QCD_Pt-300t470_MuEnrichedPt5 &> ${logdir}/QCD_Pt-300to470_MuEnrichedPt5_skim17.out
#./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir19ul17}/QCD_Pt-470to600_MuEnrichedPt5_TuneCP5_13TeV_pythia8 ${tgdir17}/QCD_Pt-470to600_MuEnrichedPt5 &> ${logdir}/QCD_Pt-470to600_MuEnrichedPt5_skim17.out
#./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir19ul17}/QCD_Pt-600to800_MuEnrichedPt5_TuneCP5_13TeV_pythia8 ${tgdir17}/QCD_Pt-600to800_MuEnrichedPt5 &> ${logdir}/QCD_Pt-600to800_MuEnrichedPt5_skim17.out
#./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir19ul17}/QCD_Pt-800to1000_MuEnrichedPt5_TuneCP5_13TeV_pythia8 ${tgdir17}/QCD_Pt-800to1000_MuEnrichedPt5 &> ${logdir}/QCD_Pt-800to1000_MuEnrichedPt5_skim17.out
#./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir19ul17}/QCD_Pt-1000toInf_MuEnrichedPt5_TuneCP5_13TeV_pythia8 ${tgdir17}/QCD_Pt-1000toInf_MuEnrichedPt5 &> ${logdir}/QCD_Pt-1000toInf_MuEnrichedPt5_skim17.out
#./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir19ul17}/ST_t-channel_antitop_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8 ${tgdir17}/ST_t-channel_antitop &> ${logdir}/ST_t-channel_antitop_skim17.out
#./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir19ul17}/ST_t-channel_top_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8 ${tgdir17}/ST_t-channel_top &> ${logdir}/ST_t-channel_top_skim17.out
#./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir20ul17}/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8 ${tgdir17}/DYJetsToLL_M-50_amcatnlo20ul  &> ${logdir}/DYJetsToLL_M-50_amcatnlo_skim17.out
#./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir20ul17}/DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8 ${tgdir17}/DYJetsToLL_M-10to50 &> ${logdir}/DYJetsToLL_M-10to50_skim17.out
#./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir20ul17}/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8 ${tgdir17}/DYJetsToLL_M-50_madgraph &> ${logdir}/DYJetsToLL_M-50_madgraph_skim17.out
#./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir20ul17}/ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8 ${tgdir17}/ST_tW_antitop &> ${logdir}/ST_tW_antitop_skim17.out
#./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir20ul17}/ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8 ${tgdir17}/ST_tW_top &> ${logdir}/ST_tW_top_skim17.out
#./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir20ul17}/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8 ${tgdir17}/TTTo2L2Nu &> ${logdir}/TTTo2L2Nu_skim17.out
#./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir20ul17}/TTToHadronic_TuneCP5_13TeV-powheg-pythia8 ${tgdir17}/TTToHadronic &> ${logdir}/TTToHadronic_skim17.out
#./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir20ul17}/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8 ${tgdir17}/TTToSemiLeptonic &> ${logdir}/TTToSemiLeptonic_skim17.out
#./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir20ul17}/TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8 ${tgdir17}/TTWJetsToLNu &> ${logdir}/TTWJetsToLNu_skim17.out
#./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir20ul17}/TTWJetsToQQ_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8 ${tgdir17}/TTWJetsToQQ &> ${logdir}/TTWJetsToQQ_skim17.out
#./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir20ul17}/TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8 ${tgdir17}/TTZToLLNuNu &> ${logdir}/TTZToLLNuNu_skim17.out
#./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir20ul17}/TTZToQQ_TuneCP5_13TeV-amcatnlo-pythia8 ${tgdir17}/TTZToQQ &> ${logdir}/TTZToQQ_skim17.out
#./skimnanoaod.py -F --split ${njobs} -Y wjetskim17 ${mcdir20ul17}/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8 ${tgdir17}/WJetsToLNu_inclHT100 &> ${logdir}/WJetsToLNu_inclHT100_skim17.out
#./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir20ul17}/WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8 ${tgdir17}/WJetsToLNu_HT-100To200 &> ${logdir}/WJetsToLNu_HT-100To200_skim17.out
#./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir20ul17}/WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8 ${tgdir17}/WJetsToLNu_HT-200To400 &> ${logdir}/WJetsToLNu_HT-200To400_skim17.out
#./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir20ul17}/WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8 ${tgdir17}/WJetsToLNu_HT-400To600 &> ${logdir}/WJetsToLNu_HT-400To600_skim17.out
#./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir20ul17}/WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8 ${tgdir17}/WJetsToLNu_HT-600To800 &> ${logdir}/WJetsToLNu_HT-600To800_skim17.out
#./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir20ul17}/WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8 ${tgdir17}/WJetsToLNu_HT-800To1200 &> ${logdir}/WJetsToLNu_HT-800To1200_skim17.out
#./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir20ul17}/WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8 ${tgdir17}/WJetsToLNu_HT-1200To2500 &> ${logdir}/WJetsToLNu_HT-1200To2500_skim17.out
#./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir20ul17}/WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM-pythia8 ${tgdir17}/WJetsToLNu_HT-2500ToInf &> ${logdir}/WJetsToLNu_HT-2500ToInf_skim17.out
#./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir20ul17}/WW_TuneCP5_13TeV-pythia8 ${tgdir17}/WW &> ${logdir}/WW_skim17.out
#./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir20ul17}/WZ_TuneCP5_13TeV-pythia8 ${tgdir17}/WZ &> ${logdir}/WZ_skim17.out
#./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir20ul17}/ZZ_TuneCP5_13TeV-pythia8 ${tgdir17}/ZZ &> ${logdir}/ZZ_skim17.out
## 18
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir19ul18}/QCD_Pt-15to20_MuEnrichedPt5_TuneCP5_13TeV_pythia8 ${tgdir18}/QCD_Pt-15to20_MuEnrichedPt5 &> ${logdir}/QCD_Pt-15to20_MuEnrichedPt5_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir19ul18}/QCD_Pt-20to30_MuEnrichedPt5_TuneCP5_13TeV_pythia8 ${tgdir18}/QCD_Pt-20to30_MuEnrichedPt5 &> ${logdir}/QCD_Pt-20to30_MuEnrichedPt5_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir19ul18}/QCD_Pt-30to50_MuEnrichedPt5_TuneCP5_13TeV_pythia8 ${tgdir18}/QCD_Pt-30to50_MuEnrichedPt5 &> ${logdir}/QCD_Pt-30to50_MuEnrichedPt5_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir19ul18}/QCD_Pt-50to80_MuEnrichedPt5_TuneCP5_13TeV_pythia8 ${tgdir18}/QCD_Pt-50to80_MuEnrichedPt5 &> ${logdir}/QCD_Pt-50to80_MuEnrichedPt5_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir19ul18}/QCD_Pt-80to120_MuEnrichedPt5_TuneCP5_13TeV_pythia8 ${tgdir18}/QCD_Pt-80to120_MuEnrichedPt5 &> ${logdir}/QCD_Pt-80to120_MuEnrichedPt5_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir19ul18}/QCD_Pt-120to170_MuEnrichedPt5_TuneCP5_13TeV_pythia8 ${tgdir18}/QCD_Pt-120to170_MuEnrichedPt5 &> ${logdir}/QCD_Pt-120to170_MuEnrichedPt5_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir19ul18}/QCD_Pt-170to300_MuEnrichedPt5_TuneCP5_13TeV_pythia8 ${tgdir18}/QCD_Pt-170to300_MuEnrichedPt5 &> ${logdir}/QCD_Pt-170to300_MuEnrichedPt5_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir19ul18}/QCD_Pt-300to470_MuEnrichedPt5_TuneCP5_13TeV_pythia8 ${tgdir18}/QCD_Pt-300t470_MuEnrichedPt5 &> ${logdir}/QCD_Pt-300to470_MuEnrichedPt5_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir19ul18}/QCD_Pt-470to600_MuEnrichedPt5_TuneCP5_13TeV_pythia8 ${tgdir18}/QCD_Pt-470to600_MuEnrichedPt5 &> ${logdir}/QCD_Pt-470to600_MuEnrichedPt5_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir19ul18}/QCD_Pt-600to800_MuEnrichedPt5_TuneCP5_13TeV_pythia8 ${tgdir18}/QCD_Pt-600to800_MuEnrichedPt5 &> ${logdir}/QCD_Pt-600to800_MuEnrichedPt5_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir19ul18}/QCD_Pt-800to1000_MuEnrichedPt5_TuneCP5_13TeV_pythia8 ${tgdir18}/QCD_Pt-800to1000_MuEnrichedPt5 &> ${logdir}/QCD_Pt-800to1000_MuEnrichedPt5_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir19ul18}/QCD_Pt-1000toInf_MuEnrichedPt5_TuneCP5_13TeV_pythia8 ${tgdir18}/QCD_Pt-1000toInf_MuEnrichedPt5 &> ${logdir}/QCD_Pt-1000toInf_MuEnrichedPt5_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir19ul18}/ST_t-channel_antitop_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8 ${tgdir18}/ST_t-channel_antitop &> ${logdir}/ST_t-channel_antitop_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir19ul18}/ST_t-channel_top_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8 ${tgdir18}/ST_t-channel_top &> ${logdir}/ST_t-channel_top_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul18}/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8 ${tgdir18}/DYJetsToLL_M-50_amcatnlo20ul  &> ${logdir}/DYJetsToLL_M-50_amcatnlo_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul18}/DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8 ${tgdir18}/DYJetsToLL_M-10to50 &> ${logdir}/DYJetsToLL_M-10to50_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul18}/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8 ${tgdir18}/DYJetsToLL_M-50_madgraph &> ${logdir}/DYJetsToLL_M-50_madgraph_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul18}/ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8 ${tgdir18}/ST_tW_antitop &> ${logdir}/ST_tW_antitop_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul18}/ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8 ${tgdir18}/ST_tW_top &> ${logdir}/ST_tW_top_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul18}/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8 ${tgdir18}/TTTo2L2Nu &> ${logdir}/TTTo2L2Nu_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul18}/TTToHadronic_TuneCP5_13TeV-powheg-pythia8 ${tgdir18}/TTToHadronic &> ${logdir}/TTToHadronic_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul18}/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8 ${tgdir18}/TTToSemiLeptonic &> ${logdir}/TTToSemiLeptonic_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul18}/TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8 ${tgdir18}/TTWJetsToLNu &> ${logdir}/TTWJetsToLNu_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul18}/TTWJetsToQQ_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8 ${tgdir18}/TTWJetsToQQ &> ${logdir}/TTWJetsToQQ_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul18}/TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8 ${tgdir18}/TTZToLLNuNu &> ${logdir}/TTZToLLNuNu_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul18}/TTZToQQ_TuneCP5_13TeV-amcatnlo-pythia8 ${tgdir18}/TTZToQQ &> ${logdir}/TTZToQQ_skim18.out
./skimnanoaod.py -F --split ${njobs} -Y wjetskim18 ${mcdir20ul18}/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8 ${tgdir18}/WJetsToLNu_inclHT100 &> ${logdir}/WJetsToLNu_inclHT100_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul18}/WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8 ${tgdir18}/WJetsToLNu_HT-100To200 &> ${logdir}/WJetsToLNu_HT-100To200_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul18}/WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8 ${tgdir18}/WJetsToLNu_HT-200To400 &> ${logdir}/WJetsToLNu_HT-200To400_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul18}/WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8 ${tgdir18}/WJetsToLNu_HT-400To600 &> ${logdir}/WJetsToLNu_HT-400To600_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul18}/WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8 ${tgdir18}/WJetsToLNu_HT-600To800 &> ${logdir}/WJetsToLNu_HT-600To800_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul18}/WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8 ${tgdir18}/WJetsToLNu_HT-800To1200 &> ${logdir}/WJetsToLNu_HT-800To1200_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul18}/WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8 ${tgdir18}/WJetsToLNu_HT-1200To2500 &> ${logdir}/WJetsToLNu_HT-1200To2500_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul18}/WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM-pythia8 ${tgdir18}/WJetsToLNu_HT-2500ToInf &> ${logdir}/WJetsToLNu_HT-2500ToInf_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul18}/WW_TuneCP5_13TeV-pythia8 ${tgdir18}/WW &> ${logdir}/WW_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul18}/WZ_TuneCP5_13TeV-pythia8 ${tgdir18}/WZ &> ${logdir}/WZ_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul18}/ZZ_TuneCP5_13TeV-pythia8 ${tgdir18}/ZZ &> ${logdir}/ZZ_skim18.out
#
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul18}/LFV_ST_TCMuTau_Scalar_NANO ${tgdir18}/LFV_ST_TCMuTau_Scalar &> ${logdir}/LFV_ST_TCMuTau_Scalar_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul18}/LFV_ST_TCMuTau_Vector_NANO ${tgdir18}/LFV_ST_TCMuTau_Vector &> ${logdir}/LFV_ST_TCMuTau_Vector_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul18}/LFV_ST_TCMuTau_Tensor_NANO ${tgdir18}/LFV_ST_TCMuTau_Tensor &> ${logdir}/LFV_ST_TCMuTau_Tensor_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul18}/LFV_ST_TUMuTau_Scalar_NANO ${tgdir18}/LFV_ST_TUMuTau_Scalar &> ${logdir}/LFV_ST_TUMuTau_Scalar_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul18}/LFV_ST_TUMuTau_Vector_NANO ${tgdir18}/LFV_ST_TUMuTau_Vector &> ${logdir}/LFV_ST_TUMuTau_Vector_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul18}/LFV_ST_TUMuTau_Tensor_NANO ${tgdir18}/LFV_ST_TUMuTau_Tensor &> ${logdir}/LFV_ST_TUMuTau_Tensor_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul18}/LFV_TT_TToCMuTau_Scalar_NANO ${tgdir18}/LFV_TT_TToCMuTau_Scalar &> ${logdir}/LFV_TT_TToCMuTau_Scalar_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul18}/LFV_TT_TToCMuTau_Vector_NANO ${tgdir18}/LFV_TT_TToCMuTau_Vector &> ${logdir}/LFV_TT_TToCMuTau_Vector_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul18}/LFV_TT_TToCMuTau_Tensor_NANO ${tgdir18}/LFV_TT_TToCMuTau_Tensor &> ${logdir}/LFV_TT_TToCMuTau_Tensor_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul18}/LFV_TT_TToUMuTau_Scalar_NANO ${tgdir18}/LFV_TT_TToUMuTau_Scalar &> ${logdir}/LFV_TT_TToUMuTau_Scalar_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul18}/LFV_TT_TToUMuTau_Vector_NANO ${tgdir18}/LFV_TT_TToUMuTau_Vector &> ${logdir}/LFV_TT_TToUMuTau_Vector_skim18.out
#./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul18}/LFV_TT_TToUMuTau_Tensor_NANO ${tgdir18}/LFV_TT_TToUMuTau_Tensor &> ${logdir}/LFV_TT_TToUMuTau_Tensor_skim18.out
#
#./skimnanoaod.py -F --split ${njobs} -Y skim16post ${mcdir20ul16post}/ ${tgdir16post}/ &> ${logdir}/_skim16post.out
