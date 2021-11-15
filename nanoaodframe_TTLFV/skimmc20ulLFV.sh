#!/bin/basou
#### SETTINGS ####
version=skim_LFVv7
njobs=50

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
tgdir17=/data1/common/skimmed_NanoAOD/${version}/mc/17
tgdir18=/data1/common/skimmed_NanoAOD/${version}/mc/18
logdir=/data1/common/skimmed_NanoAOD/${version}/log
mkdir -p ${tgdir}
mkdir -p ${logdir}
# 16pre
./skimnanoaod.py -F --split ${njobs} -Y skim16pre ${mcdir20ul16post}/ST_LFV_TCMuTau_Scalar_TuneCP5_13TeV-madgraph-pythia8 ${tgdir16pre}/ST_LFV_TCMuTau_Scalar &> ${logdir}/ST_LFV_TCMuTau_Scalar_skim16pre.out
./skimnanoaod.py -F --split ${njobs} -Y skim16pre ${mcdir20ul16post}/ST_LFV_TCMuTau_Vector_TuneCP5_13TeV-madgraph-pythia8 ${tgdir16pre}/ST_LFV_TCMuTau_Vector &> ${logdir}/ST_LFV_TCMuTau_Vector_skim16pre.out
./skimnanoaod.py -F --split ${njobs} -Y skim16pre ${mcdir20ul16post}/ST_LFV_TCMuTau_Tensor_TuneCP5_13TeV-madgraph-pythia8 ${tgdir16pre}/ST_LFV_TCMuTau_Tensor &> ${logdir}/ST_LFV_TCMuTau_Tensor_skim16pre.out
./skimnanoaod.py -F --split ${njobs} -Y skim16pre ${mcdir20ul16post}/ST_LFV_TUMuTau_Scalar_TuneCP5_13TeV-madgraph-pythia8 ${tgdir16pre}/ST_LFV_TUMuTau_Scalar &> ${logdir}/ST_LFV_TUMuTau_Scalar_skim16pre.out
./skimnanoaod.py -F --split ${njobs} -Y skim16pre ${mcdir20ul16post}/ST_LFV_TUMuTau_Vector_TuneCP5_13TeV-madgraph-pythia8 ${tgdir16pre}/ST_LFV_TUMuTau_Vector &> ${logdir}/ST_LFV_TUMuTau_Vector_skim16pre.out
./skimnanoaod.py -F --split ${njobs} -Y skim16pre ${mcdir20ul16post}/ST_LFV_TUMuTau_Tensor_TuneCP5_13TeV-madgraph-pythia8 ${tgdir16pre}/ST_LFV_TUMuTau_Tensor &> ${logdir}/ST_LFV_TUMuTau_Tensor_skim16pre.out
./skimnanoaod.py -F --split ${njobs} -Y skim16pre ${mcdir20ul16post}/TT_LFV_TToCMuTau_Scalar_TuneCP5_13TeV-madgraph-pythia8 ${tgdir16pre}/TT_LFV_TToCMuTau_Scalar &> ${logdir}/TT_LFV_TToCMuTau_Scalar_skim16pre.out
./skimnanoaod.py -F --split ${njobs} -Y skim16pre ${mcdir20ul16post}/TT_LFV_TToCMuTau_Vector_TuneCP5_13TeV-madgraph-pythia8 ${tgdir16pre}/TT_LFV_TToCMuTau_Vector &> ${logdir}/TT_LFV_TToCMuTau_Vector_skim16pre.out
./skimnanoaod.py -F --split ${njobs} -Y skim16pre ${mcdir20ul16post}/TT_LFV_TToCMuTau_Tensor_TuneCP5_13TeV-madgraph-pythia8 ${tgdir16pre}/TT_LFV_TToCMuTau_Tensor &> ${logdir}/TT_LFV_TToCMuTau_Tensor_skim16pre.out
./skimnanoaod.py -F --split ${njobs} -Y skim16pre ${mcdir20ul16post}/TT_LFV_TToUMuTau_Scalar_TuneCP5_13TeV-madgraph-pythia8 ${tgdir16pre}/TT_LFV_TToUMuTau_Scalar &> ${logdir}/TT_LFV_TToUMuTau_Scalar_skim16pre.out
./skimnanoaod.py -F --split ${njobs} -Y skim16pre ${mcdir20ul16post}/TT_LFV_TToUMuTau_Vector_TuneCP5_13TeV-madgraph-pythia8 ${tgdir16pre}/TT_LFV_TToUMuTau_Vector &> ${logdir}/TT_LFV_TToUMuTau_Vector_skim16pre.out
./skimnanoaod.py -F --split ${njobs} -Y skim16pre ${mcdir20ul16post}/TT_LFV_TToUMuTau_Tensor_TuneCP5_13TeV-madgraph-pythia8 ${tgdir16pre}/TT_LFV_TToUMuTau_Tensor &> ${logdir}/TT_LFV_TToUMuTau_Tensor_skim16pre.out
# 16post
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

