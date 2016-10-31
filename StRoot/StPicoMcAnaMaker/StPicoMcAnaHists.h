#ifndef StPicoMcAnaHists__h
#define StPicoMcAnaHists__h

/* **************************************************
 *  A class to create and save my D0 analysis histograms.
 *
 *  Authors: Mustafa Mustafa (mmustafa@lbl.gov)
 *
 * **************************************************
 */

#include "TObject.h"
#include "StLorentzVectorF.hh"
#include "TH2.h"
#include "TH3.h"
class TH1F;
class TH2F;
class TH3F;
class TF1;
class TFile;
class TString;
class StPicoPrescales;
class StPicoEvent;
class StPicoMcTrack;
class StPicoTrack;
class StKaonPion;
class StRandom;

class StPicoMcAnaHists: public TObject
{
  public:
  StPicoMcAnaHists(const char * fileBaseName);
   virtual ~StPicoMcAnaHists(){;};
   void closeFile();
   void addEvent(StPicoEvent const * const event);
   //void addMcTrack(StPicoMcTrack const * const);
   //void addRcTrack(StPicoTrack const * const, StPicoEvent const * const);
   void addMatchedTrack(StPicoEvent const * const, StPicoMcTrack const * const, StPicoTrack const * const);
   void addRcTrack(StPicoEvent const * const, StPicoTrack const * const);
   void addRcTrack_Tight(StPicoEvent const * const, StPicoTrack const * const);
   void addMatchedTrack_Tight(StPicoEvent const * const, StPicoMcTrack const * const, StPicoTrack const * const);
   void addMatchedVertex(StThreeVectorF, StThreeVectorF );
  private:
   //
   bool keepTPCTrack(float phi, float eta);
   TFile* mOutFile;
   //
   TH2* mHftRatioDenom;
   TH2* mHftRatioNum;
   TH2* mHftMatchedRatioDenom;
   TH2* mHftMatchedRatioNum;
   TH2* mHftMatchedRatioDenom_idTruth;
   TH2* mHftMatchedRatioNum_idTruth;
   //
   TH2* mHftRatioDenom_tight;
   TH2* mHftRatioNum_tight;
   //Vertex position
   TH3* mVtx;
   //
   TH1* mTpcFitPoints;
   TH1* mHFTFitPoints;
   TH2* mHftTracksEtaPhi;
   TH2* mTpcTracksEtaPhi;
   //
   TH2* mDCAxy_pT;
   TH2* mDCAz_pT;
   //
   TH2* mDCAxy_pT_idTruth;
   TH2* mDCAz_pT_idTruth;
   //
   TH2* tpcProb;
   StRandom* mRandom;
   //
   vector<int> nPxlInnerHits;
   vector<int> nPxlOuterHits;
   vector<int> nIstHits;
   vector<int> nTpcTracks;
   ClassDef(StPicoMcAnaHists, 1)
};
#endif
