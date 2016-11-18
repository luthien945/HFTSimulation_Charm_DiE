#ifndef StMyTrack_H
#define StMyTrack_H
#include "TObject.h"
#include "StThreeVectorF.hh"

class StMyTrack : public TObject {
    public:
	StMyTrack();
	StMyTrack(UShort_t IndexMc ,UShort_t IndexRc, UShort_t GePid, 
		UShort_t IndexParent, UShort_t ParentPid, 
		Float_t DcaXY, Float_t DcaZ, StThreeVectorF McMom, StThreeVectorF PMom, StThreeVectorF GMom,
		Int_t HFTHitsMap);
	~StMyTrack();

	StThreeVectorF   mcMom()       { return  mMcMom;       }
	StThreeVectorF   pMom()        { return  mPMom;        }
	StThreeVectorF   gMom()        { return  mGMom;        }
	UShort_t         indexMc()     { return  mIndexMc;     }
	UShort_t         indexRc()     { return  mIndexRc;     }
	UShort_t         indexParent() { return  mIndexParent; }
	UShort_t         gePid()       { return  mGePid;       }
	UShort_t         parentPid()   { return  mParentPid;   }
	Float_t          dcaXY()       { return  mDcaXY;       }
	Float_t          dcaZ()        { return  mDcaZ;        }
	Int_t            hftHitsMap()  { return  mHftHitsMap;  }

	void setMcMom       ( StThreeVectorF   McMom      ) {  mMcMom       = McMom;         }
	void setPMom        ( StThreeVectorF   PMom       ) {  mPMom        = PMom;          }
	void setGMom        ( StThreeVectorF   GMom       ) {  mGMom        = GMom;          }
	void setIndexMc     ( UShort_t         IndexMc    ) {  mIndexMc     = IndexMc;       }
	void setIndexRc     ( UShort_t         IndexRc    ) {  mIndexRc     = IndexRc;       }
	void setIndexParent ( UShort_t         IndexP     ) {  mIndexParent = IndexP;        }
	void setGePid       ( UShort_t         GePid      ) {  mGePid       = GePid;         }
	void setParentPid   ( UShort_t         ParentPid  ) {  mParentPid   = ParentPid;     }
	void setDcaXY       ( Float_t          DcaXY      ) {  mDcaXY       = DcaXY;         }
	void setDcaZ        ( Float_t          DcaZ       ) {  mDcaZ        = DcaZ;          }
	void setHftHitsMap  ( Int_t            HftHitsMap ) {  mHftHitsMap  = HftHitsMap;    }

    protected:
	UShort_t         mIndexMc;
	UShort_t         mIndexRc;
	UShort_t         mIndexParent;
	UShort_t         mGePid;
	UShort_t         mParentPid;
	StThreeVectorF   mMcMom;
	StThreeVectorF   mPMom;
	StThreeVectorF   mGMom;
	Float_t          mDcaXY;
	Float_t          mDcaZ;
	Int_t            mHftHitsMap;


	ClassDef(StMyTrack, 1)
};

#endif
