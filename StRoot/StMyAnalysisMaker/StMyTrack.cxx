#include "StMyTrack.h"

ClassImp(StMyTrack)

StMyTrack::StMyTrack() {
}

StMyTrack::StMyTrack(UShort_t IndexMc ,UShort_t IndexRc, UShort_t GePid, 
	UShort_t IndexParent, UShort_t ParentPid, 
	Float_t DcaXY, Float_t DcaZ, StThreeVectorF McMom, StThreeVectorF PMom, StThreeVectorF GMom,
	Int_t HFTHitsMap){
    mIndexMc     = IndexMc;
    mIndexRc     = IndexRc;
    mIndexParent = IndexParent;
    mGePid       = GePid;
    mParentPid   = ParentPid;
    mDcaXY       = DcaXY;
    mDcaZ        = DcaZ;

    mMcMom       = McMom;
    mPMom        = PMom;
    mGMom        = GMom;
    mHftHitsMap  = HFTHitsMap;
}

StMyTrack::~StMyTrack () {
}
