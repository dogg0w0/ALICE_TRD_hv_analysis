#!/bin/bash

for i in {0..17}
do
    root -l -b -q "working.C( ${i} )"
done