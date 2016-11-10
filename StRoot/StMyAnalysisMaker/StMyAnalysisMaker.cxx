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
#include "../StPicoDstMaker/StPicoMcVertex.h"

#include "StMyAnalysisMaker.h"
#include "StMyTrack.h"
#include "histClass.h"
#include "Cuts.h"
#include "TMath.h"

ClassImp(StMyAnalysisMaker)

StMyAnalysisMaker::StMyAnalysisMaker(char const *name, StPicoDstMaker *picoMaker, char const* filename)
: StMaker(name), mPicoDstMaker(picoMaker), mPicoDst(NULL), mPicoEvent(NULL) 
{
    mOutFile = TFile::Open(filename, "recreate");
}

StMyAnalysisMaker::~StMyAnalysisMaker() {
}

Int_t StMyAnalysisMaker::Init() {
    if(!mOutFile && !mOutFile->IsOpen()) {
	cout<<"No output file!!!"<<endl;
    }
    mHist = new histClass(mOutFile);
    mHist->initHist();
    createArrays();
    return kStOK;
}

void StMyAnalysisMaker::Clear(Option_t *opt) {
    mPicoDst = NULL;
    mPicoEvent = NULL;
    mPicoMcEvent = NULL;
    nCharmMesons = 0;
    nCharmElectrons = 0;

    clearArrays();
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
    mPicoMcEvent = mPicoDst->mcevent();

    mHist->hEvtCounter->Fill(0);
    if(!isGoodEvent()) return kStOK;
    mHist->hEvtCounter->Fill(1);
    loopMcTrack();

    if(nCharmMesons>=1) mHist->hEvtCounter->Fill(2);
    if(nCharmMesons>=2) mHist->hEvtCounter->Fill(3);
    if(nCharmMesons>=3) mHist->hEvtCounter->Fill(4);

    if(nCharmElectrons>=1) mHist->hEvtCounter->Fill(6);
    if(nCharmElectrons>=2) mHist->hEvtCounter->Fill(7);
    if(nCharmElectrons>=3) mHist->hEvtCounter->Fill(8);
    //loopMcVertex();

    return kStOK;
}

Int_t StMyAnalysisMaker::Finish() {
    mHist->writeHist();
    if(mHist) delete mHist;
    if(rcEP) delete rcEP;
    if(rcEM) delete rcEM;
    return kStOK;
}


//---------------------------------------------------------
// other utility function
//---------------------------------------------------------
void StMyAnalysisMaker::createArrays() {
    rcEP = new TClonesArray("StMyTrack", 100);
    rcEM = new TClonesArray("StMyTrack", 100);
}

void StMyAnalysisMaker::clearArrays() {
    rcEP->Clear();
    rcEM->Clear();
}

bool StMyAnalysisMaker::isGoodEvent()
{
    pVtx = mPicoEvent->primaryVertex();
    int refMult = mPicoEvent->refMult();
    int gRefMult= mPicoEvent->grefMult();
    bField = mPicoEvent->bField();

    StThreeVectorF pVtxMc = mPicoDst->mcevent()->pVertex();

    StThreeVectorF vtxDiff = pVtx-pVtxMc;

    mHist->hMc_VxVy->Fill(pVtxMc.x(), pVtxMc.y());
    mHist->hMc_Vz->Fill(pVtxMc.z());
    mHist->hRc_VxVy->Fill(pVtx.x(), pVtx.y());
    mHist->hRc_Vz->Fill(pVtx.z());

    mHist->hVxDiff->Fill(vtxDiff.x());
    mHist->hVyDiff->Fill(vtxDiff.y());
    mHist->hVzDiff->Fill(vtxDiff.z());

    mHist->hRefMult->Fill(refMult);
    mHist->hgRefMult->Fill(gRefMult);

    return fabs(pVtx.z()) < cuts::vz ;
}

