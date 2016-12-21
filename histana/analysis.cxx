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
#include "TString.h"
//#include "StPhysicalHelixD.h"
#include "SystemOfUnits.h"
#include <stdio.h>
#include <stdlib.h>

using std::cout;
using std::endl;
using std::setw;
#endif 

void init();
bool isHftM(int hitmap);
void bookHistograms();
void initCutHist(TString name);

int main(int argc, char** argv){
    if(argc!=1 && argc!=3 && argc!=2) {
	cout<<"Usage: analysis [inDir] [outfile] [option]"<<endl;
	return -1;
    }
    char *inFile = "test.list";
    char outFile[100];
    sprintf(outFile,"charm_simu.root");

    if(argc>=3){
	inFile = argv[1];
	sprintf(outFile, "%s", argv[2]);
    }

    bool doTrueM = true;
    if(argc == 4) {
	doTrueM = atoi(argv[3]);
    } else if(argc == 2) {
	doTrueM = atoi(argv[1]);
    }

    if(doTrueM) sprintf(outFile, "charm_simu_TrueM.root");

    TString lazy;

    fout = new TFile(outFile,"recreate");
    fout->cd();
    //--------------------------------------------------
    // initializa
    //--------------------------------------------------
    init();
    bookHistograms();
    fout->mkdir("dcaTune");
    fout->mkdir("dcaPairTune");
    //initCutHist("dcaTune");
    //initCutHist("dcaPairTune");
    //initCutHist("decayLTune");
    //initCutHist("dcaV0Tune");
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
    int counter = 0;

    for(int i=0;i<n;i++){
	if(i%10000==0) cout<<"begin "<<i<<"th entry...."<<endl;
	tree->GetEntry(i);
	int nPairs = tree->nPairs;
	//cout<<nPairs<<endl;
	for(int it=0; it<nPairs; it++) {
	    counter++;
	    int parentPid1  = tree->parentPid1[it];
	    int parentPid2  = tree->parentPid2[it];
	    float decayLMc  = tree->decayLMc[it];
	    float parDisMc  = tree->parDisMc[it];
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

	    int  idtrue1_pxl1 = (Int_t)tree->idtrue1_pxl1[it];
	    int  idtrue1_pxl2 = (Int_t)tree->idtrue1_pxl2[it];
	    int  idtrue1_ist  = (Int_t)tree->idtrue1_ist[it];

	    int  idtrue2_pxl1 = tree->idtrue2_pxl1[it];
	    int  idtrue2_pxl2 = tree->idtrue2_pxl2[it];
	    int  idtrue2_ist  = tree->idtrue2_ist[it];

	    bool trueM1 = idtrue1_pxl1>0 && idtrue1_pxl2>0 && idtrue1_ist>0;
	    bool trueM2 = idtrue2_pxl1>0 && idtrue2_pxl2>0 && idtrue2_ist>0;

	    TVector3 dau1(tree->pP1x[it], tree->pP1y[it], tree->pP1z[it]);
	    TVector3 dau2(tree->pP2x[it], tree->pP2y[it], tree->pP2z[it]);

	    float pt1 = dau1.Pt();
	    float pt2 = dau2.Pt();

	    //cout<<idtrue1_pxl1<<" "<<idtrue2_pxl1<<endl;

	    int   indexP    = parentPid1-12137+2;
	    if(parentPid1>=12037 && parentPid1<=12043) {
		indexP = 1;
	    } 
	    else if(parentPid1 != parentPid2) {
		cout<<"from two parents???"<<endl;
		continue;
	    }

	    hMassVsParentMc -> Fill(mcPairM , 0);
	    hMassVsParentMc -> Fill(mcPairM , indexP);

	    if(pairM<0) continue;
	    //if(parentPid1>12100 && parDisMc>1e-6) continue;

	    TLorentzVector pair(0,0,0,0);
	    pair.SetVectM(dau1+dau2, pairM);

	    if(fabs(pair.Rapidity())>1.) continue;
	    if(!isHftM(hftMap1) || !isHftM(hftMap2)) continue;

	    //cout<<isHftM(hftMap1)<<" "<<" "<<trueM1<<" "<<idtrue1_pxl1<<" "<<idtrue1_pxl2<<" "<<idtrue1_ist<<endl;
	    //cout<<isHftM(hftMap2)<<" "<<" "<<trueM2<<" "<<idtrue2_pxl1<<" "<<idtrue2_pxl2<<" "<<idtrue2_ist<<endl;

	    if(!doTrueM || trueM1 && trueM2)
	    {
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
		}
		else if(pt1<1.) {
		    hDca1VsParentPt2->Fill(dca1, indexP);
		}
		else {
		    hDca1VsParentPt3->Fill(dca1, indexP);
		}

		if(pt2<0.5) {
		    hDca2VsParentPt1->Fill(dca2, indexP);
		}
		else if(pt2<1.) {
		    hDca2VsParentPt2->Fill(dca2, indexP);
		}
		else {
		    hDca2VsParentPt3->Fill(dca2, indexP);
		}

		if(parentPid1<12100) {
		    hDcaXYVsPtCharm->Fill(dca1XY, pt1);
		    hDcaXYVsPtCharm->Fill(dca2XY, pt2);
		    hDcaZVsPtCharm->Fill(dca1Z, pt1);
		    hDcaZVsPtCharm->Fill(dca2Z, pt2);
		    hDcaVsPtCharm->Fill(dca1, pt1);
		    hDcaVsPtCharm->Fill(dca2, pt2);

		    if(pt1>0.5 && pt2>0.5) {
			hDcaPairVsMassCharm->Fill(dcaPair, pairM);
			hDcaV0VsMassCharm->Fill(dcaV0, pairM);
			hDecayLVsMassCharm->Fill(decayL, pairM);
			hOpenAngleVsMassCharm->Fill(openAngle, pairM);
		    }
		}
		if(parentPid1>12100) {
		    hDcaXYVsPtMeson->Fill(dca1XY, pt1);
		    hDcaXYVsPtMeson->Fill(dca2XY, pt2);
		    hDcaZVsPtMeson->Fill(dca1Z, pt1);
		    hDcaZVsPtMeson->Fill(dca2Z, pt2);
		    hDcaVsPtMeson->Fill(dca1, pt1);
		    hDcaVsPtMeson->Fill(dca2, pt2);

		    if(pt1>0.5 && pt2>0.5) {
			hDcaPairVsMassMeson->Fill(dcaPair, pairM);
			hDcaV0VsMassMeson->Fill(dcaV0, pairM);
			hDecayLVsMassMeson->Fill(decayL, pairM);
			hOpenAngleVsMassMeson->Fill(openAngle, pairM);
		    }
		}
	    }

#if 1

	    if(pt1<0.5 && pt2<0.5) continue;
	    fout->cd("dcaTune");
	    htmC = NULL;
	    htmM = NULL;
	    for(int i=0;i<10;i++) {
		lazy.Form("dcaTune/hMassC_%i", i);
		fout->GetObject(lazy.Data(), htmC);
		if(!htmC) {
		    lazy.Form("hMassC_%i", i);
		    htmC = new TH1F(lazy.Data(), "", 400, 0, 4);
		}

		lazy.Form("dcaTune/hMassM_%i",i);
		fout->GetObject(lazy.Data(), htmM);
		if(!htmM) {
		    lazy.Form("hMassM_%i", i);
		    htmM = new TH1F(lazy.Data(), "", 400, 0, 4);
		}

		if(dca1<dcaCuts[i] && dca2<dcaCuts[i]) {
		    if(parentPid1<12100) htmC->Fill(pairM);
		    if(parentPid1>12100) htmM->Fill(pairM);
		}
	    }

	    fout->cd("dcaPairTune");
	    htmC = NULL;
	    htmM = NULL;
	    for(int i=0;i<10;i++) {
		//lazy.Form("dcaPairTune/hMassC_%i",i);
		//TH1F *htmC; fout->GetObject(lazy.Data(), htmC);
		//lazy.Form("dcaPairTune/hMassM_%i",i);
		//TH1F *htmM; fout->GetObject(lazy.Data(), htmM);

		lazy.Form("dcaPairTune/hMassC_%i", i);
		fout->GetObject(lazy.Data(), htmC);
		if(!htmC) {
		    lazy.Form("hMassC_%i", i);
		    htmC = new TH1F(lazy.Data(), "", 400, 0, 4);
		}
		lazy.Form("dcaPairTune/hMassM_%i",i);
		fout->GetObject(lazy.Data(), htmM);
		if(!htmM) {
		    lazy.Form("hMassM_%i", i);
		    htmM = new TH1F(lazy.Data(), "", 400, 0, 4);
		}

		if(dca1>0.015 && dca2>0.015) continue;

		if(dcaPair<dcaPairCuts[i]) {
		    if(parentPid1<12100) htmC->Fill(pairM);
		    if(parentPid1>12100) htmM->Fill(pairM);
		}
	    }
#endif
	}
    }
    cout<<"number of pairs : "<<counter<<endl;

