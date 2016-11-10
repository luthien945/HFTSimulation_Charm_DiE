#ifndef StMyTrack_H
#define StMyTrack_H
#include "TObject.h"
#include "StThreeVectorF.hh"

class StMyTrack : public TObject {
    public:
	StMyTrack();
	StMyTrack(UShort_t pid, UShort_t parentGid, Float_t dcaXY, Float_t dcaZ, StThreeVectorF mcMom, StThreeVectorF pMom, StThreeVectorF gMom);
	~StMyTrack();

	StThreeVectorF mMcMom;
	StThreeVectorF mPMom;
	StThreeVectorF mGMom;
	UShort_t mGePid;
	UShort_t mParentPid;
	Float_t mDcaXY;
	Float_t mDcaZ;

	Int_t mHftHitsMap;

	ClassDef(StMyTrack, 1)
};

#endif
