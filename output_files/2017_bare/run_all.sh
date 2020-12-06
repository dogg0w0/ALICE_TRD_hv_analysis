#!/bin/bash

for i in {0..17}
do
    ./analysis_run $i /home/doggo/Downloads/ALICE_TRD/hv_ALICE_Data/ /home/doggo/Downloads/ALICE_TRD/create_chamber_map/outfile.txt 2017/05/09 trendingOCDB_2017.root 
done