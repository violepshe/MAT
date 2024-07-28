#!/bin/bash

# Iterate with values given from Qt/C++ interface and call other script for image generation

mcaseSetFile=$1
numberOfModesSSPI=$2
rdbName=$3
startharmValue=$4
endharmValue=$5
runMVIB=$6
mVIBrpmValue=$7
projectName=$8

dos2unix ./export_image.sh
foldername="plots"
typeofplot="disp"
for ((j=startharmValue; j<=endharmValue; j++)); do
    for ((i=1; i<=numberOfModesSSPI; i++)); do
        for dispvalue in 1102 1103; do
            if [$projectName= ""]; then
                nameImage="harm${j}_mode${i}_disp${dispvalue}.tiff"
            else
                nameImage="${projectName}_harm${j}_mode${i}_disp${dispvalue}.tiff"
            fi
            caseNumber = $(( (j - startharmValue) * numberOfModesSSPI + i))
            sh ./export_image.sh $rdbName $nameImage $caseNumber $dispvalue $typeofplot
            mv "${nameImage}" "./${foldername}"
        done
    done
done
