#!/bin/csh

starver new

set inDir=`pwd`

cd $inDir

root4star -b -q ${inDir}/runHF.C\($1,$2\)


