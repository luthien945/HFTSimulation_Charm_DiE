#include "TVector3.h"
#include "TTree.h"
#include "TFile.h"
#include "TString.h"
#include "StThreeVectorF.hh"
#include "StLorentzVectorF.hh"
#include "../StPicoDstMaker/StPicoDst.h"
#include "../StPicoDstMaker/StPicoDstMaker.h"
#include "../StPicoDstMaker/StPicoEvent.h"
#include "../StPicoDstMaker/StPicoTrack.h"
#include "../StPicoDstMaker/StPicoMcEvent.h"
#include "../StPicoDstMaker/StPicoMcTrack.h"

#include "StMyAnalysisMaker.h"
#include "Cuts.h"

ClassImp(StMyAnalysisMaker)

StMyAnalysisMaker::StMyAnalysisMaker(char const *name, StPicoDstMaker *picoMaker, char const* filename)
: StMaker(name), mPicoDstMaker(picoMaker), mPicoDst(NULL), mPicoEvent(NULL) 
{
    mOutFile = TFile::Open(filename, "recreate");
}

StMyAnalysisMaker::~StMyAnalysisMaker() {
}

Int_t StMyAnalysisMaker::Init() {
    return kStOK;
}

void StMyAnalysisMaker::Clear(Option_t *opt) {
    mPicoDst = NULL;
    mPicoEvent = NULL;

    rcEid.clear();
    rcPid.clear();
    mcEid.clear();
    mcPid.clear();
}

Int_t StMyAnalysisMaker::Make() {
    if (!mPicoDstMaker)
    {
        LOG_WARN << " No PicoDstMaker! Skip! " << endm;
        return kStWarn;
    }
    
    mPicoDst = mPicoDstMaker->picoDst();
    if (!mPicoDst)
    {
        LOG_WARN << " No PicoDst! Skip! " << endm;
        return kStWarn;
    }
    
    mPicoEvent = mPicoDst->event();
    if(!isGoodEvent()) return kStOK;
    loopTrack();

    return kStOK;
}

Int_t StMyAnalysisMaker::Finish() {
    return kStOK;
}


//---------------------------------------------------------
// other utility function
//---------------------------------------------------------
bool StMyAnalysisMaker::isGoodEvent()
{
    pVtx = mPicoEvent->primaryVertex();
    int refMult = mPicoEvent->refMult();
    int gRefMult= mPicoEvent->grefMult();

    return fabs(pVtx.z()) < cuts::vz ;
}

int StMyAnalysisMaker::loopTrack() {
    nMcTracks = mPicoDst->numberOfMcTracks();

    cout<<"Number of mctracks : "<<nMcTracks<<endl;
    for(int itMc=0; itMc<nMcTracks; itMc++) {
	StPicoMcTrack *mcTrk = (StPicoMcTrack*) mPicoDst->mctrack(itMc);
	int gePid = mcTrk->GePid();
	int parentId = getParent(mcTrk, kFALSE);
	int ancestorId = getParent(mcTrk, kTRUE);
	if(gePid>=37 && gePid<=40) cout<<"Charmed : "<<gePid<<endl;
	int parentGeId = -999;
	int ancestorGeId = -999;
	if(parentId>=0 && parentId<=Pico::USHORTMAX) {
	    StPicoMcTrack *mcParentTrk = (StPicoMcTrack*) mPicoDst->mctrack(parentId);
	    parentGeId = mcParentTrk->GePid();
	}

	if(ancestorId>=0 && ancestorId<=Pico::USHORTMAX) {
	    StPicoMcTrack *mcAncestorTrk = (StPicoMcTrack*) mPicoDst->mctrack(ancestorId);
	    ancestorGeId = mcAncestorTrk->GePid();
	}

	//if(parentGeId>=37 && parentGeId<=40) {
	//    cout<<"A charm decays : "<<parentGeId<<"->"<<gePid<<endl;
	//}
	//if(gePid==cuts::dau1Gid || gePid==cuts::dau2Gid) {
	//    cout<<"I get an electron";
	//    if(parentGeId >= 37 && parentGeId <=40) {
	//	cout<<" from charm : "<<"parentGeId = "<<parentGeId
	//	    <<" gePid = "<<gePid<<endl;
	//    }else {
	//	cout<<" from sth else : "<<"parentGeId = "<<parentGeId
	//	    <<" gePid = "<<gePid<<endl;
	//    }
	//}
	cout<<"["<<std::setw(5)<<itMc<<", "<<std::setw(5)<<gePid<<" ]"
	    <<"["<<std::setw(5)<<parentId<<", "<<std::setw(5)<<parentGeId<<" ]"
	    <<endl;
    }

}

int StMyAnalysisMaker::getParent(StPicoMcTrack const * const mcTrk, bool doTraceUp) {
    if(mcTrk->parentId()==Pico::USHORTMAX || mcTrk->parentId()==mcTrk->mcId()) return -999;

    int parentId = mcTrk->parentId();
    StPicoMcTrack *mcParentTrack = (StPicoMcTrack*) mPicoDst->mctrack(parentId);
    if(parentId != mcParentTrack->parentId() && doTraceUp) parentId = getParent(mcParentTrack, doTraceUp);
    return parentId;
}
