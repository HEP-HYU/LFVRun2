#!/bin/bash
version=skim_LFVv7
datapath=/data1/common/skimmed_NanoAOD/$version/data
# Syst : norm, jecup, jecdown, puup, pudown, btagup_jes, btagdown_jes
syst=btagup_hf
syst=btagdown_hf
#syst=btagup_lf
syst=btagdown_lf
target=dec_02_${syst} # Arbitrary folder name
mkdir -p ${target}
./processnanoaod.py --allinone -Y 16pre -S ${syst} --globaltag Summer19UL16APV_RunBCD_V7 --json=data/GoldenJSON/Cert_271036-284044_13TeV_Legacy2016_Collisions16_JSON.txt ${datapath}/SingleMuon2016B2 ${target}/Run16preB2_${syst}.root &> ${target}/Run16preB2_${syst}.out &
./processnanoaod.py --allinone -Y 16pre -S ${syst} --globaltag Summer19UL16APV_RunBCD_V7 --json=data/GoldenJSON/Cert_271036-284044_13TeV_Legacy2016_Collisions16_JSON.txt ${datapath}/SingleMuon2016C ${target}/Run16preC_${syst}.root &> ${target}/Run16preC_${syst}.out &
./processnanoaod.py --allinone -Y 16pre -S ${syst} --globaltag Summer19UL16APV_RunBCD_V7 --json=data/GoldenJSON/Cert_271036-284044_13TeV_Legacy2016_Collisions16_JSON.txt ${datapath}/SingleMuon2016D ${target}/Run16preD_${syst}.root &> ${target}/Run16preD_${syst}.out &
./processnanoaod.py --allinone -Y 16pre -S ${syst} --globaltag Summer19UL16APV_RunEF_V7 --json=data/GoldenJSON/Cert_271036-284044_13TeV_Legacy2016_Collisions16_JSON.txt ${datapath}/SingleMuon2016E ${target}/Run16preE_${syst}.root &> ${target}/Run16preE_${syst}.out &
./processnanoaod.py --allinone -Y 16pre -S ${syst} --globaltag Summer19UL16APV_RunEF_V7 --json=data/GoldenJSON/Cert_271036-284044_13TeV_Legacy2016_Collisions16_JSON.txt ${datapath}/SingleMuon2016Fpre ${target}/Run16preF_${syst}.root &> ${target}/Run16preF_${syst}.out &
./processnanoaod.py --allinone -Y 16post -S ${syst} --globaltag Summer19UL16_RunFGH_V7 --json=data/GoldenJSON/Cert_271036-284044_13TeV_Legacy2016_Collisions16_JSON.txt ${datapath}/SingleMuon2016Fpost ${target}/Run16postF_${syst}.root &> ${target}/Run16postF_${syst}.out &
./processnanoaod.py --allinone -Y 16post -S ${syst} --globaltag Summer19UL16_RunFGH_V7 --json=data/GoldenJSON/Cert_271036-284044_13TeV_Legacy2016_Collisions16_JSON.txt ${datapath}/SingleMuon2016G ${target}/Run16postG_${syst}.root &> ${target}/Run16postG_${syst}.out &
./processnanoaod.py --allinone -Y 16post -S ${syst} --globaltag Summer19UL16_RunFGH_V7 --json=data/GoldenJSON/Cert_271036-284044_13TeV_Legacy2016_Collisions16_JSON.txt ${datapath}/SingleMuon2016H ${target}/Run16postH_${syst}.root &> ${target}/Run16postH_${syst}.out &
#sleep 40m
./processnanoaod.py --allinone -Y 17 -S ${syst} --globaltag Summer19UL17_RunB_V5 --json=data/GoldenJSON/Cert_294927-306462_13TeV_UL2017_Collisions17_GoldenJSON.txt ${datapath}/SingleMuon2017B ${target}/Run17B_${syst}.root &> ${target}/Run17B_${syst}.out &
./processnanoaod.py --allinone -Y 17 -S ${syst} --globaltag Summer19UL17_RunC_V5 --json=data/GoldenJSON/Cert_294927-306462_13TeV_UL2017_Collisions17_GoldenJSON.txt ${datapath}/SingleMuon2017C ${target}/Run17C_${syst}.root &> ${target}/Run17C_${syst}.out &
./processnanoaod.py --allinone -Y 17 -S ${syst} --globaltag Summer19UL17_RunD_V5 --json=data/GoldenJSON/Cert_294927-306462_13TeV_UL2017_Collisions17_GoldenJSON.txt ${datapath}/SingleMuon2017D ${target}/Run17D_${syst}.root &> ${target}/Run17D_${syst}.out &
./processnanoaod.py --allinone -Y 17 -S ${syst} --globaltag Summer19UL17_RunE_V5 --json=data/GoldenJSON/Cert_294927-306462_13TeV_UL2017_Collisions17_GoldenJSON.txt ${datapath}/SingleMuon2017E ${target}/Run17E_${syst}.root &> ${target}/Run17E_${syst}.out &
./processnanoaod.py --allinone -Y 17 -S ${syst} --globaltag Summer19UL17_RunF_V5 --json=data/GoldenJSON/Cert_294927-306462_13TeV_UL2017_Collisions17_GoldenJSON.txt ${datapath}/SingleMuon2017F/000 ${target}/Run17F_${syst}_1.root &> ${target}/Run17F_${syst}_1.out &
./processnanoaod.py --allinone -Y 17 -S ${syst} --globaltag Summer19UL17_RunF_V5 --json=data/GoldenJSON/Cert_294927-306462_13TeV_UL2017_Collisions17_GoldenJSON.txt ${datapath}/SingleMuon2017F/058 ${target}/Run17F_${syst}_2.root &> ${target}/Run17F_${syst}_2.out &
./processnanoaod.py --allinone -Y 17 -S ${syst} --globaltag Summer19UL17_RunF_V5 --json=data/GoldenJSON/Cert_294927-306462_13TeV_UL2017_Collisions17_GoldenJSON.txt ${datapath}/SingleMuon2017F/116 ${target}/Run17F_${syst}_3.root &> ${target}/Run17F_${syst}_3.out &
./processnanoaod.py --allinone -Y 18 -S ${syst} --globaltag Summer19UL18_RunA_V5 --json=data/GoldenJSON/Cert_314472-325175_13TeV_Legacy2018_Collisions18_JSON.txt ${datapath}/SingleMuon2018A ${target}/Run18A_${syst}.root &> ${target}/Run18A_${syst}.out &
./processnanoaod.py --allinone -Y 18 -S ${syst} --globaltag Summer19UL18_RunB_V5 --json=data/GoldenJSON/Cert_314472-325175_13TeV_Legacy2018_Collisions18_JSON.txt ${datapath}/SingleMuon2018B ${target}/Run18B_${syst}.root &> ${target}/Run18B_${syst}.out &
./processnanoaod.py --allinone -Y 18 -S ${syst} --globaltag Summer19UL18_RunC_V5 --json=data/GoldenJSON/Cert_314472-325175_13TeV_Legacy2018_Collisions18_JSON.txt ${datapath}/SingleMuon2018C ${target}/Run18C_${syst}.root &> ${target}/Run18C_${syst}.out &
./processnanoaod.py --allinone -Y 18 -S ${syst} --globaltag Summer19UL18_RunD_V5 --json=data/GoldenJSON/Cert_314472-325175_13TeV_Legacy2018_Collisions18_JSON.txt ${datapath}/SingleMuon2018D/000 ${target}/Run18D_${syst}_1.root &> ${target}/Run18D_${syst}_1.out &
./processnanoaod.py --allinone -Y 18 -S ${syst} --globaltag Summer19UL18_RunD_V5 --json=data/GoldenJSON/Cert_314472-325175_13TeV_Legacy2018_Collisions18_JSON.txt ${datapath}/SingleMuon2018D/049 ${target}/Run18D_${syst}_2.root &> ${target}/Run18D_${syst}_2.out &
./processnanoaod.py --allinone -Y 18 -S ${syst} --globaltag Summer19UL18_RunD_V5 --json=data/GoldenJSON/Cert_314472-325175_13TeV_Legacy2018_Collisions18_JSON.txt ${datapath}/SingleMuon2018D/098 ${target}/Run18D_${syst}_3.root &> ${target}/Run18D_${syst}_3.out &
./processnanoaod.py --allinone -Y 18 -S ${syst} --globaltag Summer19UL18_RunD_V5 --json=data/GoldenJSON/Cert_314472-325175_13TeV_Legacy2018_Collisions18_JSON.txt ${datapath}/SingleMuon2018D/147 ${target}/Run18D_${syst}_4.root &> ${target}/Run18D_${syst}_4.out &
