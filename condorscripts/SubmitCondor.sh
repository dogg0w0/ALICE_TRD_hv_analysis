#!/bin/bash

runData="kTRUE"
basedir="/misc/alidata120/schlepper/trd_hv"
input_data_dir="/misc/alidata130/alice_u/mjkim/TRDHV/trdhv_historytree/www/"
outfile_path="/misc/alidata120/alice_u/schlepper/ALICE_TRD_hv_analysis/outfile.txt"
output_dir="condoroutput"

rm -rf sector_number.txt

# Available dates
declare -a dates=("2017/05/09")



if [ ! -d "condorOut" ]; then
    mkdir -p condorOut
fi

if [ ! -d "condorLog" ]; then
    mkdir -p condorLog
fi

if [ ! -d "condoroutput" ]; then
    mkdir -p condoroutput/rootlog
fi


#submit data
if [ "$runData" = "kTRUE" ]
then
    
    for sector_number in {0..17}
    do
        for date in ${dates[@]}; do
            echo "${sector_number} ${input_data_dir} ${outfile_path} ${date} ${output_dir}" >> sector_number.txt
        done
    done
fi

condor_submit CondorRun.sh
