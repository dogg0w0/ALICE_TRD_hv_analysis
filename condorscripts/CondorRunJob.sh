#!/bin/bash

BASEDIR=/misc/alidata120/alice_u/schlepper
OUTDIR=$5

echo "OUTPUT DIR:  $OUTDIR"


# copy executable
cp -a -v $BASEDIR/analysis_run .

./analysis_run $1 $2 $3 $4 >>  $OUTDIR/rootlog/log_$1.txt

echo "process finished: create the dir. for output"

mkdir -p $OUTDIR
mv -f sm_*.root $OUTDIR/.

echo "done copying output"
