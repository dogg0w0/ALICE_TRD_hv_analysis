#!/bin/bash

pathTOsorted="/home/doggo/Downloads/ALICE_TRD/hv_ALICE_Data/"

declare -a listTOFfiles=("tof_t0/HighLumiTest_3Sep2018.csv" "tof_t0/HighLumiTest_7Sep2018_Fill_7133_Ramp.csv" "tof_t0/HighLumiTest_7Sep2018_Fill_7135.csv")
declare -a listT0files=("tof_t0/Fill1722_InstLumi.cvs" "tof_t0/Fill1733_InstLumi.cvs" "tof_t0/Fill1735_InstLumi.cvs")
declare -a listStartTimes=("1535963411" "1536326026" "1536350231")
declare -a listEndTimes=("1535986886" "1536329471" "1536359296")

rm -rf temp
rm -rf output

if [ ! -d "temp" ]; then
    mkdir -p temp/tof_t0
    mkdir -p temp/sortedcsv
    mkdir -p temp/match_hv_current
    mkdir -p temp/match_current_lumi
fi

if [ ! -d "output" ]; then
    mkdir -p output
fi


for tofFileindex in "${!listTOFfiles[@]}"
do
    python code/match_lumi.py ${listTOFfiles[tofFileindex]} ${listT0files[tofFileindex]} temp/tof_t0/tof_t0_${tofFileindex}.csv
    mkdir -p output/date_${tofFileindex}
    #for index in {1..2160..2}
    for index in {1..4..2}
    do
        echo "$((${index} + 1))"
        ./code/makecsv "${pathTOsorted}/sorted_${index}.csv.root" temp/sortedcsv/sorted_${index}.csv ${listStartTimes[tofFileindex]} ${listEndTimes[tofFileindex]}
        ./code/makecsv "${pathTOsorted}/sorted_$((${index} + 1)).csv.root" temp/sortedcsv/sorted_$((${index} + 1)).csv ${listStartTimes[tofFileindex]} ${listEndTimes[tofFileindex]}
        python code/match_hv_current.py temp/sortedcsv/sorted_${index}.csv temp/sortedcsv/sorted_$((${index} + 1)).csv temp/match_hv_current/hv_current_${index}.csv
        python code/match_lumi_current.py temp/match_hv_current/hv_current_${index}.csv temp/tof_t0/tof_t0_${tofFileindex}.csv temp/match_current_lumi/matched_lumi_current_${index}.csv
        ./code/makeroot temp/match_current_lumi/matched_lumi_current_${index}.csv output/date_${tofFileindex}/sorted_$((${index} + 1)).csv.root
    done
done

rm -rf temp