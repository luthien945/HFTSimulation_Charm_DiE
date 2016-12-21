#ifndef StMyAnalysisMaker_H
#define StMyAnalysisMaker_H

#include "StMaker.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TClonesArray.h"

#include "StarClassLibrary/StThreeVectorF.hh"
#include "StEvent/StDcaGeometry.h"

#include "StPicoDstMaker/StPicoEvent.h"
#include "StPicoDstMaker/StPicoTrack.h"
#include "StPicoDstMaker/StPicoMcTrack.h"

#include "TreeStruct.h"

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
class histClass;
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
	void createArrays();
	void clearArrays();
	void initTree();
	bool isGoodEvent();
	bool isGoodTrack(StPicoTrack * trk);
	bool isCharmChild(int parentPid);
	bool isMesonChild(int parentPid);
	int  loopMcTrack();
	int  loopMcVertex();
	void makePair();
	bool isTrueMatch(StPicoMcTrack const * const trk);
	int  getParent(StPicoMcTrack const * const mcTrk, bool doTraceUp);
	int  getRcTrack(StPicoMcTrack const * const mcTrk);
	int  mcFilterAndGharge(int gePid);

	StPicoDstMaker * mPicoDstMaker;
	StPicoDst      * mPicoDst;
	StPicoEvent    * mPicoEvent;
	StPicoMcEvent  * mPicoMcEvent;

	TFile *mOutFile;
	histClass *mHist;

	StThreeVectorF pVtx;
	float bField;
	int nMcTracks;
	int nRcTracks;
	int nCharmMesons;
	int nCharmElectrons;
	int nPairs;

	TTree *mTree;
	epPair *mPairDst;

	std::vector<Int_t> rcEid;
	std::vector<Int_t> rcPid;
	std::vector<Int_t> mcEid;
	std::vector<Int_t> mcPid;

	TClonesArray *rcEP;
	TClonesArray *rcEM;

	ClassDef(StMyAnalysisMaker, 0)
};
#endif
