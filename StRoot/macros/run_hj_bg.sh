#!/bin/sh
#starver new  ~~not working on pdsf (╯‵□′)╯︵┻━┻
current_data_time="`date \"+%m/%d %H:%M:%S\"`"
echo $current_data_time
job=$1
nevt=$2
#run=${job: -1}
#run=$job;
run=`perl -e "my @ss = split /_/, \"$job\"; print @ss[1];"`
echo  runid: $run
echo  jobid: $job
mkdir ./Files_Hijing_bg_$job
at=`perl -e 'srand; print int(rand(99)+1)'`

function doStarHijing {
echo "Generating Hijing BG event"
root4star  <<EOF
.L starsim.hijing.C
starsim($nevt,$run,$RANDOM)
.q
EOF
mv hijing* ./Files_Hijing_bg_$job/.
}

doStarHijing

current_data_time="`date \"+%m/%d %H:%M:%S\"`"
echo $current_data_time
