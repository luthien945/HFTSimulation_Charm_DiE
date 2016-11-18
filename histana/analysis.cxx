#define Mkaon 0.493677
#define Mproton 0.93827231
#define Melectron 0.00051099907

#include <iostream>
#include <fstream>
#include <cstdlib>
#include "sys/types.h"
#include "dirent.h"

#include "math.h"
#include "string.h"
#include "HistAndConst.h"
#include "TMath.h"
//Add the data structure
#include "mPairDst.h"
#include <iomanip>

//#include <stdio.h> 

#ifndef __CINT__  
#include "TROOT.h"
#include "TFile.h" 
#include "TChain.h"
#include "TMath.h"
#include "TH1.h" 
#include "TH2.h"   
#include "TH3.h" 
#include "TString.h"
#include "TF1.h" 
#include "TStyle.h"
#include "TCanvas.h"
#include "TProfile.h"
#include "TVector2.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TRandom.h"
#include "TRandom3.h"
#include "TUnixSystem.h"
#include "TVector3.h"
#include "TLorentzVector.h"
//#include "StPhysicalHelixD.h"
#include "SystemOfUnits.h"

using std::cout;
using std::endl;
using std::setw;
#endif 

void init();
bool isHftM(int hitmap);
void bookHistograms();

int main(int argc, char** argv){
    if(argc!=1&&argc!=4) {
	cout<<"Usage: analysis [inDir] [outfile]"<<endl;
	return -1;
    }
    char *inFile = "test.list";
    char outFile[100];
    sprintf(outFile,"test2.root");

    if(argc==3){
	inFile = argv[1];
    }

    fout = new TFile(outFile,"recreate");
    fout->cd();
    //--------------------------------------------------
    // initializa
    //--------------------------------------------------
    init();
    bookHistograms();
    //---------------------------------------------------
    // open files and add to the chain
    //---------------------------------------------------
    TChain *chain = new TChain("mPairDst");

    int ifile = 0;
    char filename[512];
    ifstream *inputStream = new ifstream;
    inputStream->open(inFile);
    if(!(inputStream)){
	cout<<"Can't open list file!!"<<endl;
	return 0;
    }
    while(inputStream->good()){
	inputStream->getline(filename,512);
	if(inputStream->good()){
	    TFile *ftmp = new TFile(filename);
	    if(!ftmp||!(ftmp->IsOpen())||!(ftmp->GetNkeys())){
		cout<<"Can't open file or file has no key!!"<<endl;
	    }
	    else {
		cout<<"read in"<<ifile<<"th file: "<<filename<<endl;
		chain->Add(filename);
		ifile++;
	    }
	    delete ftmp;
	}
    }
    delete inputStream;

    //---------------------------------------------------------
    // loop events
    //---------------------------------------------------------	
    mPairDst *tree = new mPairDst(chain);

    int n = chain->GetEntries();
    cout<<n<<" events"<<endl;

    for(int i=0;i<n;i++){
	if(i%10000==0) cout<<"begin "<<i<<"th entry...."<<endl;
	tree->GetEntry(i);
	int nPairs = tree->nPairs;
	for(int it=0; it<nPairs; it++) {
	    int parentPid1  = tree->parentPid1[it];
	    float mcPairM   = tree->mcPairM[it];
	    float pairM     = tree->pairM[it];
	    float decayL    = tree->decayL[it];
	    float dcaV0     = tree->dcaV0[it];
	    float dcaPair   = tree->dcaPair[it];
	    float openAngle = tree->openAngle[it];
	    float dca1XY    = tree->dca1XY[it];
	    float dca1Z     = tree->dca1Z[it];
	    float dca1      = TMath::Sqrt(dca1XY*dca1XY+dca1Z*dca1Z);
	    float dca2XY    = tree->dca2XY[it];
	    float dca2Z     = tree->dca2Z[it];
	    float dca2      = TMath::Sqrt(dca2XY*dca2XY+dca2Z*dca2Z);
	    int   hftMap1   = tree->hftHitMap1[it];
	    int   hftMap2   = tree->hftHitMap2[it];

	    TVector3 dau1(tree->pP1x[it], tree->pP1y[it], tree->pP1z[it]);
	    TVector3 dau2(tree->pP2x[it], tree->pP2y[it], tree->pP2z[it]);

	    float pt1 = dau1.Pt();
	    float pt2 = dau2.Pt();

	    int   indexP    = parentPid1-12137+2;
	    if(parentPid1>=12037 && parentPid1<=12043) {
		indexP = 1;
	    }

	    hMassVsParentMc -> Fill(mcPairM , 0);
	    hMassVsParentMc -> Fill(mcPairM , indexP);

	    if(isHftM(hftMap1) && isHftM(hftMap2)) {
		hMassVsParentRc -> Fill(pairM    , 0);
		hDca1XYVsParent -> Fill(dca1XY   , 0);
		hDca1ZVsParent  -> Fill(dca1Z    , 0);
		hDca1VsParent   -> Fill(dca1     , 0);
		hDca2XYVsParent -> Fill(dca2XY   , 0);
		hDca2ZVsParent  -> Fill(dca2Z    , 0);
		hDca1VsParent   -> Fill(dca1     , 0);

		hMassVsParentRc  -> Fill(pairM   , indexP);
		hDecayLVsParent  -> Fill(decayL  , indexP);
		hDcaPairVsParent -> Fill(dcaPair , indexP);
		hDca1XYVsParent  -> Fill(dca1XY  , indexP);
		hDca1ZVsParent   -> Fill(dca1Z   , indexP);
		hDca1VsParent    -> Fill(dca1    , indexP);
		hDca2XYVsParent  -> Fill(dca2XY  , indexP);
		hDca2ZVsParent   -> Fill(dca2Z   , indexP);
		hDca2VsParent    -> Fill(dca1    , indexP);

		if(pt1<0.5) {
		    hDca1VsParentPt1->Fill(dca1, indexP);
		    hDca2VsParentPt1->Fill(dca2, indexP);
		}
		else if(pt1<1.) {
		    hDca1VsParentPt2->Fill(dca1, indexP);
		    hDca2VsParentPt2->Fill(dca2, indexP);
		}
		else {
		    hDca1VsParentPt3->Fill(dca1, indexP);
		    hDca2VsParentPt3->Fill(dca2, indexP);
		}
	    }
	}
    }
    //writeHistograms(outFile);
    //deleteHistograms();
    fout->Write();
    fout->Close();
    gDirectory->GetList()->Delete();
    delete chain;

    cout<<"end of program"<<endl;
    return 0;
}

