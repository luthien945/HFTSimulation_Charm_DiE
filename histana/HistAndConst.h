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
char *set[3] = {"charm", "kineE", "meson"};
// Pair cuts
//const float mPairYCut = 1.;

TH2F *hMassVsParentMc;
TH2F *hMassVsParentRc;

TH2F *hDcaXYVsPt[3];
TH2F *hDcaZVsPt[3];
TH2F *hDcaVsPt[3];

TH2F *hDeltaPhiVsMass[3];
TH2F *hDeltaEtaVsMass[3];

TH2F *hDcaPVsMass0[3];
TH2F *hDcaPVsMass1[3]; // normalized

void bookHistograms(){
    hMassVsParentRc = new TH2F("hMassVsParentRc", "", 1000, 0, 5, 10, 0, 10);
    hMassVsParentMc = new TH2F("hMassVsParentMc", "", 1000, 0, 5, 10, 0, 10);
    for(int i=0;i<2;i++) {

	hDeltaPhiVsMass[i] = new TH2F(Form("hDeltaPhiVsMass_%s", set[i]), "", 360, -0.5*PI, 1.5*PI, 500, 0, 5.);
	hDeltaEtaVsMass[i] = new TH2F(Form("hDeltaEtaVsMass_%s", set[i]), "", 200, -2., 2., 500, 0, 5.);

	hDcaXYVsPt[i] = new TH2F(Form("hDcaXYVsPt_%s", set[i]), "", 500, -0.05, 0.05, 200, 0, 6);
	hDcaZVsPt[i]  = new TH2F(Form("hDcaZVsPt_%s", set[i]), "", 500, -0.05, 0.05, 200, 0, 6);
	hDcaVsPt[i]   = new TH2F(Form("hDcaVsPt_%s", set[i]), "", 500, -0.05, 0.05, 200, 0, 6);

	hDcaPVsMass0[i] = new TH2F(Form("hDcaPVsMass0_%s", set[i]), "", 500, 0, 0.05, 500, 0, 5);
	hDcaPVsMass1[i] = new TH2F(Form("hDcaPVsMass1_%s", set[i]), "", 500, 0, 0.05, 500, 0, 5);
    }
}