#if 0
    float yC[10], eyC[10];
    float yM[10], eyM[10];
    TH1F *hMassC = (TH1F *)hMassVsParentRc->ProjectionX("hMassC",2,2);
    TH1F *hMassM = (TH1F *)hMassVsParentRc->ProjectionX("hMassM",3,-1);
    {
	fout->cd("dcaTune");
	TGraphErrors *gRC; fout->GetObject("dcaTune/gRatioCharm", gRC); 
	TGraphErrors *gRM; fout->GetObject("dcaTune/gRatioMeson", gRM); 
	for(int i=0; i<6; i++) {
	    lazy.Form("dcaTune/hMassC_%i",i);
	    TH1F *htmC; fout->GetObject(lazy.Data(), htmC);
	    lazy.Form("dcaTune/hMassM_%i",i);
	    TH1F *htmM; fout->GetObject(lazy.Data(), htmM);

	    int bin1 = htmC->GetXaxis()->FindBin(0.3+1e-8);
	    int bin2 = htmC->GetXaxis()->FindBin(0.3-1e-8);

	    double c1 = 0;
	    double ec1 = 0;
	    double c2 = 0;
	    double ec2 = 0;
	    c1 = htmC->IntegralAndError(bin1,bin2,ec1);
	    c2 = hMassC->IntegralAndError(bin1,bin2,ec2);
	    yC[i] = c2>1e-19? c1/c2 : 0;

	    c1 = htmM->IntegralAndError(bin1,bin2,ec1);
	    c2 = hMassM->IntegralAndError(bin1,bin2,ec2);
	    yM[i] = c2>1e-19? c1/c2 : 0;

	    gRC->SetPoint(i, dcaCuts[i], yC[i]);
	    gRM->SetPoint(i, dcaCuts[i], yM[i]);
	}
    }
    {
	fout->cd("dcaPairTune");
	TGraphErrors *gRC; fout->GetObject("dcaPairTune/gRatioCharm", gRC); 
	TGraphErrors *gRM; fout->GetObject("dcaPairTune/gRatioMeson", gRM); 
	for(int i=0; i<6; i++) {
	    lazy.Form("dcaPairTune/hMassC_%i",i);
	    TH1F *htmC; fout->GetObject(lazy.Data(), htmC);
	    lazy.Form("dcaPairTune/hMassM_%i",i);
	    TH1F *htmM; fout->GetObject(lazy.Data(), htmM);

	    int bin1 = htmC->GetXaxis()->FindBin(0.3+1e-8);
	    int bin2 = htmC->GetXaxis()->FindBin(0.3-1e-8);

	    double c1 = 0;
	    double ec1 = 0;
	    double c2 = 0;
	    double ec2 = 0;
	    c1 = htmC->IntegralAndError(bin1,bin2,ec1);
	    c2 = hMassC->IntegralAndError(bin1,bin2,ec2);
	    yC[i] = c2>1e-19? c1/c2 : 0;

	    c1 = htmM->IntegralAndError(bin1,bin2,ec1);
	    c2 = hMassM->IntegralAndError(bin1,bin2,ec2);
	    yM[i] = c2>1e-19? c1/c2 : 0;

	    gRC->SetPoint(i, dcaPairCuts[i], yC[i]);
	    gRM->SetPoint(i, dcaPairCuts[i], yM[i]);
	}
    }
