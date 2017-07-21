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
#define Debug 0

void init();
bool isHftM(int hitmap);
void bookHistograms();
void initCutHist(TString name);
int  getParentIndex(int index);

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
    //fout->mkdir("dcaTune");
    //fout->mkdir("dcaPairTune");
    //initCutHist("dcaTune");
    //initCutHist("dcaPairTune");
    //initCutHist("decayLTune");
    //initCutHist("dcaV0Tune");

    TFile * fNtp = new TFile("Pair_Ntuple.root", "recreate");
    TNtuple *pairT = new TNtuple("pairT", "pairT",
	    "M:dcaV0:dcaPair:decayL:pt1:fPid1:dca1:dcaXY1:dcaZ1:pt2:fPid2:dca2:dcaXY2:dcaZ2");

    float arr[13];


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
	    float eta1 = dau1.Eta();
	    float eta2 = dau2.Eta();

	    arr[0]  = pairM;
	    arr[1]  = dcaV0;
	    arr[2]  = dcaPair;
	    arr[3]  = decayL;
	    arr[4]  = pt1;
	    arr[5]  = parentPid1;
	    arr[6]  = dca1;
	    arr[7]  = dca1XY;
	    arr[8]  = dca1Z;
	    arr[9]  = pt2;
	    arr[10] = parentPid2;
	    arr[11] = dca2;
	    arr[12] = dca2XY;
	    arr[13] = dca2Z;

	    if((parentPid1 == 0 || parentPid1 > 12100) && parentPid1 != parentPid2) continue;

	    int indexP1 = getParentIndex(parentPid1);
	    int indexP2 = getParentIndex(parentPid2);
#if Debug
	    cout<<"Debug : indexP1 "<<setw(3)<<indexP1<<" gid "<<setw(5)<<parentPid1<<endl;
	    cout<<"Debug : indexP2 "<<setw(3)<<indexP2<<" gid "<<setw(5)<<parentPid2<<endl;
#endif
	    if(parentPid1>12100) {
		hMassVsParentMc -> Fill(mcPairM , indexP1-3);
	    } else if(parentPid1>=12037 && parentPid1<=12043) {
		hMassVsParentMc -> Fill(mcPairM , 1);
	    } else if(parentPid1 == 0 && parentPid2 ==0) {
		hMassVsParentMc -> Fill(mcPairM, 0);
	    } else {
		cout<<"Error : Wrong pair? parentPid1 "<<parentPid1<<" parentPid2 "<<parentPid2<<endl;
	    }

	    if(pairM<0) continue;
	    //if(parentPid1>12100 && parDisMc>1e-6) continue;

	    TLorentzVector pair(0,0,0,0);
	    pair.SetVectM(dau1+dau2, pairM);

	    if(pt1<0.2 && fabs(eta1)>1.) continue;
	    if(pt2<0.2 && fabs(eta2)>1.) continue;
	    if(fabs(pair.Rapidity())>1.) continue;
	    if(!isHftM(hftMap1) || !isHftM(hftMap2)) continue;

	    pairT->Fill(arr);

	    if(!doTrueM || (trueM1 && trueM2))
	    {
		hDcaVsPt[indexP1]->Fill(dca1, pt1);
		hDcaXYVsPt[indexP1]->Fill(dca1XY, pt1);
		hDcaZVsPt[indexP1]->Fill(dca1Z, pt1);

		hDcaVsPt[indexP2]->Fill(dca2, pt2);
		hDcaXYVsPt[indexP2]->Fill(dca2XY, pt2);
		hDcaZVsPt[indexP2]->Fill(dca2Z, pt2);

		if(pt1>0.5 && pt2>0.5) {
		    if(parentPid1>12100) {
			hDcaPairVsMass[indexP1-3]->Fill(dcaPair, pairM);
			hDecayLVsMass[indexP1-3]->Fill(decayL, pairM);
			hDcaV0VsMass[indexP1-3]->Fill(dcaV0, pairM);
			hOpenAngleVsMass[indexP1-3]->Fill(openAngle, pairM);
		    } else if(parentPid1>=12037 && parentPid1<=12043) {
			hDcaPairVsMass[1]->Fill(dcaPair, pairM);
			hDecayLVsMass[1]->Fill(decayL, pairM);
			hDcaV0VsMass[1]->Fill(dcaV0, pairM);
			hOpenAngleVsMass[1]->Fill(openAngle, pairM);
		    } else if(parentPid1 == 0 && parentPid2 == 0){
			hDcaPairVsMass[0]->Fill(dcaPair, pairM);
			hDecayLVsMass[0]->Fill(decayL, pairM);
			hDcaV0VsMass[0]->Fill(dcaV0, pairM);
			hOpenAngleVsMass[0]->Fill(openAngle, pairM);
		    }
		}
	    }

#if 0
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

    //writeHistograms(outFile);
    //deleteHistograms();
    fout->Write();
    fout->Close();

    fNtp->cd();
    pairT->Write();
    fNtp->Close();
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

    TString s;
    TString n;

    char* spec[9] = {"Kine_e", "D+", "D0", "Ds", "Lamba_c", "pi0", "eta", "omega", "phi"};

    for(int i = 0; i<9; i++) {
	s.Form("hDcaVsPt_%i", i);  n.Form("hDcaVsPt_%s", spec[i]);
	hDcaVsPt[i]   = new TH2F(s.Data() , n.Data() , 250 , 0     , 0.25 , 200 , 0 , 4);
	s.Form("hDcaXYVsPt_%i", i);  n.Form("hDcaXYVsPt_%s", spec[i]);
	hDcaXYVsPt[i] = new TH2F(s.Data() , n.Data() , 500 , -0.25 , 0.25 , 200 , 0 , 4);
	s.Form("hDcaZVsPt_%i", i);  n.Form("hDcaZVsPt_%s", spec[i]);
	hDcaZVsPt[i]  = new TH2F(s.Data() , n.Data() , 500 , -0.25 , 0.25 , 200 , 0 , 4);
    }
    fout->mkdir("pair");
    fout->cd("pair");
    hMassVsParentMc  = new TH2F("hMassVsParentMc"  , "" , 400 , 0     , 4    , 10 , 0 , 10);

    char* name[6] = {"Kine_e", "charm", "pi0", "eta", "omega", "phi"};
    for(int i=0; i<6; i++) {
	s.Form("hDcaPairVsMass_%i", i);  n.Form("hDcaPairVsMass_%s", name[i]);
	hDcaPairVsMass[i]   = new TH2F(s.Data() , n.Data() , 250 , 0   , 0.05        , 400 , 0 , 4);
	s.Form("hDecayLVsMass_%i", i);  n.Form("hDecayLVsMass_%s", name[i]);
	hDecayLVsMass[i]    = new TH2F(s.Data() , n.Data() , 500 , -10 , 10          , 400 , 0 , 4);
	s.Form("hDcaV0VsMass_%i", i);  n.Form("hDcaV0VsMass_%s", name[i]);
	hDcaV0VsMass[i]     = new TH2F(s.Data() , n.Data() , 250 , 0   , 0.25        , 400 , 0 , 4);
	s.Form("hOpenAngleVsMass_%i", i);  n.Form("hOpenAngleVsMass_%s", name[i]);
	hOpenAngleVsMass[i] = new TH2F(s.Data() , n.Data() , 180 , 0   , TMath::Pi() , 400 , 0 , 4);
    }
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

int getParentIndex(int pid) {
    if(pid == 0) return 0;
    else if(pid >= 12037 && pid <12100)  return (int)((pid-12037)*0.5)+1;
    else if(pid > 12100) return pid-12137+5;
    else return -999;
}















