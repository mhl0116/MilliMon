#!/bin/bash
run=539
subrun=1
plotdir="/eos/user/h/hmei/www/milliqan/plots/2022.11.02_source_run/"
while true
do
    if [[ -f /home/milliqan/data/MilliQan_Run${run}.${subrun}_default.root ]]
    then
        echo Run ${run} subrun ${subrun} exists
        cd ..
        ./build/Main ${run} ${subrun}
        scp ./plots/DigitizerHeatMap_Run${run}_Subrun${subrun}_2supermodules_ln.pdf hmei@lxplus:${plotdir}
        run=$(( $run + 1 ))
        cd -
    else
        echo $(date) wait for new file from run ${run}
        sleep 10 # or less like 0.2
    fi
done
