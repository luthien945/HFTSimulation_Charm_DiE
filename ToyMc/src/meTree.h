////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Dec 22 13:52:33 2016 by ROOT version 5.34/30// from TTree meTree/eTree
// found on file: pythiaevent01.root
//////////////////////////////////////////////////////////

#ifndef meTree_h
#define meTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class meTree {
    public :
	TTree          *fChain;   //!pointer to the analyzed TTree or TChain
	Int_t           fCurrent; //!current Tree number in a TChain

	// Declaration of leaf types
	Int_t           EventId;
	Float_t         cPt;
	Float_t         cEta;
	Float_t         cPhi;
	Float_t         cY;
	Float_t         cbarPt;
	Float_t         cbarEta;
	Float_t         cbarPhi;
	Float_t         cbarY;
	Float_t         eePt;
	Float_t         eeEta;
	Float_t         eePhi;
	Float_t         eeRapidity;
	Float_t         eeM;
	Int_t           ePosParentGID;
	Float_t         ePosPt;
	Float_t         ePosEta;
	Float_t         ePosPhi;
	Float_t         ePosX;
	Float_t         ePosY;
	Float_t         ePosZ;
	Int_t           eNegParentGID;
	Float_t         eNegPt;
	Float_t         eNegEta;
	Float_t         eNegPhi;
	Float_t         eNegX;
	Float_t         eNegY;
	Float_t         eNegZ;

	// List of branches
	TBranch        *b_EventId;   //!
	TBranch        *b_cPt;   //!
	TBranch        *b_cEta;   //!
	TBranch        *b_cPhi;   //!
	TBranch        *b_cY;   //!
	TBranch        *b_cbarPt;   //!
	TBranch        *b_cbarEta;   //!
	TBranch        *b_cbarPhi;   //!
	TBranch        *b_cbarY;   //!
	TBranch        *b_eePt;   //!
	TBranch        *b_eeEta;   //!
	TBranch        *b_eePhi;   //!
	TBranch        *b_eeRapidity;   //!
	TBranch        *b_eeM;   //!
	TBranch        *b_ePosParentGID;   //!
	TBranch        *b_ePosPt;   //!
	TBranch        *b_ePosEta;   //!
	TBranch        *b_ePosPhi;   //!
	TBranch        *b_ePosX;   //!
	TBranch        *b_ePosY;   //!
	TBranch        *b_ePosZ;   //!
	TBranch        *b_eNegParentGID;   //!
	TBranch        *b_eNegPt;   //!
	TBranch        *b_eNegEta;   //!
	TBranch        *b_eNegPhi;   //!
	TBranch        *b_eNegX;   //!
	TBranch        *b_eNegY;   //!
	TBranch        *b_eNegZ;   //!

	meTree(TTree *tree=0);
	virtual ~meTree();
	virtual Int_t    Cut(Long64_t entry);
	virtual Int_t    GetEntry(Long64_t entry);
	virtual Long64_t LoadTree(Long64_t entry);
	virtual void     Init(TTree *tree);
	virtual void     Loop();
	virtual Bool_t   Notify();
	virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef meTree_cxx
meTree::meTree(TTree *tree) : fChain(0) 
{
    // if parameter tree is not specified (or zero), connect the file
    // used to generate this class and read the Tree.
    if (tree == 0) {
	TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("pythiaevent01.root");
	if (!f || !f->IsOpen()) {
	    f = new TFile("pythiaevent01.root");
	}
	f->GetObject("meTree",tree);

    }
    Init(tree);
}

meTree::~meTree()
{
    if (!fChain) return;
    delete fChain->GetCurrentFile();
}

Int_t meTree::GetEntry(Long64_t entry)
{
    // Read contents of entry.
    if (!fChain) return 0;
    return fChain->GetEntry(entry);
}
Long64_t meTree::LoadTree(Long64_t entry)
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

void meTree::Init(TTree *tree)
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

    fChain->SetBranchAddress("EventId", &EventId, &b_EventId);
    fChain->SetBranchAddress("cPt", &cPt, &b_cPt);
    fChain->SetBranchAddress("cEta", &cEta, &b_cEta);
    fChain->SetBranchAddress("cPhi", &cPhi, &b_cPhi);
    fChain->SetBranchAddress("cY", &cY, &b_cY);
    fChain->SetBranchAddress("cbarPt", &cbarPt, &b_cbarPt);
    fChain->SetBranchAddress("cbarEta", &cbarEta, &b_cbarEta);
    fChain->SetBranchAddress("cbarPhi", &cbarPhi, &b_cbarPhi);
    fChain->SetBranchAddress("cbarY", &cbarY, &b_cbarY);
    fChain->SetBranchAddress("eePt", &eePt, &b_eePt);
    fChain->SetBranchAddress("eeEta", &eeEta, &b_eeEta);
    fChain->SetBranchAddress("eePhi", &eePhi, &b_eePhi);
    fChain->SetBranchAddress("eeRapidity", &eeRapidity, &b_eeRapidity);
    fChain->SetBranchAddress("eeM", &eeM, &b_eeM);
    fChain->SetBranchAddress("ePosParentGID", &ePosParentGID, &b_ePosParentGID);
    fChain->SetBranchAddress("ePosPt", &ePosPt, &b_ePosPt);
    fChain->SetBranchAddress("ePosEta", &ePosEta, &b_ePosEta);
    fChain->SetBranchAddress("ePosPhi", &ePosPhi, &b_ePosPhi);
    fChain->SetBranchAddress("ePosX", &ePosX, &b_ePosX);
    fChain->SetBranchAddress("ePosY", &ePosY, &b_ePosY);
    fChain->SetBranchAddress("ePosZ", &ePosZ, &b_ePosZ);
    fChain->SetBranchAddress("eNegParentGID", &eNegParentGID, &b_eNegParentGID);
    fChain->SetBranchAddress("eNegPt", &eNegPt, &b_eNegPt);
    fChain->SetBranchAddress("eNegEta", &eNegEta, &b_eNegEta);
    fChain->SetBranchAddress("eNegPhi", &eNegPhi, &b_eNegPhi);
    fChain->SetBranchAddress("eNegX", &eNegX, &b_eNegX);
    fChain->SetBranchAddress("eNegY", &eNegY, &b_eNegY);
    fChain->SetBranchAddress("eNegZ", &eNegZ, &b_eNegZ);
    Notify();
}

Bool_t meTree::Notify()
{
    // The Notify() function is called when a new file is opened. This
    // can be either for a new TTree in a TChain or when when a new TTree
    // is started when using PROOF. It is normally not necessary to make changes
    // to the generated code, but the routine can be extended by the
    // user if needed. The return value is currently not used.

    return kTRUE;
}

void meTree::Show(Long64_t entry)
{
    // Print contents of entry.
    // If entry is not specified, print current entry
    if (!fChain) return;
    fChain->Show(entry);
}
Int_t meTree::Cut(Long64_t entry)
{
    // This function may be called from Loop.
    // returns  1 if entry is accepted.
    // returns -1 otherwise.
    return 1;
}
#endif // #ifdef meTree_cxx
