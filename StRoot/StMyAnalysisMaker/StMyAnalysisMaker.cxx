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

ClassImp(StMyAnalysisMaker)

StMyAnalysisMaker::StMyAnalysisMaker(char const *name, StPicoDstMaker *picoMaker, char *filename)
   : StMaker(name), mPicoDstMaker(picoMaker), mPicoEvent(NULL), mOutFile(NUKK) 
{
    mOutFile = TFile::Open(filename, "recreate");
}

StMyAnalysisMaker::~StMyAnalysisMaker() {
}

Int_t StMyAnalysisMaker::Init() {
}

Int_t StMyAnalysisMaker::Make() {
    if (!mPicoDstMaker)
    {
        LOG_WARN << " No PicoDstMaker! Skip! " << endm;
        return kStWarn;
    }
    
    StPicoDst const * picoDst = mPicoDstMaker->picoDst();
    if (!picoDst)
    {
        LOG_WARN << " No PicoDst! Skip! " << endm;
        return kStWarn;
    }
    
    mPicoEvent = picoDst->event();
}

Int_t StMyAnalysisMaker::Finish() {
}


//---------------------------------------------------------
// other utility function
//---------------------------------------------------------
