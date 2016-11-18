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
TH2F *hDecayLVsParent;
TH2F *hDcaPairVsParent;
TH2F *hDca1XYVsParent;
TH2F *hDca1ZVsParent;
TH2F *hDca1VsParent;
TH2F *hDca2XYVsParent;
TH2F *hDca2ZVsParent;
TH2F *hDca2VsParent;

TH2F *hDca1VsParentPt1;
TH2F *hDca2VsParentPt1;
TH2F *hDca1VsParentPt2;
TH2F *hDca2VsParentPt2;
TH2F *hDca1VsParentPt3;
TH2F *hDca2VsParentPt3;