int StMyAnalysisMaker::loopMcTrack() {
    nMcTracks = mPicoDst->numberOfMcTracks();

    //cout<<"Number of mctracks : "<<nMcTracks<<endl;
    for(int itMc=0; itMc<nMcTracks; itMc++) {
	StPicoMcTrack *mcTrk = (StPicoMcTrack*) mPicoDst->mctrack(itMc);
	StThreeVectorF startVertex = mcTrk->Origin();
	StThreeVectorF pVtxMc = mPicoDst->mcevent()->pVertex();
	bool isPrimary = kFALSE;
	if(abs(startVertex-pVtxMc)<1e-5) isPrimary = kTRUE;
	//cout<<itMc<<" "<<mcTrk->mcId()<<endl;
	int gePid = mcTrk->GePid();
	int parentId = getParent(mcTrk, kFALSE);
	int ancestorId = getParent(mcTrk, kTRUE);

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

	if(gePid>=12037 && gePid<=12044) {
	    mHist->hPid_CharmPid->Fill(gePid);
	    //cout<<"Charmed : "<<gePid<<endl;
	    nCharmMesons++;
	}

	if(parentGeId)mHist->hPid_GenIdVsParentGenId->Fill(gePid, parentGeId);
	if(mcTrk->parentId()==mcTrk->mcId()) mHist->hPid_GenIdPrimary->Fill(gePid);

	if(parentGeId>=12037 && parentGeId<=12044) {
	    mHist->hPid_GenIdVsCharmPid->Fill(gePid, parentGeId);
	    //cout<<"A charm decays : "<<parentGeId<<"->"<<gePid<<endl;
	}

	// only pi, k, p and e 
	int mcCharge = mcFilterAndGharge(gePid);
	if(mcCharge==0) continue;

	StThreeVectorF mcMom = mcTrk->Mom();
	StThreeVectorF mcOrigin = mcTrk->Origin();
	float mcPt = mcMom.perp();
	float mcEta = mcMom.pseudoRapidity();
	float mcPhi = mcMom.phi();
	float mcPhi0 = mcPhi + (mcPhi<0 ? 2*TMath::Pi() : 0);
	int   mcNhitsTpc = mcTrk->hitsTpc();

	bool isGoodMcTrack = mcPt>cuts::ptMin && fabs(mcEta)<1. && mcNhitsTpc>20 ;
	if(isGoodMcTrack) {
	    StPhysicalHelixD mcHelix(mcMom, mcOrigin, bField, mcCharge);
	    StThreeVectorF mcDcaPoint = mcHelix.at(mcHelix.pathLength(pVtx.x(), pVtx.y()));
	    double mcDcaZ  = mcDcaPoint.z()-pVtx.z();
	    double mcDcaXY = mcHelix.geometricSignedDistance(pVtx.x(), pVtx.y());
	    double mcDca   = TMath::Sqrt(mcDcaZ*mcDcaZ+mcDcaXY*mcDcaXY);
	    mcDca *= mcDcaXY>=0 ? 1. : -1.;

	    if(!(parentGeId>=cuts::parentGidCut1 && parentGeId<=cuts::parentGidCut2)) {
		mHist->hMc_DcaXYVsPt->Fill(mcDcaXY, mcPt);
		mHist->hMc_DcaZVsPt->Fill(mcDcaZ, mcPt);
		mHist->hMc_DcaVsPt->Fill(mcDca, mcPt);

		mHist->hMc_PtVsPhi->Fill(mcPt*mcCharge, mcPhi0);
		mHist->hMc_EtaVsPhi->Fill(mcEta, mcPhi0);
		bool mcPxl1 = mcTrk->hitsPxl1()>0;
		bool mcPxl2 = mcTrk->hitsPxl2()>0;
		bool mcIst  = mcTrk->hitsIst()>0;

		mHist->hMc_PtVsHFTMatch->Fill(mcPt, 0);
		if(mcPxl1) mHist->hMc_PtVsHFTMatch->Fill(mcPt,1);
		if(mcPxl1 && mcPxl2) mHist->hMc_PtVsHFTMatch->Fill(mcPt, 2);
		if(mcPxl1 && mcPxl2 && mcIst) mHist->hMc_PtVsHFTMatch->Fill(mcPt, 3);
	    }

	    if(gePid == 2 || gePid == 3) {
		mHist->hMc_incl_DcaXYVsPt->Fill(mcDcaXY, mcPt);
		mHist->hMc_incl_DcaZVsPt->Fill(mcDcaZ, mcPt);
		mHist->hMc_incl_DcaVsPt->Fill(mcDca, mcPt);
		if(parentGeId>=cuts::parentGidCut1 && parentGeId<=cuts::parentGidCut2) {
		    nCharmElectrons++;
		    mHist->hMc_charm_DcaXYVsPt->Fill(mcDcaXY, mcPt);
		    mHist->hMc_charm_DcaZVsPt->Fill(mcDcaZ, mcPt);
		    mHist->hMc_charm_DcaVsPt->Fill(mcDca, mcPt);
		}
	    }
	}

	int nTpcCommonHits = mcTrk->tpcCommon();
	int rcIndex = getRcTrack(mcTrk);

	if(rcIndex<=0) continue;
	StPicoTrack *rcTrk = (StPicoTrack *) mPicoDst->track(rcIndex);
	StThreeVectorF rcGMom = rcTrk->gMom(pVtx, bField);
	StThreeVectorF rcPMom = rcTrk->pMom();
	float rcPt        = rcPMom.perp();
	float rcEta       = rcPMom.pseudoRapidity();
	float rcPhi       = rcPMom.phi();

	float rcGPt       = rcGMom.perp();
	float rcGEta      = rcGMom.pseudoRapidity();
	float rcGPhi      = rcGMom.phi();

	float rcPhi0      = rcPhi + (rcPhi<0 ? 2*TMath::Pi() : 0);

	int   nHitsTpc    = rcTrk->nHitsFit();
	int   nHitsMapHFT = rcTrk->nHitsMapHFT();
	int   rcCharge    = rcTrk->charge();
	bool  rcPxl1      = nHitsMapHFT>>0 & 0x1;
	bool  rcPxl2      = nHitsMapHFT>>1 & 0x3;
	bool  rcIst       = nHitsMapHFT>>3 & 0x3;
	if(rcCharge == 0) continue;

	bool isGoodRcTrack = rcPt>cuts::ptMin && fabs(rcEta)<1. && nTpcCommonHits>10
	    && nHitsTpc>20;

	if(isGoodRcTrack) {
	    StPhysicalHelixD rcHelix = rcTrk->dcaGeometry().helix();

	    StThreeVectorF rcDcaPoint = rcHelix.at(rcHelix.pathLength(pVtx.x(), pVtx.y()));
	    double rcDcaZ  = rcDcaPoint.z()-pVtx.z();
	    double rcDcaXY = rcHelix.geometricSignedDistance(pVtx.x(), pVtx.y());
	    double rcDca   = TMath::Sqrt(rcDcaZ*rcDcaZ+rcDcaXY*rcDcaXY);
	    rcDca *= rcDcaXY>=0 ? 1. : -1.;

	    if(gePid == 2) {
		int counts = rcEP->GetEntries();
		new((*rcEP)[counts]) StMyTrack(gePid, parentGeId, rcDcaXY, rcDcaZ, mcMom, rcPMom, rcGMom);
	    }
	    if(gePid == 3) {
		int counts = rcEM->GetEntries();
		new((*rcEM)[counts]) StMyTrack(gePid, parentGeId, rcDcaXY, rcDcaZ, mcMom, rcPMom, rcGMom);
	    }

	    if(!(parentGeId>=cuts::parentGidCut1 && parentGeId<=cuts::parentGidCut2) && gePid != 2 && gePid != 3) {
		mHist->hRc_DcaXYVsPt->Fill(rcDcaXY, rcPt);
		mHist->hRc_DcaZVsPt->Fill(rcDcaZ, rcPt);
		mHist->hRc_DcaVsPt->Fill(rcDca, rcPt);

		mHist->hRc_PtVsPhi->Fill(rcPt*rcCharge, rcPhi0);
		mHist->hRc_EtaVsPhi->Fill(rcEta, rcPhi0);

		mHist->hRc_PtVsHFTMatch->Fill(rcPt, 0);
		if(rcPxl1) mHist->hRc_PtVsHFTMatch->Fill(rcPt,1);
		if(rcPxl1 && rcPxl2) mHist->hRc_PtVsHFTMatch->Fill(rcPt, 2);
		if(rcPxl1 && rcPxl2 && rcIst) {
		    mHist->hRc_PtVsHFTMatch->Fill(rcPt, 3);
		    mHist->hRc_HFTMatched_PtVsPhi->Fill(rcPt*rcCharge, rcPhi0);
		    mHist->hRc_HFTMatched_EtaVsPhi->Fill(rcEta, rcPhi0);

		    mHist->hRc_HFTMatched_DcaVsPt->Fill(rcDca, rcPt);  
		    mHist->hRc_HFTMatched_DcaXYVsPt->Fill(rcDcaXY, rcPt);
		    mHist->hRc_HFTMatched_DcaZVsPt->Fill(rcDcaZ, rcPt);
		}
	    }
	    if(parentGeId>=cuts::parentGidCut1 
		    && parentGeId<=cuts::parentGidCut2
		    && (gePid == 2 || gePid ==3 )){
		mHist->hRc_charm_PtVsHFTMatch->Fill(rcGPt, 0);
		if(rcPxl1) mHist->hRc_charm_PtVsHFTMatch->Fill(rcGPt,1);
		if(rcPxl1 && rcPxl2) mHist->hRc_charm_PtVsHFTMatch->Fill(rcGPt, 2);
		if(rcPxl1 && rcPxl2 && rcIst) mHist->hRc_charm_PtVsHFTMatch->Fill(rcGPt, 3);
	    }

	    if(gePid == 2 || gePid == 3) {
		if(isPrimary) {
		    mHist->hRc_incl_DcaXYVsPt->Fill(rcDcaXY, rcPt);
		    mHist->hRc_incl_DcaZVsPt->Fill(rcDcaZ, rcPt);
		    mHist->hRc_incl_DcaVsPt->Fill(rcDca, rcPt);
		}
		if(parentGeId>=cuts::parentGidCut1 && parentGeId<=cuts::parentGidCut2) {
		    mHist->hRc_charm_DcaXYVsPt->Fill(rcDcaXY, rcPt);
		    mHist->hRc_charm_DcaZVsPt->Fill(rcDcaZ, rcPt);
		    mHist->hRc_charm_DcaVsPt->Fill(rcDca, rcPt);
		}
	    }
	}

    }
}

