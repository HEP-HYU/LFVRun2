#!/bin/basou
#### SETTINGS ####
version=skim_LFVv7
njobs=25

#### PATHs ####
mcdir19ul16pre=/data1/common/NanoAOD/v8_UL/mc/RunIISummer19UL16NanoAODAPVv2
mcdir19ul16post=/data1/common/NanoAOD/v8_UL/mc/RunIISummer19UL16NanoAODv2
mcdir19ul17=/data1/common/NanoAOD/v8_UL/mc/RunIISummer19UL17NanoAODv2
mcdir19ul18=/data1/common/NanoAOD/v8_UL/mc/RunIISummer19UL18NanoAODv2
mcdir20ul16pre=/data1/common/NanoAOD/v8_UL/mc/RunIISummer20UL16NanoAODAPVv2
mcdir20ul16post=/data1/common/NanoAOD/v8_UL/mc/RunIISummer20UL16NanoAODv2
mcdir20ul17=/data1/common/NanoAOD/v8_UL/mc/RunIISummer20UL17NanoAODv2
mcdir20ul18=/data1/common/NanoAOD/v8_UL/mc/RunIISummer20UL18NanoAODv2
tgdir16pre=/data1/common/skimmed_NanoAOD/${version}/mc/16pre
tgdir16post=/data1/common/skimmed_NanoAOD/${version}/mc/16post
logdir=/data1/common/skimmed_NanoAOD/${version}/log
mkdir -p ${tgdir}
mkdir -p ${logdir}
# 16pre
#./skimnanoaod.py -F --split ${njobs} -Y skim16pre ${mcdir20ul16pre}/DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8 ${tgdir16pre}/DYJetsToLL_M-10to50 &> ${logdir}/DYJetsToLL_M-10to50_skim16pre.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16pre ${mcdir19ul16pre}/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8 ${tgdir16pre}/DYJetsToLL_M-50_amcatnlo  &> ${logdir}/DYJetsToLL_M-50_amcatnlo_skim16pre.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16pre ${mcdir20ul16pre}/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8 ${tgdir16pre}/DYJetsToLL_M-50_madgraph &> ${logdir}/DYJetsToLL_M-50_madgraph_skim16pre.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16pre ${mcdir20ul16pre}/ST_t-channel_antitop_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8 ${tgdir16pre}/ST_t-channel_antitop &> ${logdir}/ST_t-channel_antitop_skim16pre.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16pre ${mcdir20ul16pre}/ST_t-channel_top_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8 ${tgdir16pre}/ST_t-channel_top &> ${logdir}/ST_t-channel_top_skim16pre.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16pre ${mcdir20ul16pre}/ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8 ${tgdir16pre}/ST_tW_antitop &> ${logdir}/ST_tW_antitop_skim16pre.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16pre ${mcdir20ul16pre}/ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8 ${tgdir16pre}/ST_tW_top &> ${logdir}/ST_tW_top_skim16pre.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16pre ${mcdir20ul16pre}/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8 ${tgdir16pre}/TTTo2L2Nu &> ${logdir}/TTTo2L2Nu_skim16pre.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16pre ${mcdir20ul16pre}/TTToHadronic_TuneCP5_13TeV-powheg-pythia8 ${tgdir16pre}/TTToHadronic &> ${logdir}/TTToHadronic_skim16pre.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16pre ${mcdir20ul16pre}/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8 ${tgdir16pre}/TTToSemiLeptonic &> ${logdir}/TTToSemiLeptonic_skim16pre.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16pre ${mcdir20ul16pre}/TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8 ${tgdir16pre}/TTWJetsToLNu &> ${logdir}/TTWJetsToLNu_skim16pre.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16pre ${mcdir20ul16pre}/TTWJetsToQQ_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8 ${tgdir16pre}/TTWJetsToQQ &> ${logdir}/TTWJetsToQQ_skim16pre.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16pre ${mcdir20ul16pre}/TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8 ${tgdir16pre}/TTZToLLNuNu &> ${logdir}/TTZToLLNuNu_skim16pre.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16pre ${mcdir20ul16pre}/TTZToQQ_TuneCP5_13TeV-amcatnlo-pythia8 ${tgdir16pre}/TTZToQQ &> ${logdir}/TTZToQQ_skim16pre.out
#./skimnanoaod.py -F --split ${njobs} -Y wjetskim16pre ${mcdir20ul16pre}/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8 ${tgdir16pre}/WJetsToLNu_inclHT100 &> ${logdir}/WJetsToLNu_inclHT100_skim16pre.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16pre ${mcdir20ul16pre}/WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8 ${tgdir16pre}/WJetsToLNu_HT-100To200 &> ${logdir}/WJetsToLNu_HT-100To200_skim16pre.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16pre ${mcdir20ul16pre}/WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8 ${tgdir16pre}/WJetsToLNu_HT-200To400 &> ${logdir}/WJetsToLNu_HT-200To400_skim16pre.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16pre ${mcdir20ul16pre}/WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8 ${tgdir16pre}/WJetsToLNu_HT-400To600 &> ${logdir}/WJetsToLNu_HT-400To600_skim16pre.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16pre ${mcdir20ul16pre}/WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8 ${tgdir16pre}/WJetsToLNu_HT-600To800 &> ${logdir}/WJetsToLNu_HT-600To800_skim16pre.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16pre ${mcdir20ul16pre}/WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8 ${tgdir16pre}/WJetsToLNu_HT-800To1200 &> ${logdir}/WJetsToLNu_HT-800To1200_skim16pre.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16pre ${mcdir20ul16pre}/WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8 ${tgdir16pre}/WJetsToLNu_HT-1200To2500 &> ${logdir}/WJetsToLNu_HT-1200To2500_skim16pre.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16pre ${mcdir20ul16pre}/WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM-pythia8 ${tgdir16pre}/WJetsToLNu_HT-2500ToInf &> ${logdir}/WJetsToLNu_HT-2500ToInf_skim16pre.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16pre ${mcdir20ul16pre}/WW_TuneCP5_13TeV-pythia8 ${tgdir16pre}/WW &> ${logdir}/WW_skim16pre.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16pre ${mcdir20ul16pre}/WZ_TuneCP5_13TeV-pythia8 ${tgdir16pre}/WZ &> ${logdir}/WZ_skim16pre.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16pre ${mcdir20ul16pre}/ZZ_TuneCP5_13TeV-pythia8 ${tgdir16pre}/ZZ &> ${logdir}/ZZ_skim16pre.out