void init(){
}

void bookHistograms(){
    hMassVsParentRc  = new TH2F("hMassVsParentRc"  , "" , 400 , 0     , 4    , 6 , 0 , 6);
    hMassVsParentMc  = new TH2F("hMassVsParentMc"  , "" , 400 , 0     , 4    , 6 , 0 , 6);
    hDecayLVsParent  = new TH2F("hDecayLVsParent"  , "" , 500 , -10   , 10   , 6 , 0 , 6);
    hDcaPairVsParent = new TH2F("hDcaPairVsParent" , "" , 500 , 0     , 1    , 6 , 0 , 6);
    hDca1XYVsParent  = new TH2F("hDca1XYVsParent"  , "" , 500 , -0.25 , 0.25 , 6 , 0 , 6);
    hDca2XYVsParent  = new TH2F("hDca2XYVsParent"  , "" , 500 , -0.25 , 0.25 , 6 , 0 , 6);
    hDca1ZVsParent   = new TH2F("hDca1ZVsParent"   , "" , 500 , -0.25 , 0.25 , 6 , 0 , 6);
    hDca2ZVsParent   = new TH2F("hDca2ZVsParent"   , "" , 500 , -0.25 , 0.25 , 6 , 0 , 6);
    hDca1VsParent    = new TH2F("hDca1VsParent"    , "" , 500 , 0     , 0.25 , 6 , 0 , 6);
    hDca2VsParent    = new TH2F("hDca2VsParent"    , "" , 500 , 0     , 0.25 , 6 , 0 , 6);

    hDca1VsParentPt1 = new TH2F("hDca1VsParentPt1" , "" , 500 , 0     , 0.25 , 6 , 0 , 6);
    hDca2VsParentPt1 = new TH2F("hDca2VsParentPt1" , "" , 500 , 0     , 0.25 , 6 , 0 , 6);
    hDca1VsParentPt2 = new TH2F("hDca1VsParentPt2" , "" , 500 , 0     , 0.25 , 6 , 0 , 6);
    hDca2VsParentPt2 = new TH2F("hDca2VsParentPt2" , "" , 500 , 0     , 0.25 , 6 , 0 , 6);
    hDca1VsParentPt3 = new TH2F("hDca1VsParentPt3" , "" , 500 , 0     , 0.25 , 6 , 0 , 6);
    hDca2VsParentPt3 = new TH2F("hDca2VsParentPt3" , "" , 500 , 0     , 0.25 , 6 , 0 , 6);
}


bool isHftM(int hitsMap) {
    return (hitsMap>>0 & 0x1) && (hitsMap>>1 & 0x3) && (hitsMap>>3 & 0x3);
}