int StMyAnalysisMaker::loopMcVertex() {
    int nMcVertices = mPicoDst->numberOfMcVertices();
    cout<<"Number of mcvertices : "<<nMcVertices<<endl;

    for(int itMc = 0; itMc<nMcVertices; itMc++) {
	StPicoMcVertex *mcVtx = (StPicoMcVertex*) mPicoDst->mcvertex(itMc);
	int parentId = mcVtx->parentId();
	if(parentId==Pico::SHORTMAX) continue;
	//parentId = mPicoMcEvent->mcKey2PicoId().at(parentId);
	parentId -= 1;
	StPicoMcTrack *mcParentTrk = (StPicoMcTrack *) mPicoDst->mctrack(parentId);
	int parentGeId = -999;
	if(mcParentTrk) parentGeId = mcParentTrk->GePid();

	if(parentGeId>=12037 && parentGeId<=12044) {
	    cout<<"Vertex : "<<itMc<<endl
		<<"  ["<<std::setw(5)<<parentId<<", "<<std::setw(5)<<parentGeId<<"]-->";
	    if(mcVtx->nDaughters()<=0) continue;
	    for(int i=0; i<mcVtx->nDaughters(); i++) {
		int daughterId = mcVtx->daughterId(i);
		//daughterId = mPicoMcEvent->mcKey2PicoId().at(daughterId);
		daughterId -= 1;
		StPicoMcTrack *mcDaughterTrk = (StPicoMcTrack *) mPicoDst->mctrack(daughterId); 
		if(!mcDaughterTrk) continue; 
		int daughterGeId = mcDaughterTrk->GePid();
		cout<<"["<<std::setw(5)<<daughterId<<", "<<std::setw(5)<<daughterGeId<<"]";
	    }
	    cout<<endl;
	}
    }
}

