#!/bin/bash

pathTOsorted="/home/doggo/Downloads/ALICE_TRD/hv_ALICE_Data/"

declare -a listTOFfiles=("tof_t0/HighLumiTest_3Sep2018.csv" "tof_t0/HighLumiTest_7Sep2018_Fill_7133_Ramp.csv" "tof_t0/HighLumiTest_7Sep2018_Fill_7135.csv")
declare -a listT0files=("tof_t0/Fill1722_InstLumi.csv" "tof_t0/Fill1733_InstLumi.csv" "tof_t0/Fill1735_InstLumi.csv")
declare -a listStartTimes=("1535963411" "1536326026" "1536350231")
declare -a listEndTimes=("1535986886" "1536329471" "1536359296")

rm -rf temp_all
rm -rf output_all

if [ ! -d "temp_all" ]; then
    mkdir -p temp_all/tof_t0
    mkdir -p temp_all/sortedcsv
    mkdir -p temp_all/match_hv_current
    mkdir -p temp_all/match_current_lumi
fi

if [ ! -d "output_all" ]; then
    mkdir -p output_all
fi


for tofFileindex in "${!listTOFfiles[@]}"
do
    python code/match_lumi.py ${listTOFfiles[tofFileindex]} ${listT0files[tofFileindex]} temp_all/tof_t0/tof_t0_${tofFileindex}.csv
    mkdir -p output_all/date_${tofFileindex}
    for index in {1..2160..2}
    do
        echo "$((${index} + 1))"
        ./code/makecsv "${pathTOsorted}/sorted_${index}.csv.root" temp_all/sortedcsv/sorted_${index}.csv ${listStartTimes[tofFileindex]} ${listEndTimes[tofFileindex]}
        ./code/makecsv "${pathTOsorted}/sorted_$((${index} + 1)).csv.root" temp_all/sortedcsv/sorted_$((${index} + 1)).csv ${listStartTimes[tofFileindex]} ${listEndTimes[tofFileindex]}
        python code/match_hv_current.py temp_all/sortedcsv/sorted_${index}.csv temp_all/sortedcsv/sorted_$((${index} + 1)).csv temp_all/match_hv_current/hv_current_${index}.csv
        python code/match_lumi_current.py temp_all/match_hv_current/hv_current_${index}.csv temp_all/tof_t0/tof_t0_${tofFileindex}.csv temp_all/match_current_lumi/matched_lumi_current_${index}.csv
        ./code/makeroot temp_all/match_current_lumi/matched_lumi_current_${index}.csv output_all/date_${tofFileindex}/sorted_$((${index} + 1)).csv.root
    done
done

rm -rf temp_all
