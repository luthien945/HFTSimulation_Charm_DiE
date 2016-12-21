//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Nov 28 13:44:50 2016 by ROOT version 5.34/30
// from TTree mPairDst/mPairDst
// found on file: ../hadd/charm_hijing_pythia6.root
//////////////////////////////////////////////////////////

#ifndef mPairDst_h
#define mPairDst_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <TObject.h>
#define MAX_N_PAIR 500

// Fixed size dimensions of array or collections stored in the TTree if any.

class mPairDst {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
 //epPair          *mPairDst;
   UInt_t          fUniqueID;
   UInt_t          fBits;
   Float_t         BField;
   Int_t           nPairs;
   Float_t         decayLMc[MAX_N_PAIR];   //[nPairs]
   Float_t         parDisMc[MAX_N_PAIR];   //[nPairs]
   Float_t         decayL[MAX_N_PAIR];   //[nPairs]
   Float_t         dcaV0[MAX_N_PAIR];   //[nPairs]
   Float_t         dcaPair[MAX_N_PAIR];   //[nPairs]
   Float_t         mcPairM[MAX_N_PAIR];   //[nPairs]
   Float_t         pairM[MAX_N_PAIR];   //[nPairs]
   Float_t         openAngle[MAX_N_PAIR];   //[nPairs]
   UShort_t        gePid1[MAX_N_PAIR];   //[nPairs]
   UShort_t        parentPid1[MAX_N_PAIR];   //[nPairs]
   UShort_t        parentId1[MAX_N_PAIR];   //[nPairs]
   Float_t         pP1x[MAX_N_PAIR];   //[nPairs]
   Float_t         pP1y[MAX_N_PAIR];   //[nPairs]
   Float_t         pP1z[MAX_N_PAIR];   //[nPairs]
   Float_t         gP1x[MAX_N_PAIR];   //[nPairs]
   Float_t         gP1y[MAX_N_PAIR];   //[nPairs]
   Float_t         gP1z[MAX_N_PAIR];   //[nPairs]
   Float_t         dca1XY[MAX_N_PAIR];   //[nPairs]
   Float_t         dca1Z[MAX_N_PAIR];   //[nPairs]
   Int_t           idtrue1_pxl1[MAX_N_PAIR];   //[nPairs]
   Int_t           idtrue1_pxl2[MAX_N_PAIR];   //[nPairs]
   Int_t           idtrue1_ist[MAX_N_PAIR];   //[nPairs]
   Int_t           hftHitMap1[MAX_N_PAIR];   //[nPairs]
   UShort_t        gePid2[MAX_N_PAIR];   //[nPairs]
   UShort_t        parentPid2[MAX_N_PAIR];   //[nPairs]
   UShort_t        parentId2[MAX_N_PAIR];   //[nPairs]
   Float_t         pP2x[MAX_N_PAIR];   //[nPairs]
   Float_t         pP2y[MAX_N_PAIR];   //[nPairs]
   Float_t         pP2z[MAX_N_PAIR];   //[nPairs]
   Float_t         gP2x[MAX_N_PAIR];   //[nPairs]
   Float_t         gP2y[MAX_N_PAIR];   //[nPairs]
   Float_t         gP2z[MAX_N_PAIR];   //[nPairs]
   Float_t         dca2XY[MAX_N_PAIR];   //[nPairs]
   Float_t         dca2Z[MAX_N_PAIR];   //[nPairs]
   Int_t           idtrue2_pxl1[MAX_N_PAIR];   //[nPairs]
   Int_t           idtrue2_pxl2[MAX_N_PAIR];   //[nPairs]
   Int_t           idtrue2_ist[MAX_N_PAIR];   //[nPairs]
   Int_t           hftHitMap2[MAX_N_PAIR];   //[nPairs]

   // List of branches
   TBranch        *b_mPairDst_fUniqueID;   //!
   TBranch        *b_mPairDst_fBits;   //!
   TBranch        *b_mPairDst_BField;   //!
   TBranch        *b_mPairDst_nPairs;   //!
   TBranch        *b_decayLMc;   //!
   TBranch        *b_parDisMc;   //!
   TBranch        *b_decayL;   //!
   TBranch        *b_dcaV0;   //!
   TBranch        *b_dcaPair;   //!
   TBranch        *b_mcPairM;   //!
   TBranch        *b_pairM;   //!
   TBranch        *b_openAngle;   //!
   TBranch        *b_gePid1;   //!
   TBranch        *b_parentPid1;   //!
   TBranch        *b_parentId1;   //!
   TBranch        *b_pP1x;   //!
   TBranch        *b_pP1y;   //!
   TBranch        *b_pP1z;   //!
   TBranch        *b_gP1x;   //!
   TBranch        *b_gP1y;   //!
   TBranch        *b_gP1z;   //!
   TBranch        *b_dca1XY;   //!
   TBranch        *b_dca1Z;   //!
   TBranch        *b_idtrue1_pxl1;   //!
   TBranch        *b_idtrue1_pxl2;   //!
   TBranch        *b_idtrue1_ist;   //!
   TBranch        *b_hftHitMap1;   //!
   TBranch        *b_gePid2;   //!
   TBranch        *b_parentPid2;   //!
   TBranch        *b_parentId2;   //!
   TBranch        *b_pP2x;   //!
   TBranch        *b_pP2y;   //!
   TBranch        *b_pP2z;   //!
   TBranch        *b_gP2x;   //!
   TBranch        *b_gP2y;   //!
   TBranch        *b_gP2z;   //!
   TBranch        *b_dca2XY;   //!
   TBranch        *b_dca2Z;   //!
   TBranch        *b_idtrue2_pxl1;   //!
   TBranch        *b_idtrue2_pxl2;   //!
   TBranch        *b_idtrue2_ist;   //!
   TBranch        *b_hftHitMap2;   //!

