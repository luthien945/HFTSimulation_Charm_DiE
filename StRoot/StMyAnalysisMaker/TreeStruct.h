#ifndef TreeStruct_H
#define TreeStruct_H
#define MAXTRACKS 2000
#define MAX_N_PAIR 500
#include "TObject.h"

class epPair : public TObject {
    public:
	Float_t BField;
	Int_t nPairs;

	// nPairs
	Float_t   *decayL        ; //[nPairs]
	Float_t   *dcaV0         ; //[nPairs]
	Float_t   *dcaPair       ; //[nPairs]
	Float_t   *mcPairM       ; //[nPairs]
	Float_t   *pairM         ; //[nPairs]
	Float_t   *openAngle     ; //[nPairs]

	UShort_t  *gePid1       ; //[nPairs]
	UShort_t  *parentPid1   ; //[nPairs]
	UShort_t  *parentId1    ; //[nPairs]
	Float_t    *pP1x         ; //[nPairs]
	Float_t    *pP1y         ; //[nPairs]
	Float_t    *pP1z         ; //[nPairs]
	Float_t    *gP1x         ; //[nPairs]
	Float_t    *gP1y         ; //[nPairs]
	Float_t    *gP1z         ; //[nPairs]
	Float_t    *dca1XY       ; //[nPairs]
	Float_t    *dca1Z        ; //[nPairs]
	Int_t      *idtrue1_pxl1 ; //[nPairs]
	Int_t      *idtrue1_pxl2 ; //[nPairs]
	Int_t      *idtrue1_ist  ; //[nPairs]
	Int_t      *hftHitMap1   ; //[nPairs]

	UShort_t  *gePid2       ; //[nPairs]
	UShort_t  *parentPid2   ; //[nPairs]
	UShort_t  *parentId2    ; //[nPairs]
	Float_t    *pP2x         ; //[nPairs]
	Float_t    *pP2y         ; //[nPairs]
	Float_t    *pP2z         ; //[nPairs]
	Float_t    *gP2x         ; //[nPairs]
	Float_t    *gP2y         ; //[nPairs]
	Float_t    *gP2z         ; //[nPairs]
	Float_t    *dca2XY       ; //[nPairs]
	Float_t    *dca2Z        ; //[nPairs]
	Int_t      *idtrue2_pxl1 ; //[nPairs]
	Int_t      *idtrue2_pxl2 ; //[nPairs]
	Int_t      *idtrue2_ist  ; //[nPairs]
	Int_t      *hftHitMap2   ; //[nPairs]

	ClassDef(epPair, 1)
};

#endif
