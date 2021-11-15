pname=$1
if [ -z "$1" ]
then
    echo "No Input Argument"
else
    sys=norm
    cd ${pname}
    rm -rf *.out
    for i in 18 16post 16pre 17
    do
        echo Setup ${i} samples
        mkdir -p ${i}/tmp
        mv *${i}*${sys}*.root ${i}
        cd ${i}
        mv Run* tmp
        rm -rf tmp/Run${i}_${sys}.root
        hadd Run${i}_${sys}.root tmp/Run${i}*_${sys}.root
        #hadd TTToSemiLeptonic_${i}_${sys}.root TTToSemi*.root
        #mv tmp/Run20${i}_${sys}.root tmp/TTToSemiLeptonic_${i}_${sys}.root ./
        cd ../
    done

    #cd ${pname}
    #for i in 16pre 16post 17
    #do
    #    cp 18/LFV* ${i}/
    #    cd ${i}
    #    rename _18_ _${i}_ *_18_*
    #    cd ../
    #done
    rm -rf Run2_${sys}.root
    hadd Run2_${sys}.root 16pre/Run16pre_${sys}.root 16post/Run16post_${sys}.root 17/Run17_${sys}.root 18/Run18_${sys}.root
    rm -rf *LFV*.root
    hadd ST_LFV_norm.root 18/ST_LFV*.root
    hadd TT_LFV_norm.root 18/TT_LFV*.root
    rm -rf TTTo2L2Nu_norm.root TTToSemiLeptonic_norm.root
    hadd TTTo2L2Nu_norm.root 1*/TTTo2L2Nu*.root
    hadd TTToSemiLeptonic_norm.root 1*/TTToSemiLeptonic*.root
    cd ../
fi