# 16post
#./skimnanoaod.py -F --split ${njobs} -Y skim16post ${mcdir20ul16post}/DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8 ${tgdir16post}/DYJetsToLL_M-10to50 &> ${logdir}/DYJetsToLL_M-10to50_skim16post.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16post ${mcdir20ul16post}/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8 ${tgdir16post}/DYJetsToLL_M-50_amcatnlo  &> ${logdir}/DYJetsToLL_M-50_amcatnlo_skim16post.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16post ${mcdir20ul16post}/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8 ${tgdir16post}/DYJetsToLL_M-50_madgraph &> ${logdir}/DYJetsToLL_M-50_madgraph_skim16post.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16post ${mcdir20ul16post}/ST_t-channel_antitop_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8 ${tgdir16post}/ST_t-channel_antitop &> ${logdir}/ST_t-channel_antitop_skim16post.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16post ${mcdir20ul16post}/ST_t-channel_top_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8 ${tgdir16post}/ST_t-channel_top &> ${logdir}/ST_t-channel_top_skim16post.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16post ${mcdir20ul16post}/ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8 ${tgdir16post}/ST_tW_antitop &> ${logdir}/ST_tW_antitop_skim16post.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16post ${mcdir20ul16post}/ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8 ${tgdir16post}/ST_tW_top &> ${logdir}/ST_tW_top_skim16post.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16post ${mcdir20ul16post}/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8 ${tgdir16post}/TTTo2L2Nu &> ${logdir}/TTTo2L2Nu_skim16post.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16post ${mcdir20ul16post}/TTToHadronic_TuneCP5_13TeV-powheg-pythia8 ${tgdir16post}/TTToHadronic &> ${logdir}/TTToHadronic_skim16post.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16post ${mcdir20ul16post}/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8 ${tgdir16post}/TTToSemiLeptonic &> ${logdir}/TTToSemiLeptonic_skim16post.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16post ${mcdir20ul16post}/TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8 ${tgdir16post}/TTWJetsToLNu &> ${logdir}/TTWJetsToLNu_skim16post.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16post ${mcdir20ul16post}/TTWJetsToQQ_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8 ${tgdir16post}/TTWJetsToQQ &> ${logdir}/TTWJetsToQQ_skim16post.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16post ${mcdir20ul16post}/TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8 ${tgdir16post}/TTZToLLNuNu &> ${logdir}/TTZToLLNuNu_skim16post.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16post ${mcdir20ul16post}/TTZToQQ_TuneCP5_13TeV-amcatnlo-pythia8 ${tgdir16post}/TTZToQQ &> ${logdir}/TTZToQQ_skim16post.out
#./skimnanoaod.py -F --split ${njobs} -Y wjetskim16post ${mcdir20ul16post}/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8 ${tgdir16post}/WJetsToLNu_inclHT100 &> ${logdir}/WJetsToLNu_inclHT100_skim16post.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16post ${mcdir20ul16post}/WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8 ${tgdir16post}/WJetsToLNu_HT-100To200 &> ${logdir}/WJetsToLNu_HT-100To200_skim16post.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16post ${mcdir20ul16post}/WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8 ${tgdir16post}/WJetsToLNu_HT-200To400 &> ${logdir}/WJetsToLNu_HT-200To400_skim16post.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16post ${mcdir20ul16post}/WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8 ${tgdir16post}/WJetsToLNu_HT-400To600 &> ${logdir}/WJetsToLNu_HT-400To600_skim16post.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16post ${mcdir20ul16post}/WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8 ${tgdir16post}/WJetsToLNu_HT-600To800 &> ${logdir}/WJetsToLNu_HT-600To800_skim16post.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16post ${mcdir20ul16post}/WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8 ${tgdir16post}/WJetsToLNu_HT-800To1200 &> ${logdir}/WJetsToLNu_HT-800To1200_skim16post.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16post ${mcdir20ul16post}/WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8 ${tgdir16post}/WJetsToLNu_HT-1200To2500 &> ${logdir}/WJetsToLNu_HT-1200To2500_skim16post.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16post ${mcdir20ul16post}/WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM-pythia8 ${tgdir16post}/WJetsToLNu_HT-2500ToInf &> ${logdir}/WJetsToLNu_HT-2500ToInf_skim16post.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16post ${mcdir20ul16post}/WW_TuneCP5_13TeV-pythia8 ${tgdir16post}/WW &> ${logdir}/WW_skim16post.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16post ${mcdir20ul16post}/WZ_TuneCP5_13TeV-pythia8 ${tgdir16post}/WZ &> ${logdir}/WZ_skim16post.out
#./skimnanoaod.py -F --split ${njobs} -Y skim16post ${mcdir20ul16post}/ZZ_TuneCP5_13TeV-pythia8 ${tgdir16post}/ZZ &> ${logdir}/ZZ_skim16post.out

