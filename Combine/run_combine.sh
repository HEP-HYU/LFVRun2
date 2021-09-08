#!/bin/bash
regions=(st tt)
operators=(c_s c_v c_t u_s u_v u_t)
rmaxs=(3.0 3.0 3.0 3.0 1.0 0.05)
# stc  s   v   t stus  v   t ttcs  v   t ttus  v   t
runs=(Run2 Run16 Run17 Run18)
outlogfile=combinelog_0906.txt
for run in ${runs[*]}; do
    for region in ${regions[*]}; do
        i=0
        datacard_path=newdatacards/${region}/datacard_countingLFV
        for op in ${operators[*]}; do
            card=${datacard_path}${op}_${run}.txt
            echo "Command : combine ${card} --run blind -n .${region}_${op}_${run} --rMin 0.0001 --rMax ${rmaxs[i]}" >> ${outlogfile}
            combine ${card} --run blind -n .${region}_${op}_${run} --rMin 0.0001 --rMax ${rmaxs[i]}>> ${outlogfile}
            echo "" >> ${outlogfile}
            echo "######################################" >> ${outlogfile}
            echo "" >> ${outlogfile}
            i=$((i+1))
        done
    done
done
