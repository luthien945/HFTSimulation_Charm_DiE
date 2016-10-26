#!/bin/sh
starver SL16d
job=$1
#run=${job: -1}
#run=$job;
run=`perl -e "my @ss = split /_/, \"$job\"; print @ss[1];"`
echo  runid: $run
echo  jobid: $job
mkdir ./Files_$job
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

# ---- Producing Pythia6 charmed events
root4star <<EOF
.L starsim.hijing.Charm.C
starsim(1,$run,$RANDOM,0)
.q
EOF
mv pythia6_charm* ./Files_$job/Pythia6/.
inPyFile=./Files_$job/Pythia6/pythia6_charm_$job.starsim.root

# ---- Producing sim file .fzd
root4star <<EOF
.L starsim.hijing.Charm.C
starsim(1,$run,$RANDOM,1,"$inPyFile")
.q
EOF
mv hijing* ./Files_$job/fzd/.

echo "Lomnitz: TPC reco starting"
# ---- TPC reconstruction
nEvents=10
start=0
end=9
inFile=Files_$job/fzd/hijing_charm_$run.starsim.fzd

chain=y2014a,event,tpc,fzin,geantout,tpcrs,TpcHitMover,TpxClu,evout,-HitFilt

echo $chain

pwd 
#env
root4star -b -l <<EOF
.x bfc.C($start,$end,"$chain","$inFile");
EOF

mv hijing* Files_$job/tpc_reco/.

echo "Lomnitz: HFT Reco starting"
# ---- hft reconstruction
inFile2=Files_$job/tpc_reco/hijing_charm_$run.event.root
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
# ---- PicoDst
root4star -l -b -q makePicoDst.C\($run,\"Files_$job/hft_reco/hijing_charm_$run.MuDst.root\",\"Files_$job/hft_reco/hijing_charm_$run.McEvent.root\"\)
find . -type f -name "*.picoDst.root" -exec chgrp rhstar {} \;
find . -type f -name "*.picoDst.root" -exec chmod g+rw {} \;
mv *.picoDst.root hijing_charm_sim_production_v0_$run.picoDst.root
#privilges
#find . -type f -name "*.picoDst.root" -exec chmod g+rw {} \;
#find Files_$job/ -type d -exec chgrp rhstar {} \;
#find Files_$job/ -type d -exec chmod g+rw {} \;
#find Files_$job/ -type f -exec chgrp rhstar {} \;
#find Files_$job/ -type f -exec chmod g+rw {} \;
# ---- Done bring files back
#tar -cvf Files_$job.tar Files_$job
#mv Files_$job ../output/.
