#!/bin/sh
starver new
current_data_time="`date \"+%m/%d %H:%M:%S\"`"
echo $current_data_time
job=$1
#run=${job: -1}
#run=$job;
run=`perl -e "my @ss = split /_/, \"$job\"; print @ss[1];"`
echo  runid: $run
echo  jobid: $job
mkdir ./Files_$job
mkdir ./Files_$job/Hijing
mkdir ./Files_$job/Pythia6
mkdir ./Files_$job/fzd
mkdir ./Files_$job/tpc_reco
mkdir ./Files_$job/hft_reco
mkdir ./Files_$job/pile_up
mkdir ./Files_$job/picodst
mkdir ./Files_$job/tpcRes
# --- Pile up file
at=`perl -e 'srand; print int(rand(99)+1)'`
cp -p /star/institutions/lbl/mlomnitz/mlomnitz_prod/EffStudies/Sti_2015/pileup/pileupSet$at/pile**.root ./Files_$job/pile_up/pile_up$at.root

nevt=$2

function doStarPythia {
# ---- Producing Pythia6 charmed events
echo "Generating pythia charm event"
root4star <<EOF
.L starsim.hijing.Charm.C
starsim($nevt,$run,$RANDOM,0)
.q
EOF
mv pythia6_charm* ./Files_$job/Pythia6/.
#mv hijing* ./Files_$job/fzd/.
}

function doStarHijing {
# ---- Producing Pythia6 charmed events
echo "Generating Hijing BG event"
root4star <<EOF
.L starsim.hijing.Charm.C
starsim($nevt,$run,$RANDOM,4)
.q
EOF
mv hijing* ./Files_$job/Hijing/.
}


function doStarsim {
# ---- Producing sim file .fzd
echo "A hijing......"
inPyFile=$1
inMode=$2

root4star <<EOF
.L starsim.hijing.Charm.C
starsim($nevt,$run,$RANDOM,$inMode,"$inPyFile")
.q
EOF
mv hijing* ./Files_$job/fzd/.
}

function doStarsimTest {
# ---- Producing sim file .fzd
echo "A hijing......"
inMode=$1

root4star <<EOF
.L starsim.hijing.Charm.C
starsim($nevt,$run,$RANDOM,$inMode)
.q
EOF
mv hijing* ./Files_$job/fzd/.
}

function doTest {
# ---- Producing sim file .fzd
echo "do test"
inPyFile=$1

root4star <<EOF
.L starsim.hijing.Charm.C
starsim($nevt,$run,$RANDOM,3,"$inPyFile")
.q
EOF
#mv hijing* ./Files_$job/fzd/.
}

function doTpcReco {
echo "Lomnitz: TPC reco starting"
# ---- TPC reconstruction
nEvents=10
start=0
end=9
inFile=$1

chain=y2014a,event,tpc,fzin,geantout,tpcrs,TpcHitMover,TpxClu,evout,-HitFilt

echo $chain

pwd 
#env
root4star -b -l <<EOF
.x bfc.C($start,$end,"$chain","$inFile");
EOF

mv hijing* Files_$job/tpc_reco/.
}

function doHftReco {
echo "Lomnitz: HFT Reco starting"
# ---- hft reconstruction
inFile2=$1
inPile=Files_$job/pile_up/pile_up$at.root
chain2=y2014a,event,McEvent,MuDst,in,sim_T,gen_T,geantout,evout,FieldOn,AgML,usexgeom,MakeEvent,ITTF,Sti,NoSsdIt,NoSvtIt,NoSstIt,StiHftC,pxlFastSim,istFastSim,Idst,BAna,l0,Tree,logger,genvtx,tpcDB,bbcSim,btofsim,emcY2,EEfs,evout,-dstout,IdTruth,big,McEvout,MiniMcMk,McAna,ReadAll,clearmem

echo $chain2

pwd 
#env
root4star -b -l <<EOF
.x bfc.C(-1,"$chain2","$inFile2");
StPxlSimMaker* pxl = chain->GetMaker("pxlSimMaker");
pxl->useIdealGeom(); // ideal geometry
//pxl->useDbGeom();  // survey geometry
pxl->useRandomSeed();

pxl->addPileup(); 
pxl->setPileupFile("$inPile");


TString OutputFileName(gSystem->BaseName("$inFile2"));
OutputFileName.ReplaceAll(".event.root","");
StMcAnalysisMaker* mcAnalysisMaker = (StMcAnalysisMaker*)chain->GetMaker("StMcAnalysisMaker");
mcAnalysisMaker->setOutFileName(OutputFileName);

chain->Init();
chain->EventLoop($start,$end);
chain->Finish();

EOF
mv hijing_charm_$run.tpcRes.root Files_$job/tpcRes/hijing_charm_sim_production_v0_$job.tpcRes.root
mv *.root Files_$job/hft_reco/.
}

function doPicoDst {
# ---- PicoDst
inMuDst=$1
inMcEvt=$2
root4star -l -b -q makePicoDst.C\($run,\"$inMuDst\",\"$inMcEvt\"\)
find . -type f -name "*.picoDst.root" -exec chgrp rhstar {} \;
find . -type f -name "*.picoDst.root" -exec chmod g+rw {} \;
mv *.picoDst.root hijing_charm_sim_production_v0_$run.picoDst.root
}

inPyFile=./Files_$job/Pythia6/pythia6_charm_$run.starsim.root
inHjFile=./Files_$job/Hijing/hijing_charm_$run.starsim.root
inFzd=Files_$job/fzd/hijing_charm_$run.starsim.fzd
inEvent=Files_$job/tpc_reco/hijing_charm_$run.event.root
inMuDst=Files_$job/hft_reco/hijing_charm_$run.MuDst.root
inMcEvt=Files_$job/hft_reco/hijing_charm_$run.McEvent.root

if [ ! -e "$inPyFile" ]; then
doStarPythia
fi
doStarsim $inPyFile 1
#doStarsimTest 3

doTpcReco $inFzd
doHftReco $inEvent
doPicoDst $inMuDst $inMcEvt

#mv vertex*.root ./Files_$job/.

# for test
#doTest $inPyFile
#if [ ! -e "$inHjFile" ]; then
#doStarHijing
#fi

#if [ -e "$inPyFile" ];  then
#echo "Run Hijing + eventReader"
#doStarsim $inPyFile 1
#elif [ -e "$inHjFile" ]; then
#echo "Run pythia6 + eventReader"
#doStarsim $inHjFile 5
#else
#echo "No bg file !!!!!"
#fi

current_data_time="`date \"+%m/%d %H:%M:%S\"`"
echo $current_data_time


#privilges
#find . -type f -name "*.picoDst.root" -exec chmod g+rw {} \;
#find Files_$job/ -type d -exec chgrp rhstar {} \;
#find Files_$job/ -type d -exec chmod g+rw {} \;
#find Files_$job/ -type f -exec chgrp rhstar {} \;
#find Files_$job/ -type f -exec chmod g+rw {} \;
# ---- Done bring files back
#tar -cvf Files_$job.tar Files_$job
#mv Files_$job ../output/.
