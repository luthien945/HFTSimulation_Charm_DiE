#!/bin/bash
date

mystarver=SL17a
starver $mystarver
echo "working under STAR_LEVEL $STAR_LEVEL"

rm -rf *session*

if [ -d ./jobs ]; then
  rm -rf ./jobs
fi

mkdir ./jobs
mkdir ./jobs/report
mkdir ./jobs/csh
mkdir ./jobs/list

if [ -d ./Local_lib_*.package ]; then
  rm -rf Local_lib*
fi

function doSetup {
rm -rf ./StRoot
rm -rf starsim.hijing.Charm.C
rm -rf starsim.hijing.C
rm -rf run.sh
rm -rf run_hj_bg.sh
rm -rf *.xml
rm -rf makePicoDst.C

mkdir ./StRoot
cp -r ../../src/StRoot/macros ./StRoot/
cp -r ../../src/StRoot/StarGenerator ./StRoot/
cp -r ../../src/StRoot/StMcAnalysisMaker ./StRoot/
cp -r ../../src/StRoot/StPicoMcAnaMaker ./StRoot/
cp -r ../../src/StRoot/StPxlSimMaker ./StRoot/
cp -r ../../src/StRoot/StIstSimMaker ./StRoot/
cp -r ../../src/StRoot/StRefMultCorr ./StRoot/
cp -r ../../src/StRoot/StPicoDstMaker ./StRoot/
cp -r StRoot/macros/starsim.hijing.Charm.C ./
cp -r StRoot/macros/starsim.hijing.C ./
cp -r StRoot/macros/run.sh ./
cp -r StRoot/macros/run_hj_bg.sh ./
cp -r StRoot/macros/submit.xml ./
cp -r StRoot/macros/submit_hjbg.xml ./
cp -r StRoot/macros/makePicoDst.C ./

if [ ! -d ./.sl64_gcc482 ]; then
  cons
fi
}

if [ "$1" = "setup" ]; then
echo "doSetUp"
doSetup
elif [ "$1" = "real" ]; then
echo "real simu"
nrun=$2
path=$PWD
echo $path
star-submit-template -template submit.xml -entities nRuns=${nrun},base=${path}
elif [ "$1" = "hj_bg" ]; then
echo "run for hj background"
nrun=$2
path=$PWD
echo $path
star-submit-template -template submit_hjbg.xml -entities nRuns=${nrun},base=${path}
fi



