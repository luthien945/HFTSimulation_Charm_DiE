#ifndef TreeStruct_H
#define TreeStruct_H
#define MAXTRACKS 2000
#define MAX_N_PAIR 500
#include "TObject.h"

class epPair : public TObject {
    public:
	float BField;
	Int_t nPairs;

	// nPairs
	float   *decayL        ; //[nPairs]
	float   *dcaV0         ; //[nPairs]
	float   *dcaPair       ; //[nPairs]
	float   *mcPairM       ; //[nPairs]
	float   *pairM         ; //[nPairs]
	float   *openAngle     ; //[nPairs]

	UShort_t *gePid1       ; //[nPairs]
	UShort_t *parentPid1   ; //[nPairs]
	UShort_t *parentId1    ; //[nPairs]
	float    *pP1x         ; //[nPairs]
	float    *pP1y         ; //[nPairs]
	float    *pP1z         ; //[nPairs]
	float    *gP1x         ; //[nPairs]
	float    *gP1y         ; //[nPairs]
	float    *gP1z         ; //[nPairs]
	float    *dca1XY       ; //[nPairs]
	float    *dca1Z        ; //[nPairs]
	bool     *idtrue1_pxl1 ; //[nPairs]
	bool     *idtrue1_pxl2 ; //[nPairs]
	bool     *idtrue1_ist  ; //[nPairs]
	int      *hftHitMap1   ; //[nPairs]

	UShort_t *gePid2       ; //[nPairs]
	UShort_t *parentPid2   ; //[nPairs]
	UShort_t *parentId2    ; //[nPairs]
	float    *pP2x         ; //[nPairs]
	float    *pP2y         ; //[nPairs]
	float    *pP2z         ; //[nPairs]
	float    *gP2x         ; //[nPairs]
	float    *gP2y         ; //[nPairs]
	float    *gP2z         ; //[nPairs]
	float    *dca2XY       ; //[nPairs]
	float    *dca2Z        ; //[nPairs]
	bool     *idtrue2_pxl1 ; //[nPairs]
	bool     *idtrue2_pxl2 ; //[nPairs]
	bool     *idtrue2_ist  ; //[nPairs]
	int      *hftHitMap2   ; //[nPairs]

	ClassDef(epPair, 1)
};

#endif