./skimnanoaod.py -F --split ${njobs} -Y skim16post ${mcdir20ul16post}/ST_LFV_TCMuTau_Scalar_TuneCP5_13TeV-madgraph-pythia8 ${tgdir16post}/ST_LFV_TCMuTau_Scalar &> ${logdir}/ST_LFV_TCMuTau_Scalar_skim16post.out
./skimnanoaod.py -F --split ${njobs} -Y skim16post ${mcdir20ul16post}/ST_LFV_TCMuTau_Vector_TuneCP5_13TeV-madgraph-pythia8 ${tgdir16post}/ST_LFV_TCMuTau_Vector &> ${logdir}/ST_LFV_TCMuTau_Vector_skim16post.out
./skimnanoaod.py -F --split ${njobs} -Y skim16post ${mcdir20ul16post}/ST_LFV_TCMuTau_Tensor_TuneCP5_13TeV-madgraph-pythia8 ${tgdir16post}/ST_LFV_TCMuTau_Tensor &> ${logdir}/ST_LFV_TCMuTau_Tensor_skim16post.out
./skimnanoaod.py -F --split ${njobs} -Y skim16post ${mcdir20ul16post}/ST_LFV_TUMuTau_Scalar_TuneCP5_13TeV-madgraph-pythia8 ${tgdir16post}/ST_LFV_TUMuTau_Scalar &> ${logdir}/ST_LFV_TUMuTau_Scalar_skim16post.out
./skimnanoaod.py -F --split ${njobs} -Y skim16post ${mcdir20ul16post}/ST_LFV_TUMuTau_Vector_TuneCP5_13TeV-madgraph-pythia8 ${tgdir16post}/ST_LFV_TUMuTau_Vector &> ${logdir}/ST_LFV_TUMuTau_Vector_skim16post.out
./skimnanoaod.py -F --split ${njobs} -Y skim16post ${mcdir20ul16post}/ST_LFV_TUMuTau_Tensor_TuneCP5_13TeV-madgraph-pythia8 ${tgdir16post}/ST_LFV_TUMuTau_Tensor &> ${logdir}/ST_LFV_TUMuTau_Tensor_skim16post.out
./skimnanoaod.py -F --split ${njobs} -Y skim16post ${mcdir20ul16post}/TT_LFV_TToCMuTau_Scalar_TuneCP5_13TeV-madgraph-pythia8 ${tgdir16post}/TT_LFV_TToCMuTau_Scalar &> ${logdir}/TT_LFV_TToCMuTau_Scalar_skim16post.out
./skimnanoaod.py -F --split ${njobs} -Y skim16post ${mcdir20ul16post}/TT_LFV_TToCMuTau_Vector_TuneCP5_13TeV-madgraph-pythia8 ${tgdir16post}/TT_LFV_TToCMuTau_Vector &> ${logdir}/TT_LFV_TToCMuTau_Vector_skim16post.out
./skimnanoaod.py -F --split ${njobs} -Y skim16post ${mcdir20ul16post}/TT_LFV_TToCMuTau_Tensor_TuneCP5_13TeV-madgraph-pythia8 ${tgdir16post}/TT_LFV_TToCMuTau_Tensor &> ${logdir}/TT_LFV_TToCMuTau_Tensor_skim16post.out
./skimnanoaod.py -F --split ${njobs} -Y skim16post ${mcdir20ul16post}/TT_LFV_TToUMuTau_Scalar_TuneCP5_13TeV-madgraph-pythia8 ${tgdir16post}/TT_LFV_TToUMuTau_Scalar &> ${logdir}/TT_LFV_TToUMuTau_Scalar_skim16post.out
./skimnanoaod.py -F --split ${njobs} -Y skim16post ${mcdir20ul16post}/TT_LFV_TToUMuTau_Vector_TuneCP5_13TeV-madgraph-pythia8 ${tgdir16post}/TT_LFV_TToUMuTau_Vector &> ${logdir}/TT_LFV_TToUMuTau_Vector_skim16post.out
./skimnanoaod.py -F --split ${njobs} -Y skim16post ${mcdir20ul16post}/TT_LFV_TToUMuTau_Tensor_TuneCP5_13TeV-madgraph-pythia8 ${tgdir16post}/TT_LFV_TToUMuTau_Tensor &> ${logdir}/TT_LFV_TToUMuTau_Tensor_skim16post.out
