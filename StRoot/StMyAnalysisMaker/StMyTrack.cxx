#include "StMyTrack.h"

ClassImp(StMyTrack)

StMyTrack::StMyTrack() {
}

StMyTrack::StMyTrack(UShort_t pid, UShort_t parentGid, Float_t dcaXY, Float_t dcaZ, StThreeVectorF mcMom, StThreeVectorF pMom, StThreeVectorF gMom) {
    mGePid = pid;
    mParentPid = parentGid;
    mDcaXY = dcaXY;
    mDcaZ  = dcaZ;

    mMcMom = mcMom;
    mPMom = pMom;
    mGMom = gMom;
}

StMyTrack::~StMyTrack () {
}
