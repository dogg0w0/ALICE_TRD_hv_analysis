#!/bin/bash

pathTOsorted="/media/felix/flush/ALICE_TRD/hv_ALICE_Data"
pathToExecutable="/media/felix/flush/ALICE_TRD/code/2018/build/apps/analysis_run"
declare -a listGainIndex=("-1" "-1" "-1")


#rm analysis_run

#cp -v ${pathToExecutable} .


for date in {0..2}
do
    mkdir -p output/run_${date}
    for sector in {0..17}
    do
        echo $date
        ./analysis_run ${sector} /media/felix/flush/ALICE_TRD/create_csv/output/date_${date}/ /media/felix/flush/ALICE_TRD/create_chamber_map/outfile.txt /media/felix/flush/ALICE_TRD/hv_ALICE_Data/ 1534797124 1534833676 ${listGainIndex[date]}
    done
    mv sm_*.root output/run_${date}/.
done
