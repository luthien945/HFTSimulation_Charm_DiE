#include "TH1.h"
#include "TF1.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TLorentzVector.h"
#include "TProfile.h"
#include "TMath.h"
#include "TRandom3.h"
#include "Rtypes.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TGraphErrors.h"
#include <vector>

// option
//const bool DrawQA           = kTRUE;
//const bool RejectPhe        = kFALSE;
//const bool doMixedEventFlag = kTRUE;
//const bool doHFTMatch       = kTRUE;
//const bool PheFolder        = kFALSE;

TFile *fout;
TRandom3 *mRandom;
const float PI = TMath::Pi();
// Pair cuts
//const float mPairYCut = 1.;

TH2F *hMassVsParentMc;
TH2F *hMassVsParentRc;
//TH2F *hDecayLVsParent;
//TH2F *hDcaPairVsParent;
//TH2F *hDca1XYVsParent;
//TH2F *hDca1ZVsParent;
//TH2F *hDca1VsParent;
//TH2F *hDca2XYVsParent;
//TH2F *hDca2ZVsParent;
//TH2F *hDca2VsParent;
//
//TH2F *hDca1VsParentPt1;
//TH2F *hDca2VsParentPt1;
//TH2F *hDca1VsParentPt2;
//TH2F *hDca2VsParentPt2;
//TH2F *hDca1VsParentPt3;
//TH2F *hDca2VsParentPt3;

TH2F *hDcaVsPt[10]   ;
TH2F *hDcaXYVsPt[10] ;
TH2F *hDcaZVsPt[10]  ;
TH2F *hDcaPairVsMass[10];
TH2F *hDecayLVsMass[10];
TH2F *hDcaV0VsMass[10];
TH2F *hOpenAngleVsMass[10];

//TH2F *hDcaVsPtMeson   ;
//TH2F *hDcaXYVsPtMeson ;
//TH2F *hDcaZVsPtMeson  ;
//TH2F *hDcaPairVsMassMeson;
//TH2F *hDecayLVsMassMeson;
//TH2F *hDcaV0VsMassMeson;
//TH2F *hOpenAngleVsMassMeson;

TH1F *htmC = NULL;
TH1F *htmM = NULL;

float dcaCuts[10] = {0.005,0.006,0.007,0.008,0.009,0.01,0.011,0.012,0.013,0.014};
//float dcaPairCuts[10] = {0.005,0.01,0.015,0.02,0.025,0.03,0.035,0.04,0.045,0.05};
float dcaPairCuts[10] = {0.001,0.002,0.003,0.004,0.005,0.006,0.007,0.008,0.009,0.01};