int StMyAnalysisMaker::mcFilterAndGharge(int gePid) {
    // I only like pi, k, p and e
    if(gePid == 2 || gePid == 8 || gePid == 11 || gePid == 14) return 1;
    if(gePid == 3 || gePid == 9 || gePid == 12 || gePid == 15) return -1;
    return 0;
}

int StMyAnalysisMaker::getParent(StPicoMcTrack const * const mcTrk, bool doTraceUp) {
    if(mcTrk->parentId()==Pico::USHORTMAX || mcTrk->parentId()==mcTrk->mcId()) return -999;

    int parentId = mcTrk->parentId();
    //parentId = mPicoMcEvent->mcKey2PicoId().at(parentId);
    parentId -= 1;
    StPicoMcTrack *mcParentTrack = (StPicoMcTrack*) mPicoDst->mctrack(parentId);
    if(parentId != mcParentTrack->parentId() && doTraceUp) parentId = getParent(mcParentTrack, doTraceUp);
    return parentId;
}

int StMyAnalysisMaker::getRcTrack(StPicoMcTrack const * const mcTrk) {
    int rcTrkIndex = -999;
    int assoId = mcTrk->assoId();
    int nRcTracks = mPicoDst->numberOfTracks();
    for(int i=0; i<nRcTracks; i++) {
	StPicoTrack *rcTrk = (StPicoTrack *) mPicoDst->track(i);
	if(assoId == rcTrk->id()) {
	    rcTrkIndex = i;
	    break;
	}   
    }
    return rcTrkIndex;
}
