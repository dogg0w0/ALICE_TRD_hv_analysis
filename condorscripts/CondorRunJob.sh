#!/bin/bash

BASEDIR=/misc/alidata120/alice_u/schlepper/trd_hv
OUTDIR=$5

echo "OUTPUT DIR:  $OUTDIR"


# copy executable
cp -a -v $BASEDIR/analysis_run .

./analysis_run $1 $2 $3 $4

echo "process finished: create the dir. for output"

mkdir -p $OUTDIR
mv -f sm_*.root $OUTDIR/.

echo "done copying output"
