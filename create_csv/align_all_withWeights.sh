#!/bin/bash

pathTOsorted="/home/doggo/Downloads/ALICE_TRD/hv_ALICE_Data/"

declare -a listTOFfiles=("tof_t0/HighLumiTest_3Sep2018.csv" "tof_t0/HighLumiTest_7Sep2018_Fill_7133_Ramp.csv" "tof_t0/HighLumiTest_7Sep2018_Fill_7135.csv")
declare -a listT0files=("tof_t0/Fill1722_InstLumi.csv" "tof_t0/Fill1733_InstLumi.csv" "tof_t0/Fill1735_InstLumi.csv")
declare -a listStartTimes=("1535963411" "1536326026" "1536350231")
declare -a listEndTimes=("1535986886" "1536329471" "1536359296")

rm -rf temp_all
rm -rf output_all_wwithWights

if [ ! -d "temp_all" ]; then
    mkdir -p temp_all/tof_t0
    mkdir -p temp_all/sortedcsv
    mkdir -p temp_all/match_hv_current
    mkdir -p temp_all/match_current_lumi
fi

if [ ! -d "output_all_wwithWights" ]; then
    mkdir -p output_all_wwithWights/current
fi

index=0
for tofFileindex in "${!listTOFfiles[@]}"
do
    python code/match_lumi.py ${listTOFfiles[tofFileindex]} ${listT0files[tofFileindex]} temp_all/tof_t0/tof_t0_${tofFileindex}.csv
    mkdir -p output_all_wwithWights/date_${tofFileindex}
    for index_j in {0..35..2}
    do
        for index_i in {1..59..2}
        do
            let index=$((index_i + index_j*60))
            echo $index
            ./code/makecsv "${pathTOsorted}/sorted_${index}.csv.root" temp_all/sortedcsv/sorted_${index}.csv ${listStartTimes[tofFileindex]} ${listEndTimes[tofFileindex]}
            ./code/makecsv "${pathTOsorted}/sorted_$((${index} + 1)).csv.root" temp_all/sortedcsv/sorted_$((${index} + 1)).csv ${listStartTimes[tofFileindex]} ${listEndTimes[tofFileindex]}
            python code/match_hv_current_withWeights.py temp_all/sortedcsv/sorted_${index}.csv temp_all/sortedcsv/sorted_$((${index} + 1)).csv temp_all/match_hv_current/hv_current_${index}.csv ${index_i}
        done
    done
    python code/match_lumi_current_all.py /home/doggo/Downloads/ALICE_TRD/create_csv/temp_all/match_hv_current/ output_all_wwithWights/current/current_date_${tofFileindex}.csv
    python code/match_lumi_current.py output_all_wwithWights/current/current_date_${tofFileindex}.csv temp_all/tof_t0/tof_t0_${tofFileindex}.csv temp_all/match_current_lumi/matched_lumi_current_${tofFileindex}.csv
    ./code/makeroot_all.o temp_all/match_current_lumi/matched_lumi_current_${tofFileindex}.csv output_all_wwithWights/date_${tofFileindex}/current_lumi.csv.root
done

rm -rf temp_all