./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir20ul16post}/ST_LFV_TCMuTau_Scalar_TuneCP5_13TeV-madgraph-pythia8 ${tgdir17}/ST_LFV_TCMuTau_Scalar &> ${logdir}/ST_LFV_TCMuTau_Scalar_skim17.out
./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir20ul16post}/ST_LFV_TCMuTau_Vector_TuneCP5_13TeV-madgraph-pythia8 ${tgdir17}/ST_LFV_TCMuTau_Vector &> ${logdir}/ST_LFV_TCMuTau_Vector_skim17.out
./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir20ul16post}/ST_LFV_TCMuTau_Tensor_TuneCP5_13TeV-madgraph-pythia8 ${tgdir17}/ST_LFV_TCMuTau_Tensor &> ${logdir}/ST_LFV_TCMuTau_Tensor_skim17.out
./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir20ul16post}/ST_LFV_TUMuTau_Scalar_TuneCP5_13TeV-madgraph-pythia8 ${tgdir17}/ST_LFV_TUMuTau_Scalar &> ${logdir}/ST_LFV_TUMuTau_Scalar_skim17.out
./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir20ul16post}/ST_LFV_TUMuTau_Vector_TuneCP5_13TeV-madgraph-pythia8 ${tgdir17}/ST_LFV_TUMuTau_Vector &> ${logdir}/ST_LFV_TUMuTau_Vector_skim17.out
./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir20ul16post}/ST_LFV_TUMuTau_Tensor_TuneCP5_13TeV-madgraph-pythia8 ${tgdir17}/ST_LFV_TUMuTau_Tensor &> ${logdir}/ST_LFV_TUMuTau_Tensor_skim17.out
./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir20ul16post}/TT_LFV_TToCMuTau_Scalar_TuneCP5_13TeV-madgraph-pythia8 ${tgdir17}/TT_LFV_TToCMuTau_Scalar &> ${logdir}/TT_LFV_TToCMuTau_Scalar_skim17.out
./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir20ul16post}/TT_LFV_TToCMuTau_Vector_TuneCP5_13TeV-madgraph-pythia8 ${tgdir17}/TT_LFV_TToCMuTau_Vector &> ${logdir}/TT_LFV_TToCMuTau_Vector_skim17.out
./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir20ul16post}/TT_LFV_TToCMuTau_Tensor_TuneCP5_13TeV-madgraph-pythia8 ${tgdir17}/TT_LFV_TToCMuTau_Tensor &> ${logdir}/TT_LFV_TToCMuTau_Tensor_skim17.out
./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir20ul16post}/TT_LFV_TToUMuTau_Scalar_TuneCP5_13TeV-madgraph-pythia8 ${tgdir17}/TT_LFV_TToUMuTau_Scalar &> ${logdir}/TT_LFV_TToUMuTau_Scalar_skim17.out
./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir20ul16post}/TT_LFV_TToUMuTau_Vector_TuneCP5_13TeV-madgraph-pythia8 ${tgdir17}/TT_LFV_TToUMuTau_Vector &> ${logdir}/TT_LFV_TToUMuTau_Vector_skim17.out
./skimnanoaod.py -F --split ${njobs} -Y skim17 ${mcdir20ul16post}/TT_LFV_TToUMuTau_Tensor_TuneCP5_13TeV-madgraph-pythia8 ${tgdir17}/TT_LFV_TToUMuTau_Tensor &> ${logdir}/TT_LFV_TToUMuTau_Tensor_skim17.out