#endif

    //writeHistograms(outFile);
    //deleteHistograms();
    fout->Write();
    fout->Close();
    //gDirectory->GetList()->Delete();
    delete chain;

    cout<<"end of program"<<endl;
    return 0;
}

void init(){
}

void bookHistograms(){
    fout->mkdir("single");
    fout->cd("single");
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

    hDcaVsPtCharm   = new TH2F("hDcaVsPtCharm"   , "" , 250 , 0     , 0.25 , 200 , 0 , 4);
    hDcaVsPtMeson   = new TH2F("hDcaVsPtMeson"   , "" , 250 , 0     , 0.25 , 200 , 0 , 4);
    hDcaXYVsPtCharm = new TH2F("hDcaXYVsPtCharm" , "" , 500 , -0.25 , 0.25 , 200 , 0 , 4);
    hDcaXYVsPtMeson = new TH2F("hDcaXYVsPtMeson" , "" , 500 , -0.25 , 0.25 , 200 , 0 , 4);
    hDcaZVsPtCharm  = new TH2F("hDcaZVsPtCharm"  , "" , 500 , -0.25 , 0.25 , 200 , 0 , 4);
    hDcaZVsPtMeson  = new TH2F("hDcaZVsPtMeson"  , "" , 500 , -0.25 , 0.25 , 200 , 0 , 4);

    fout->mkdir("pair");
    fout->cd("pair");
    hMassVsParentRc  = new TH2F("hMassVsParentRc"  , "" , 400 , 0     , 4    , 6 , 0 , 6);
    hMassVsParentMc  = new TH2F("hMassVsParentMc"  , "" , 400 , 0     , 4    , 6 , 0 , 6);
    hDecayLVsParent  = new TH2F("hDecayLVsParent"  , "" , 500 , -10   , 10   , 6 , 0 , 6);
    hDcaPairVsParent = new TH2F("hDcaPairVsParent" , "" , 500 , 0     , 1    , 6 , 0 , 6);

    hDcaPairVsMassCharm = new TH2F("hDcaPairVsMassCharm" , "" , 250 , 0   , 0.05 , 400 , 0 , 4);
    hDcaPairVsMassMeson = new TH2F("hDcaPairVsMassMeson" , "" , 250 , 0   , 0.05 , 400 , 0 , 4);
    hDecayLVsMassCharm  = new TH2F("hDecayLVsMassCharm"  , "" , 500 , -10 , 10   , 400 , 0 , 4);
    hDecayLVsMassMeson  = new TH2F("hDecayLVsMassMeson"  , "" , 500 , -10 , 10   , 400 , 0 , 4);
    hDcaV0VsMassCharm   = new TH2F("hDcaV0VsMassCharm"   , "" , 250 , 0   , 0.25 , 400 , 0 , 4);
    hDcaV0VsMassMeson   = new TH2F("hDcaV0VsMassMeson"   , "" , 250 , 0   , 0.25 , 400 , 0 , 4);

    hOpenAngleVsMassCharm   = new TH2F("hOpenAngleVsMassCharm" , "" , 180 , 0 , TMath::Pi() , 400 , 0 , 4);
    hOpenAngleVsMassMeson   = new TH2F("hOpenAngleVsMassMeson" , "" , 180 , 0 , TMath::Pi() , 400 , 0 , 4);
}

void initCutHist(TString name) {
    fout->mkdir(name.Data());
    fout->cd(name.Data());
    TH1F *hMassC[10];
    TH1F *hMassM[10];

    TString fname;
    for(int i=0; i<10; i++) {
	fname.Form("hMassC_%i", i);
	hMassC[i] = new TH1F(fname.Data(), "", 200, 0, 2);
	fname.Form("hMassM_%i", i);
	hMassM[i] = new TH1F(fname.Data(), "", 200, 0, 2);
    }

    //TGraphErrors *gRatioCharm = new TGraphErrors(10);
    //gRatioCharm->SetName("gRatioCharm");
    //TGraphErrors *gRatioMeson = new TGraphErrors(10);
    //gRatioMeson->SetName("gRatioMeson");
}


bool isHftM(int hitsMap) {
    return (hitsMap>>0 & 0x1) && (hitsMap>>1 & 0x3) && (hitsMap>>3 & 0x3);
}

void assign(std::vector<float> &vec,float len,float *what)
{
    vec.erase(vec.begin(),vec.end());
    for(int i=0; i<len; ++i) {
	vec.push_back(what[i]);
    }
}
