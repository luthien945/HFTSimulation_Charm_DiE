#ifndef StMyAnalysisMaker_H
#define StMyAnalysisMaker_H

#include "StMaker.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"

#include "StarClassLibrary/StThreeVectorF.hh"
#include "StEvent/StDcaGeometry.h"

#include "StPicoDstMaker/StPicoEvent.h"
#include "StPicoDstMaker/StPicoTrack.h"
#include "StPicoDstMaker/StPicoMcTrack.h"

class TTree;
class TFile;
class TH1F;
class TH2F;

class StPicoDst;
class StPicoDstMaker;
class StPicoEvent;
class StPicoTrack;
class StPicoMcEvent;
class StPicoMcTrack;
class StPicoMcVertex;
class TString;
class StDcaGeometry;

class StMyAnalysisMaker : public StMaker {
    public:
	StMyAnalysisMaker(char const* name, StPicoDstMaker* picoMaker, char const* filename);
	virtual ~StMyAnalysisMaker();

	virtual Int_t Init();
	virtual Int_t Make();
	virtual void  Clear(Option_t *opt="");
	virtual Int_t Finish();
    private:
	bool isGoodEvent();
	int  loopMcTrack();
	int  loopMcVertex();
	int  getParent(StPicoMcTrack const * const mcTrk, bool doTraceUp);

	StPicoDstMaker * mPicoDstMaker;
	StPicoDst      * mPicoDst;
	StPicoEvent    * mPicoEvent;
	StPicoMcEvent  * mPicoMcEvent;

	TFile *mOutFile;

	StThreeVectorF pVtx;
	float bField;
	int nMcTracks;
	int nRcTracks;

	std::vector<Int_t> rcEid;
	std::vector<Int_t> rcPid;
	std::vector<Int_t> mcEid;
	std::vector<Int_t> mcPid;

	ClassDef(StMyAnalysisMaker, 0)
};
#endif