./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul16post}/ST_LFV_TCMuTau_Scalar_TuneCP5_13TeV-madgraph-pythia8 ${tgdir18}/ST_LFV_TCMuTau_Scalar &> ${logdir}/ST_LFV_TCMuTau_Scalar_skim18.out
./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul16post}/ST_LFV_TCMuTau_Vector_TuneCP5_13TeV-madgraph-pythia8 ${tgdir18}/ST_LFV_TCMuTau_Vector &> ${logdir}/ST_LFV_TCMuTau_Vector_skim18.out
./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul16post}/ST_LFV_TCMuTau_Tensor_TuneCP5_13TeV-madgraph-pythia8 ${tgdir18}/ST_LFV_TCMuTau_Tensor &> ${logdir}/ST_LFV_TCMuTau_Tensor_skim18.out
./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul16post}/ST_LFV_TUMuTau_Scalar_TuneCP5_13TeV-madgraph-pythia8 ${tgdir18}/ST_LFV_TUMuTau_Scalar &> ${logdir}/ST_LFV_TUMuTau_Scalar_skim18.out
./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul16post}/ST_LFV_TUMuTau_Vector_TuneCP5_13TeV-madgraph-pythia8 ${tgdir18}/ST_LFV_TUMuTau_Vector &> ${logdir}/ST_LFV_TUMuTau_Vector_skim18.out
./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul16post}/ST_LFV_TUMuTau_Tensor_TuneCP5_13TeV-madgraph-pythia8 ${tgdir18}/ST_LFV_TUMuTau_Tensor &> ${logdir}/ST_LFV_TUMuTau_Tensor_skim18.out
./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul16post}/TT_LFV_TToCMuTau_Scalar_TuneCP5_13TeV-madgraph-pythia8 ${tgdir18}/TT_LFV_TToCMuTau_Scalar &> ${logdir}/TT_LFV_TToCMuTau_Scalar_skim18.out
./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul16post}/TT_LFV_TToCMuTau_Vector_TuneCP5_13TeV-madgraph-pythia8 ${tgdir18}/TT_LFV_TToCMuTau_Vector &> ${logdir}/TT_LFV_TToCMuTau_Vector_skim18.out
./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul16post}/TT_LFV_TToCMuTau_Tensor_TuneCP5_13TeV-madgraph-pythia8 ${tgdir18}/TT_LFV_TToCMuTau_Tensor &> ${logdir}/TT_LFV_TToCMuTau_Tensor_skim18.out
./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul16post}/TT_LFV_TToUMuTau_Scalar_TuneCP5_13TeV-madgraph-pythia8 ${tgdir18}/TT_LFV_TToUMuTau_Scalar &> ${logdir}/TT_LFV_TToUMuTau_Scalar_skim18.out
./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul16post}/TT_LFV_TToUMuTau_Vector_TuneCP5_13TeV-madgraph-pythia8 ${tgdir18}/TT_LFV_TToUMuTau_Vector &> ${logdir}/TT_LFV_TToUMuTau_Vector_skim18.out
./skimnanoaod.py -F --split ${njobs} -Y skim18 ${mcdir20ul16post}/TT_LFV_TToUMuTau_Tensor_TuneCP5_13TeV-madgraph-pythia8 ${tgdir18}/TT_LFV_TToUMuTau_Tensor &> ${logdir}/TT_LFV_TToUMuTau_Tensor_skim18.out