   mPairDst(TTree *tree=0);
   virtual ~mPairDst();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef mPairDst_cxx
mPairDst::mPairDst(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("../hadd/charm_hijing_pythia6.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("../hadd/charm_hijing_pythia6.root");
      }
      f->GetObject("mPairDst",tree);

   }
   Init(tree);
}

mPairDst::~mPairDst()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t mPairDst::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t mPairDst::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void mPairDst::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("fUniqueID", &fUniqueID, &b_mPairDst_fUniqueID);
   fChain->SetBranchAddress("fBits", &fBits, &b_mPairDst_fBits);
   fChain->SetBranchAddress("BField", &BField, &b_mPairDst_BField);
   fChain->SetBranchAddress("nPairs", &nPairs, &b_mPairDst_nPairs);
   fChain->SetBranchAddress("decayLMc", decayLMc, &b_decayLMc);
   fChain->SetBranchAddress("parDisMc", parDisMc, &b_parDisMc);
   fChain->SetBranchAddress("decayL", decayL, &b_decayL);
   fChain->SetBranchAddress("dcaV0", dcaV0, &b_dcaV0);
   fChain->SetBranchAddress("dcaPair", dcaPair, &b_dcaPair);
   fChain->SetBranchAddress("mcPairM", mcPairM, &b_mcPairM);
   fChain->SetBranchAddress("pairM", pairM, &b_pairM);
   fChain->SetBranchAddress("openAngle", openAngle, &b_openAngle);
   fChain->SetBranchAddress("gePid1", gePid1, &b_gePid1);
   fChain->SetBranchAddress("parentPid1", parentPid1, &b_parentPid1);
   fChain->SetBranchAddress("parentId1", parentId1, &b_parentId1);
   fChain->SetBranchAddress("pP1x", pP1x, &b_pP1x);
   fChain->SetBranchAddress("pP1y", pP1y, &b_pP1y);
   fChain->SetBranchAddress("pP1z", pP1z, &b_pP1z);
   fChain->SetBranchAddress("gP1x", gP1x, &b_gP1x);
   fChain->SetBranchAddress("gP1y", gP1y, &b_gP1y);
   fChain->SetBranchAddress("gP1z", gP1z, &b_gP1z);
   fChain->SetBranchAddress("dca1XY", dca1XY, &b_dca1XY);
   fChain->SetBranchAddress("dca1Z", dca1Z, &b_dca1Z);
   fChain->SetBranchAddress("idtrue1_pxl1", idtrue1_pxl1, &b_idtrue1_pxl1);
   fChain->SetBranchAddress("idtrue1_pxl2", idtrue1_pxl2, &b_idtrue1_pxl2);
   fChain->SetBranchAddress("idtrue1_ist", idtrue1_ist, &b_idtrue1_ist);
   fChain->SetBranchAddress("hftHitMap1", hftHitMap1, &b_hftHitMap1);
   fChain->SetBranchAddress("gePid2", gePid2, &b_gePid2);
   fChain->SetBranchAddress("parentPid2", parentPid2, &b_parentPid2);
   fChain->SetBranchAddress("parentId2", parentId2, &b_parentId2);
   fChain->SetBranchAddress("pP2x", pP2x, &b_pP2x);
   fChain->SetBranchAddress("pP2y", pP2y, &b_pP2y);
   fChain->SetBranchAddress("pP2z", pP2z, &b_pP2z);
   fChain->SetBranchAddress("gP2x", gP2x, &b_gP2x);
   fChain->SetBranchAddress("gP2y", gP2y, &b_gP2y);
   fChain->SetBranchAddress("gP2z", gP2z, &b_gP2z);
   fChain->SetBranchAddress("dca2XY", dca2XY, &b_dca2XY);
   fChain->SetBranchAddress("dca2Z", dca2Z, &b_dca2Z);
   fChain->SetBranchAddress("idtrue2_pxl1", idtrue2_pxl1, &b_idtrue2_pxl1);
   fChain->SetBranchAddress("idtrue2_pxl2", idtrue2_pxl2, &b_idtrue2_pxl2);
   fChain->SetBranchAddress("idtrue2_ist", idtrue2_ist, &b_idtrue2_ist);
   fChain->SetBranchAddress("hftHitMap2", hftHitMap2, &b_hftHitMap2);
   Notify();
}

Bool_t mPairDst::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void mPairDst::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t mPairDst::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef mPairDst_cxx
