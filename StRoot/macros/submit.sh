#!/bin/bash
date
starver SL16d

rm -rf *session*

if [ -d ./jobs ]; then
  rm -rf ./jobs
fi

mkdir ./jobs
mkdir ./jobs/report
mkdir ./jobs/csh
mkdir ./jobs/list

if [ -d ./Local_lib_starsim_hijing_charm.package ]; then
  rm -rf Local_lib*
fi

if [ -d ./.sl64_gcc482 ]; then
  rm -rf .sl64_gcc482/
fi

rm -rf ./StRoot
rm -rf starsim.hijing.Charm.C
rm -rf run.sh
rm -rf makePicoDst.C

cp -r ../../src/StRoot ./
cp -r StRoot/macros/starsim.hijing.Charm.C ./
cp -r StRoot/macros/run.sh ./
cp -r StRoot/macros/makePicoDst.C ./

cons

nrun=$1
path=$PWD
echo $path
star-submit-template -template submit.xml -entities nRuns=${nrun},base=${path}
#Fix privileges for report, log, etc.
#find ./ -user $USER -exec chgrp rhstar {} \;
#find ./ -user $USER -exec chmod g+rw {} \;
