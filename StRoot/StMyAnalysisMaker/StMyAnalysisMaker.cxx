#include "TVector3.h"
#include "TTree.h"
#include "TFile.h"
#include "TString.h"
#include "StThreeVectorF.hh"
#include "StLorentzVectorF.hh"
#include "TLorentzVector.h"
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

#include <vector>

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
    mOutFile->cd();
    mHist = new histClass(mOutFile);
    mHist->initHist();
    initTree();
    createArrays();
    return kStOK;
}

void StMyAnalysisMaker::Clear(Option_t *opt) {
    mPicoDst = NULL;
    mPicoEvent = NULL;
    mPicoMcEvent = NULL;
    nCharmMesons = 0;
    nCharmElectrons = 0;
    nPairs = 0;

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

    mHist->hMc_Charm_NeVsNc->Fill(nCharmElectrons, nCharmMesons);
    makePair();
    //loopMcVertex();

    mPairDst->nPairs = nPairs;
    mPairDst->BField = bField;

    mTree->Fill();

    return kStOK;
}

Int_t StMyAnalysisMaker::Finish() {
    mHist->writeHist();
    mOutFile->cd();
    mTree->Write();
    if(mOutFile) {
	//mOutFile->Write();
	mOutFile->Close();
    }
    if(mHist) delete mHist;
    //if(mTree) delete mTree;
    if(mPairDst) delete mPairDst;
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

bool StMyAnalysisMaker::isTrueMatch(StPicoMcTrack const * const trk) {
    return trk->Pxl1Truth() && trk->Pxl2Truth() && trk->IstTruth();
}

void StMyAnalysisMaker::initTree() {
    mOutFile->cd();
    mTree = new TTree("mPairDst", "mPairDst");
    mPairDst = new epPair();

    // nPairs
    mPairDst->decayLMc     = new Float_t   [MAX_N_PAIR]; //[nPairs]
    mPairDst->parDisMc     = new Float_t   [MAX_N_PAIR]; //[nPairs]
    mPairDst->decayL       = new Float_t   [MAX_N_PAIR]; //[nPairs]
    mPairDst->dcaV0        = new Float_t   [MAX_N_PAIR]; //[nPairs]
    mPairDst->dcaPair      = new Float_t   [MAX_N_PAIR]; //[nPairs]
    mPairDst->mcPairM      = new Float_t   [MAX_N_PAIR]; //[nPairs]
    mPairDst->pairM        = new Float_t   [MAX_N_PAIR]; //[nPairs]
    mPairDst->openAngle    = new Float_t   [MAX_N_PAIR]; //[nPairs]

    mPairDst->gePid1       = new UShort_t[MAX_N_PAIR]; //[nPairs]
    mPairDst->parentPid1   = new UShort_t[MAX_N_PAIR]; //[nPairs]
    mPairDst->parentId1    = new UShort_t[MAX_N_PAIR]; //[nPairs]
    mPairDst->pP1x         = new Float_t   [MAX_N_PAIR]; //[nPairs]
    mPairDst->pP1y         = new Float_t   [MAX_N_PAIR]; //[nPairs]
    mPairDst->pP1z         = new Float_t   [MAX_N_PAIR]; //[nPairs]
    mPairDst->gP1x         = new Float_t   [MAX_N_PAIR]; //[nPairs]
    mPairDst->gP1y         = new Float_t   [MAX_N_PAIR]; //[nPairs]
    mPairDst->gP1z         = new Float_t   [MAX_N_PAIR]; //[nPairs]
    mPairDst->dca1XY       = new Float_t   [MAX_N_PAIR]; //[nPairs]
    mPairDst->dca1Z        = new Float_t   [MAX_N_PAIR]; //[nPairs]
    mPairDst->idtrue1_pxl1 = new Int_t    [MAX_N_PAIR]; //[nPairs]
    mPairDst->idtrue1_pxl2 = new Int_t    [MAX_N_PAIR]; //[nPairs]
    mPairDst->idtrue1_ist  = new Int_t    [MAX_N_PAIR]; //[nPairs]
    mPairDst->hftHitMap1   = new Int_t     [MAX_N_PAIR]; //[nPairs]

    mPairDst->gePid2       = new UShort_t[MAX_N_PAIR]; //[nPairs]
    mPairDst->parentPid2   = new UShort_t[MAX_N_PAIR]; //[nPairs]
    mPairDst->parentId2    = new UShort_t[MAX_N_PAIR]; //[nPairs]
    mPairDst->pP2x         = new Float_t   [MAX_N_PAIR]; //[nPairs]
    mPairDst->pP2y         = new Float_t   [MAX_N_PAIR]; //[nPairs]
    mPairDst->pP2z         = new Float_t   [MAX_N_PAIR]; //[nPairs]
    mPairDst->gP2x         = new Float_t   [MAX_N_PAIR]; //[nPairs]
    mPairDst->gP2y         = new Float_t   [MAX_N_PAIR]; //[nPairs]
    mPairDst->gP2z         = new Float_t   [MAX_N_PAIR]; //[nPairs]
    mPairDst->dca2XY       = new Float_t   [MAX_N_PAIR]; //[nPairs]
    mPairDst->dca2Z        = new Float_t   [MAX_N_PAIR]; //[nPairs]
    mPairDst->idtrue2_pxl1 = new Int_t    [MAX_N_PAIR]; //[nPairs]
    mPairDst->idtrue2_pxl2 = new Int_t    [MAX_N_PAIR]; //[nPairs]
    mPairDst->idtrue2_ist  = new Int_t    [MAX_N_PAIR]; //[nPairs]
    mPairDst->hftHitMap2   = new Int_t     [MAX_N_PAIR]; //[nPairs]

    mTree->Branch("mPairDst", &mPairDst);
    mTree->SetAutoSave(10000);
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
    float PI = TMath::Pi();
    nMcTracks = mPicoDst->numberOfMcTracks();

    LOG_DEBUG <<"Number of mctracks : "<<nMcTracks<<endl;
    LOG_DEBUG <<"Number of rctracks : "<<mPicoDst->numberOfTracks()<<endl;

    std::vector<int> indexMcCharm;
    std::vector<int> mcCharmPid;
    std::vector<int> orgId;
    for(int itMc=0; itMc<nMcTracks; itMc++) {
	StPicoMcTrack *mcTrk = (StPicoMcTrack*) mPicoDst->mctrack(itMc);
	StThreeVectorF startVertex = mcTrk->Origin();
	StThreeVectorF pVtxMc = mPicoDst->mcevent()->pVertex();
	bool isPrimary = kFALSE;
	if((startVertex-pVtxMc).mag()<1e-8) isPrimary = kTRUE;
	//cout<<itMc<<" "<<mcTrk->mcId()<<endl;
	int gePid = mcTrk->GePid();
	int parentId = getParent(mcTrk, kFALSE)-1; // Pico index = mcId-1
	int ancestorId = getParent(mcTrk, kTRUE)-1;

	int parentGeId = -999;
	int ancestorGeId = -999;
	float parDis = 0; 
	if(parentId>=0 && parentId<=Pico::USHORTMAX) {
	    StPicoMcTrack *mcParentTrk = (StPicoMcTrack*) mPicoDst->mctrack(parentId);
	    StThreeVectorF momPar = mcParentTrk->Mom();
	    StThreeVectorF vPar = mcParentTrk->Origin();
	    StThreeVectorF diff = startVertex-vPar;
	    parentGeId = mcParentTrk->GePid();
	    parDis = (vPar-pVtxMc).mag();
	    if(parentGeId>=cuts::parentGidCut1 && parentGeId<=cuts::parentGidCut2) {
		if(gePid == 2 || gePid == 3) {
		    mHist->hDecayLMcVsP_charm->Fill(diff.mag(), momPar.mag());
		    mHist->hParDisCharm->Fill(parDis, parentGeId-cuts::parentGidCut1);
		    mHist->hMc_Charm_PtVsPid->Fill(momPar.perp(), parentGeId);
		    mHist->hMc_Charm_PhiVsPid->Fill(momPar.phi(), parentGeId);
		    mHist->hMc_Charm_EtaVsPid->Fill(momPar.pseudoRapidity(), parentGeId);
		    if(parentId == ancestorId) {
			indexMcCharm.push_back(parentId);
			mcCharmPid.push_back(parentGeId);
			orgId.push_back(ancestorId);
		    }

		}
	    }
	    if(parentGeId>=cuts::parentGidHCut1 && parentGeId<=cuts::parentGidHCut2) {
		if(gePid == 2 || gePid == 3) {
		    mHist->hDecayLMcVsP_meson->Fill(diff.mag(), momPar.mag());
		    mHist->hParDisMeson->Fill(parDis, parentGeId-cuts::parentGidHCut1);
		}
	    }
	}

	if(parDis>0.001) continue; // primary parents

	bool isKineEP = (gePid == 2 || gePid == 3) && parentId<0 && isPrimary; 
	if(isKineEP) parentGeId = 0;

	//if( (gePid == 2 || gePid == 3) && parentId<0 && isPrimary) cout<<"YiSaid : Kine input e+/e- : "<<gePid<<" "<<parentId<<endl;

	if(ancestorId>=0 && ancestorId<=Pico::USHORTMAX) {
	    StPicoMcTrack *mcAncestorTrk = (StPicoMcTrack*) mPicoDst->mctrack(ancestorId);
	    ancestorGeId = mcAncestorTrk->GePid();
	}

	if(gePid>=cuts::parentGidCut1 && gePid<=cuts::parentGidCut2) {
	    mHist->hPid_CharmPid->Fill(gePid);
	    LOG_DEBUG <<"Charmed : "<<gePid<<endl;
	    //nCharmMesons++;
	}

	if(parentGeId)mHist->hPid_GenIdVsParentGenId->Fill(gePid, parentGeId);
	if(mcTrk->parentId()==mcTrk->mcId()) mHist->hPid_GenIdPrimary->Fill(gePid);

	if(parentGeId>=12037 && parentGeId<=12044) {
	    mHist->hPid_GenIdVsCharmPid->Fill(gePid, parentGeId);
	    LOG_DEBUG <<"A charm decays : "<<parentGeId<<"->"<<gePid<<endl;
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
	StPhysicalHelixD mcHelix(mcMom, mcOrigin, bField, mcCharge);
	StThreeVectorF mcDcaPoint = mcHelix.at(mcHelix.pathLength(pVtx.x(), pVtx.y()));
	double mcDcaZ  = mcDcaPoint.z()-pVtx.z();
	double mcDcaXY = mcHelix.geometricSignedDistance(pVtx.x(), pVtx.y());
	double mcDca   = TMath::Sqrt(mcDcaZ*mcDcaZ+mcDcaXY*mcDcaXY);
	mcDca *= mcDcaXY>=0 ? 1. : -1.;

	bool mcPxl1 = mcTrk->hitsPxl1()>0;
	bool mcPxl2 = mcTrk->hitsPxl2()>0;
	bool mcIst  = mcTrk->hitsIst()>0;

	if(isGoodMcTrack) {
	    if(!(parentGeId>=cuts::parentGidCut1 && parentGeId<=cuts::parentGidCut2)) {
		mHist->hMc_DcaXYVsPt->Fill(mcDcaXY, mcPt);
		mHist->hMc_DcaZVsPt->Fill(mcDcaZ, mcPt);
		mHist->hMc_DcaVsPt->Fill(mcDca, mcPt);

		mHist->hMc_PtVsPhi->Fill(mcPt*mcCharge, mcPhi0);
		mHist->hMc_EtaVsPhi->Fill(mcEta, mcPhi0);

		mHist->hMc_PtVsHFTMatch->Fill(mcPt, 0);
		if(mcPxl1) mHist->hMc_PtVsHFTMatch->Fill(mcPt,1);
		if(mcPxl1 && mcPxl2) mHist->hMc_PtVsHFTMatch->Fill(mcPt, 2);
		if(mcPxl1 && mcPxl2 && mcIst) mHist->hMc_PtVsHFTMatch->Fill(mcPt, 3);
	    }

	    if(gePid == 2 || gePid == 3) {
		if(parentGeId>=cuts::parentGidCut1 && parentGeId<=cuts::parentGidCut2) {
		    nCharmElectrons++;
		    mHist->hMc_charm_DcaXYVsPt->Fill(mcDcaXY, mcPt);
		    mHist->hMc_charm_DcaZVsPt->Fill(mcDcaZ, mcPt);
		    mHist->hMc_charm_DcaVsPt->Fill(mcDca, mcPt);
		}
		if(parentGeId>=cuts::parentGidHCut1 && parentGeId<=cuts::parentGidHCut2) {
		    mHist->hMc_incl_DcaXYVsPt->Fill(mcDcaXY, mcPt);
		    mHist->hMc_incl_DcaZVsPt->Fill(mcDcaZ, mcPt);
		    mHist->hMc_incl_DcaVsPt->Fill(mcDca, mcPt);
		}
	    }
	}

	int nTpcCommonHits = mcTrk->tpcCommon();
	int rcIndex = getRcTrack(mcTrk);
	StThreeVectorF rcGMom(0, 0, 0);
	StThreeVectorF rcPMom(0, 0, 0);
	float rcDcaZ = -999;
	float rcDcaXY = -999;
	float rcDca = -999;

	int   nHitsMapHFT = 0; 
	bool  rcPxl1      = kFALSE; 
	bool  rcPxl2      = kFALSE;
	bool  rcIst       = kFALSE;

	if(rcIndex>=0 && rcIndex<Pico::USHORTMAX) {
	    StPicoTrack *rcTrk = (StPicoTrack *) mPicoDst->track(rcIndex);
	    rcGMom = rcTrk->gMom(pVtx, bField);
	    rcPMom = rcTrk->pMom();
	    float rcPt        = rcPMom.perp();
	    float rcEta       = rcPMom.pseudoRapidity();
	    float rcPhi       = rcPMom.phi();

	    float rcGPt       = rcGMom.perp();
	    float rcGEta      = rcGMom.pseudoRapidity();
	    float rcGPhi      = rcGMom.phi();

	    float rcPhi0      = rcPhi + (rcPhi<0 ? 2*TMath::Pi() : 0);
	    float rcGPhi0     = rcGPhi + (rcGPhi<0 ? 2*TMath::Pi() : 0);

	    int   nHitsTpc    = rcTrk->nHitsFit();
	    int   rcCharge    = rcTrk->charge();


	    nHitsMapHFT = rcTrk->nHitsMapHFT();
	    rcPxl1      = nHitsMapHFT>>0 & 0x1;
	    rcPxl2      = nHitsMapHFT>>1 & 0x3;
	    rcIst       = nHitsMapHFT>>3 & 0x3;

	    //if(rcCharge == 0) continue;

	    StPhysicalHelixD rcHelix = rcTrk->dcaGeometry().helix();

	    StThreeVectorF rcDcaPoint = rcHelix.at(rcHelix.pathLength(pVtx.x(), pVtx.y()));
	    rcDcaZ  = rcDcaPoint.z()-pVtx.z();
	    rcDcaXY = rcHelix.geometricSignedDistance(pVtx.x(), pVtx.y());
	    rcDca   = TMath::Sqrt(rcDcaZ*rcDcaZ+rcDcaXY*rcDcaXY);
	    //rcDca *= rcDcaXY>=0 ? 1. : -1.;

	    //bool isGoodRcTrack = rcPt>cuts::ptMin && fabs(rcEta)<1. && nTpcCommonHits>10 && nHitsTpc>20;
	    bool isGoodRcTrack = isGoodTrack(rcTrk);
	    bool isHFTM = rcPxl1 && rcPxl2 && rcIst;

	    if(isGoodRcTrack) {
		mHist->hPtVsDiffPt->Fill(rcPt, (rcPt-mcPt)/rcPt);
		if(!(parentGeId>=cuts::parentGidCut1 && parentGeId<=cuts::parentGidCut2) && gePid != 2 && gePid != 3) {
		    mHist->hRc_DcaXYVsPt->Fill(rcDcaXY, rcGPt);
		    mHist->hRc_DcaZVsPt->Fill(rcDcaZ, rcGPt);
		    mHist->hRc_DcaVsPt->Fill(rcDca, rcGPt);

		    mHist->hRc_PtVsPhi->Fill(rcGPt*rcCharge, rcGPhi0);
		    mHist->hRc_EtaVsPhi->Fill(rcGEta, rcGPhi0);

		    mHist->hRc_PtVsHFTMatch->Fill(rcGPt, 0);
		    if(rcPxl1) mHist->hRc_PtVsHFTMatch->Fill(rcGPt,1);
		    if(rcPxl1 && rcPxl2) mHist->hRc_PtVsHFTMatch->Fill(rcGPt, 2);
		    if(rcPxl1 && rcPxl2 && rcIst) {
			mHist->hRc_PtVsHFTMatch->Fill(rcGPt, 3);
			mHist->hRc_HFTMatched_PtVsPhi->Fill(rcGPt*rcCharge, rcGPhi0);
			mHist->hRc_HFTMatched_EtaVsPhi->Fill(rcGEta, rcGPhi0);

			mHist->hRc_HFTMatched_DcaVsPt->Fill(rcDca, rcGPt);  
			mHist->hRc_HFTMatched_DcaXYVsPt->Fill(rcDcaXY, rcGPt);
			mHist->hRc_HFTMatched_DcaZVsPt->Fill(rcDcaZ, rcGPt);
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

		if((gePid == 2 || gePid == 3) && isHFTM) {
		    if(parentGeId>=cuts::parentGidCut1 && parentGeId<=cuts::parentGidCut2) {
			mHist->hRc_charm_DcaXYVsPt->Fill(rcDcaXY, rcGPt);
			mHist->hRc_charm_DcaZVsPt->Fill(rcDcaZ, rcGPt);
			mHist->hRc_charm_DcaVsPt->Fill(rcDca, rcGPt);
			if(isTrueMatch(mcTrk)){
			    mHist->hRc_charm_DcaVsPt_TrueM->Fill(rcDca, rcGPt);
			    mHist->hRc_charm_DcaXYVsPt_TrueM->Fill(rcDcaXY, rcGPt);
			    mHist->hRc_charm_DcaZVsPt_TrueM->Fill(rcDcaZ, rcGPt);
			}
		    }
		    if(parentGeId>=cuts::parentGidHCut1 && parentGeId<=cuts::parentGidHCut2) {
			mHist->hRc_incl_DcaXYVsPt->Fill(rcDcaXY, rcGPt);
			mHist->hRc_incl_DcaZVsPt->Fill(rcDcaZ, rcGPt);
			mHist->hRc_incl_DcaVsPt->Fill(rcDca, rcGPt);
			if(isTrueMatch(mcTrk)){
			    mHist->hRc_incl_DcaVsPt_TrueM->Fill(rcDca, rcGPt);
			    mHist->hRc_incl_DcaXYVsPt_TrueM->Fill(rcDcaXY, rcGPt);
			    mHist->hRc_incl_DcaZVsPt_TrueM->Fill(rcDcaZ, rcGPt);
			}
		    }
		}
		if(isHFTM && isPrimary) {
		    if(gePid == 2) {
			mHist->hDcaVsPt_Pos[0]->Fill(rcDca, rcGPt);
			mHist->hDcaXYVsPt_Pos[0]->Fill(rcDcaXY, rcGPt);
			mHist->hDcaZVsPt_Pos[0]->Fill(rcDcaZ, rcGPt);
		    }
		    if(gePid == 8 || gePid == 9) {
			mHist->hDcaVsPt_Pos[1]->Fill(rcDca, rcGPt);
			mHist->hDcaXYVsPt_Pos[1]->Fill(rcDcaXY, rcGPt);
			mHist->hDcaZVsPt_Pos[1]->Fill(rcDcaZ, rcGPt);
		    }
		    if(gePid == 11 || gePid == 12) {
			mHist->hDcaVsPt_Pos[2]->Fill(rcDca, rcGPt);
			mHist->hDcaXYVsPt_Pos[2]->Fill(rcDcaXY, rcGPt);
			mHist->hDcaZVsPt_Pos[2]->Fill(rcDcaZ, rcGPt);
		    }
		    if(gePid == 14 || gePid == 15) {
			mHist->hDcaVsPt_Pos[3]->Fill(rcDca, rcGPt);
			mHist->hDcaXYVsPt_Pos[3]->Fill(rcDcaXY, rcGPt);
			mHist->hDcaZVsPt_Pos[3]->Fill(rcDcaZ, rcGPt);
		    }
		    
		    if(gePid == 3) {
			mHist->hDcaVsPt_Neg[0]->Fill(rcDca, rcGPt);
			mHist->hDcaXYVsPt_Neg[0]->Fill(rcDcaXY, rcGPt);
			mHist->hDcaZVsPt_Neg[0]->Fill(rcDcaZ, rcGPt);
		    }
		    if(gePid == 9) {
			mHist->hDcaVsPt_Neg[1]->Fill(rcDca, rcGPt);
			mHist->hDcaXYVsPt_Neg[1]->Fill(rcDcaXY, rcGPt);
			mHist->hDcaZVsPt_Neg[1]->Fill(rcDcaZ, rcGPt);
		    }
		    if(gePid == 12) {
			mHist->hDcaVsPt_Neg[2]->Fill(rcDca, rcGPt);
			mHist->hDcaXYVsPt_Neg[2]->Fill(rcDcaXY, rcGPt);
			mHist->hDcaZVsPt_Neg[2]->Fill(rcDcaZ, rcGPt);
		    }
		    if(gePid == 15) {
			mHist->hDcaVsPt_Neg[3]->Fill(rcDca, rcGPt);
			mHist->hDcaXYVsPt_Neg[3]->Fill(rcDcaXY, rcGPt);
			mHist->hDcaZVsPt_Neg[3]->Fill(rcDcaZ, rcGPt);
		    }
		}
	    }
	}

	bool whosChild = isCharmChild(parentGeId) || isMesonChild(parentGeId) || isKineEP;
	if(whosChild && isGoodMcTrack) {
	    if(gePid == 2) {
		int counts = rcEP->GetEntries();
		new((*rcEP)[counts]) StMyTrack(itMc, rcIndex, gePid, parentId, parentGeId, rcDcaXY, rcDcaZ, mcMom, rcPMom, rcGMom, nHitsMapHFT);
	    }
	    if(gePid == 3) {
		int counts = rcEM->GetEntries();
		new((*rcEM)[counts]) StMyTrack(itMc, rcIndex, gePid, parentId, parentGeId, rcDcaXY, rcDcaZ, mcMom, rcPMom, rcGMom, nHitsMapHFT);
	    }
	}

    }

    std::sort(indexMcCharm.begin(), indexMcCharm.end());
    indexMcCharm.erase(std::unique(indexMcCharm.begin(), indexMcCharm.end()), indexMcCharm.end());

    nCharmMesons = indexMcCharm.size();

    if(indexMcCharm.size() != 2) {
	return kStOK; 
    }

    if(indexMcCharm[0] == indexMcCharm[1]) {
	return kStOK;
    }

    StPicoMcTrack *mC0 = (StPicoMcTrack*) mPicoDst->mctrack(indexMcCharm[0]);
    StPicoMcTrack *mC1 = (StPicoMcTrack*) mPicoDst->mctrack(indexMcCharm[1]);
    if(mC0 && mC1) {
	int Pid0 = mC0->GePid();
	int Pid1 = mC1->GePid();
	StThreeVectorF mom0 = mC0->Mom();
	StThreeVectorF mom1 = mC1->Mom();
	float dPhi = mom0.phi()-mom1.phi();
	float dEta = mom0.pseudoRapidity()-mom1.pseudoRapidity();
	if(dPhi<-0.5*PI) dPhi+=2*PI;
	if(dPhi>1.5*PI) dPhi-=2*PI;

	mHist->hMc_Charm_Corr->Fill(dPhi, dEta);
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


void StMyAnalysisMaker::makePair() {
    int nEM = rcEM->GetEntries(); 
    int nEP = rcEP->GetEntries();

    for(int itEM=0; itEM<nEM; itEM++) {
	StMyTrack *emtrk = (StMyTrack *) rcEM->UncheckedAt(itEM);
	if(!emtrk) continue;
	int indexEM     = emtrk->indexMc();
	int parentPidEM = emtrk->parentPid();
	int indexPEM    = emtrk->indexParent();;
	int rcIndexEM   = emtrk->indexRc();

	int parMaskEM = 0;
	if(parentPidEM == 0) parMaskEM = 3;
	else if(isCharmChild(parentPidEM)) parMaskEM = 1;
	else if(isMesonChild(parentPidEM)) parMaskEM = 2;


	if(!parMaskEM) continue;

	for(int itEP=0; itEP<nEP; itEP++) {
	    StMyTrack *eptrk = (StMyTrack *) rcEP->UncheckedAt(itEP);
	    if(!eptrk) continue;
	    int indexEP     = eptrk->indexMc();
	    int parentPidEP = eptrk->parentPid();
	    int indexPEP    = eptrk->indexParent();;
	    int rcIndexEP   = eptrk->indexRc();     

	    if(parMaskEM == 3 && parentPidEP != 0) continue;
	    if(parMaskEM == 2 && indexPEM != indexPEP) continue; 
	    if(parMaskEM == 1 && !isCharmChild(parentPidEP)) continue;
	    //cout<<"a Pair ["<<parentPidEP<<", "<<parentPidEM<<"]"<<" ["<<indexPEP<<","<<indexPEM<<"] ["<<rcIndexEP<<" "<<rcIndexEM<<"]"<<endl;

	    StPicoMcTrack *emMcTrk = (StPicoMcTrack *) mPicoDst->mctrack(indexEM);
	    StPicoMcTrack *epMcTrk = (StPicoMcTrack *) mPicoDst->mctrack(indexEP);

	    float dcaXYEM = emtrk->dcaXY();
	    float dcaZEM  = emtrk->dcaZ();

	    float dcaXYEP = eptrk->dcaXY();
	    float dcaZEP  = eptrk->dcaZ();

	    StThreeVectorF mcEM = emtrk->mcMom(); 
	    StThreeVectorF mcEP = eptrk->mcMom();

	    StThreeVectorF gEM = emtrk->gMom(); 
	    StThreeVectorF gEP = eptrk->gMom();

	    StThreeVectorF pEM = emtrk->pMom(); 
	    StThreeVectorF pEP = eptrk->pMom();

	    TLorentzVector mcPair(0,0,0,0);
	    TLorentzVector rcPair(0,0,0,0);
	    TLorentzVector rcGPair(0,0,0,0);

	    TLorentzVector mce  ( 0 , 0 , 0 , 0);
	    TLorentzVector mcp  ( 0 , 0 , 0 , 0);
	    TLorentzVector rce  ( 0 , 0 , 0 , 0);
	    TLorentzVector rcp  ( 0 , 0 , 0 , 0);
	    TLorentzVector rcge ( 0 , 0 , 0 , 0);
	    TLorentzVector rcgp ( 0 , 0 , 0 , 0);


	    mce.SetXYZM(mcEM.x(), mcEM.y(), mcEM.z(), cuts::massE); 
	    mcp.SetXYZM(mcEP.x(), mcEP.y(), mcEP.z(), cuts::massE); 
	    rce.SetXYZM(pEM.x(), pEM.y(), pEM.z(), cuts::massE); 
	    rcp.SetXYZM(pEP.x(), pEP.y(), pEP.z(), cuts::massE); 
	    rcge.SetXYZM(gEM.x(), gEM.y(), gEM.z(), cuts::massE); 
	    rcgp.SetXYZM(gEP.x(), gEP.y(), gEP.z(), cuts::massE); 

	    mcPair  = mce+mcp;
	    rcPair  = rce+rcp;
	    rcGPair = rcge+rcgp;

	    float decayL = -999;
	    float dcaV0  = -999;
	    float dcaPair = -999;
	    float rcPairM = -999;
	    float openangle = -999;
	    float decayLMc = -999;

	    if(rcIndexEM>=0 && rcIndexEP>=0 && rcIndexEM<Pico::USHORTMAX && rcIndexEP<Pico::USHORTMAX) {
		StPicoTrack *emRcTrk = (StPicoTrack*) mPicoDst->track(rcIndexEM);
		StPicoTrack *epRcTrk = (StPicoTrack*) mPicoDst->track(rcIndexEP);

		if(isGoodTrack(emRcTrk) && isGoodTrack(epRcTrk)) {
		    rcPairM = rcPair.M();
		    StPhysicalHelixD ele = emRcTrk->helix();
		    StPhysicalHelixD pos = epRcTrk->helix();

		    pairD pair = pos.pathLengths(ele);
		    double posLength = pair.first;
		    double eleLength = pair.second;

		    StThreeVector<double> posOrigin = pos.at(posLength);
		    StThreeVector<double> eleOrigin = ele.at(eleLength);
		    StThreeVector<double> v0        = (posOrigin+eleOrigin)*0.5;
		    StThreeVector<double> diffe     = posOrigin-eleOrigin;
		    dcaPair = diffe.mag();

		    StThreeVector<double> posV0Mom = pos.momentumAt(posLength, bField);
		    StThreeVector<double> eleV0Mom = ele.momentumAt(eleLength, bField);

		    openangle = posV0Mom.phi()-eleV0Mom.phi(); 
		    double PI = TMath::Pi();
		    if(openangle<-PI) openangle += 2*PI;
		    if(openangle>PI)  openangle -= 2*PI;

		    decayL = (v0 - pVtx).mag();
		    TVector3 v0L((v0-pVtx).x(), (v0-pVtx).y(), (v0-pVtx).z());
		    TVector3 Apos(posV0Mom.x(), posV0Mom.y(), posV0Mom.z()); 
		    TVector3 Aele(eleV0Mom.x(), eleV0Mom.y(), eleV0Mom.z()); 
		    TVector3 Apair = Apos+Aele;

		    float costheta =  Apair.Unit().Dot(v0L.Unit());
		    decayL *= costheta<0 ? -1: 1;
		    dcaV0 = decayL*costheta;

		    if(parMaskEM == 2) {
			StPicoMcTrack *parMcTrk = (StPicoMcTrack *) mPicoDst->mctrack(indexPEM);
			StThreeVectorF pVtxMc = mPicoDst->mcevent()->pVertex();
			StThreeVectorF v0LMc = parMcTrk->Origin()-pVtxMc;
			decayLMc = v0LMc.mag();
		    }
		    //if(parentPidEP == 0 && parentPidEM == 0)
		    //	cout<<parentPidEP<<" "<<dcaPair<<" "<<decayLMc<<" "<<decayL<<" "<<rcPairM <<" "<<mcPair.M()<<endl;
		}
	    }

	    mPairDst->mcPairM[nPairs]   = mcPair.M();
	    mPairDst->decayL[nPairs]    = decayL;
	    mPairDst->dcaV0[nPairs]     = dcaV0;
	    mPairDst->dcaPair[nPairs]   = dcaPair;
	    mPairDst->openAngle[nPairs] = openangle;
	    mPairDst->pairM[nPairs]     = rcPairM;

	    mPairDst->gePid1[nPairs]       = 3;
	    mPairDst->parentPid1[nPairs]   = parentPidEM;
	    mPairDst->parentId1[nPairs]    = indexPEM;
	    mPairDst->pP1x[nPairs]         = pEM.x();
	    mPairDst->pP1y[nPairs]         = pEM.y();
	    mPairDst->pP1z[nPairs]         = pEM.z();
	    mPairDst->gP1x[nPairs]         = gEM.x();
	    mPairDst->gP1y[nPairs]         = gEM.y();
	    mPairDst->gP1z[nPairs]         = gEM.z();
	    mPairDst->dca1XY[nPairs]       = dcaXYEM;
	    mPairDst->dca1Z[nPairs]        = dcaZEM;
	    mPairDst->idtrue1_pxl1[nPairs] = emMcTrk->Pxl1Truth();
	    mPairDst->idtrue1_pxl2[nPairs] = emMcTrk->Pxl2Truth();
	    mPairDst->idtrue1_ist[nPairs]  = emMcTrk->IstTruth();
	    mPairDst->hftHitMap1[nPairs]   = emtrk->hftHitsMap();

	    mPairDst->gePid2[nPairs]       = 2;
	    mPairDst->parentPid2[nPairs]   = parentPidEP;
	    mPairDst->parentId2[nPairs]    = indexPEP;
	    mPairDst->pP2x[nPairs]         = pEP.x();
	    mPairDst->pP2y[nPairs]         = pEP.y();
	    mPairDst->pP2z[nPairs]         = pEP.z();
	    mPairDst->gP2x[nPairs]         = gEP.x();
	    mPairDst->gP2y[nPairs]         = gEP.y();
	    mPairDst->gP2z[nPairs]         = gEP.z();
	    mPairDst->dca2XY[nPairs]       = dcaXYEP;
	    mPairDst->dca2Z[nPairs]        = dcaZEP;
	    mPairDst->idtrue2_pxl1[nPairs] = epMcTrk->Pxl1Truth();
	    mPairDst->idtrue2_pxl2[nPairs] = epMcTrk->Pxl2Truth();
	    mPairDst->idtrue2_ist[nPairs]  = epMcTrk->IstTruth();
	    mPairDst->hftHitMap2[nPairs]   = eptrk->hftHitsMap();

	    nPairs++;
	}
    }
}

bool StMyAnalysisMaker::isGoodTrack(StPicoTrack * trk) {
    if(!trk) return kFALSE;
    if(trk->charge()==0) return kFALSE;
    StThreeVectorF mom = trk->pMom();
    return mom.perp()>0.2 && fabs(mom.pseudoRapidity())<1. && trk->nHitsFit()>20 ;
}


bool StMyAnalysisMaker::isCharmChild(int parentPid) {
    return parentPid>=cuts::parentGidCut1 && parentPid<=cuts::parentGidCut2;
}

bool StMyAnalysisMaker::isMesonChild(int parentPid) {
    return parentPid>=cuts::parentGidHCut1 && parentPid<=cuts::parentGidHCut2;
}

int StMyAnalysisMaker::mcFilterAndGharge(int gePid) {
    // I only like pi, k, p and e
    if(gePid == 2 || gePid == 8 || gePid == 11 || gePid == 14) return 1;
    if(gePid == 3 || gePid == 9 || gePid == 12 || gePid == 15) return -1;
    return 0;
}

int StMyAnalysisMaker::getParent(StPicoMcTrack const * const mcTrk, bool doTraceUp) {
    // search for parent. parent should be primary.
    if(mcTrk->parentId()==Pico::USHORTMAX || mcTrk->parentId()==mcTrk->mcId()) {
	if(!doTraceUp) return -999;
	return mcTrk->mcId();
    }

    int parentId = mcTrk->parentId();
    //parentId = mPicoMcEvent->mcKey2PicoId().at(parentId);
    //parentId -= 1;
    StPicoMcTrack *mcParentTrack = (StPicoMcTrack*) mPicoDst->mctrack(parentId-1);
    //StThreeVectorF startVertex = mcParentTrack->Origin();
    //StThreeVectorF pVtxMc = mPicoDst->mcevent()->pVertex();
    //if((startVertex-pVtxMc).mag()>1e-8) return -999; // parent should be primary

    if(mcParentTrack->mcId() != mcParentTrack->parentId() && doTraceUp) parentId = getParent(mcParentTrack, doTraceUp);

    return parentId;
}

int StMyAnalysisMaker::getRcTrack(StPicoMcTrack const * const mcTrk) {
    int rcTrkIndex = Pico::USHORTMAX;
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
