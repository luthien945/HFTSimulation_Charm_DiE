include "TH1.h"
#include "TF1.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TH2I.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "TProfile.h"
#include "TMath.h"
#include "TRandom3.h"
#include "Rtypes.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TGraphErrors.h"
#include "TString.h"
#include "TTree.h"
#include "TChain.h"
#include "TNtuple.h"
#include <vector>
#define MK 0.493677
#define MP 0.93827231
#define ME 0.00051099907
//---------------------------------------------------------------
// const values;
int const nVzs = 4;
float const VzEdge[nVzs + 1] = { -6.e4, -3.e4, 0, 3.e4, 6.e4};

int const nEtas = 5;
float const EtaEdge[nEtas + 1] = { -1.0, -0.6, -0.2, 0.2, 0.6, 1.0};

const Int_t nPtBins = 19;
const Double_t ptEdge[nPtBins + 1] =
{
      0.3, 0.4, 0.5,
      0.6,  0.7 , 0.8 , 0.9 ,
      1. , 1.25 , 1.5 , 1.75 , 2.  , 2.25 , 2.5 , 2.75 , 3.0 , 3.5,
      4.  , 6. , 12.0
};

TH1D *hMassShape; //input massShape 

TH1D *h1Vz;
//TH1D* hHftRatio1[nEtasHftRatio][nVzsHftRatio];
TH2I *h2Dca[nEtas][nVzs][nPtBins];

TChain *chain;
mTreeDst *tree;
TNtuple *nt;
TRandom3 *gRandom;

TString inFileName;
TString outFileName;
bool doCharmFlag;

//---------------------------------------------------------------
// user functions
void init();
void loadData(char *infilename = "");
void bookObject(TFile *fout);

void getDaughters(bool option, int iter, TLorentzVector &ePmom, TVector3 &ePos, TLorentzVector &eMmom, TVector3 &eMpos);
void loadCharmTree(int iter, TLorentzVector &ePmom, TVector3 &ePos, TLorentzVector &eMmom, TVector3 &eMpos);
void virtualPhotonDecay(TLorentzVector &ePmom, TLorentzVector &eMmom);

void fill(bool option, TLorentzVector const& ePmom, TVector3 const& ePpos, TLorentzVector const& eMmom, TVector3 const& eMpos);

float dca(TVector3 const& p, TVector3 const& pos, TVector3 const& vertex);
float dcaSigned(TVector3 const& p, TVector3 const& pos, TVector3 const& vertex);
float dcaXY(TVector3 const& p, TVector3 const& pos, TVector3 const& vertex);
float dcaZ(TVector3 const& p, TVector3 const& pos, TVector3 const& vertex);
float dca1To2(TVector3 const& p1, TVector3 const& pos1, TVector3 const& p2, TVector3 const& pos2, TVector3& posDca1, TVector3& posDca2, TVector3& v0);
TLorentzVector smearMom(TLorentzVector const& b, TF1 const * const fMomResolution);
TVector3 smearPos(TLorentzVector const& mom, TLorentzVector const& rMom, TVector3 const& pos);
TVector3 smearPosData(int const iParticleIndex, double const vz, int cent, TLorentzVector const& rMom, TVector3 const& pos);

int getPtIndex(double);
int getEtaIndex(double);
int getVzIndex(double);


