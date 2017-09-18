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
int  getIndex(int Pid);

int main(int argc, char** argv){
    if(argc!=1 && argc!=3 && argc!=2) {
	cout<<"Usage: analysis [inDir] [outfile] [option]"<<endl;
	return -1;
    }
    char *inFile = "test.list";
    char outFile[100];
    sprintf(outFile,"charm_simu.root");

    if(argc>=2){
	inFile = argv[1];
	//sprintf(outFile, "%s", argv[2]);
    }

    bool doTrueM = false;
    //if(argc == 4) {
    //    doTrueM = atoi(argv[3]);
    //} else if(argc == 2) {
    //    doTrueM = atoi(argv[1]);
    //}

    //if(doTrueM) sprintf(outFile, "charm_simu_TrueM.root");

    TString lazy;

    fout = new TFile(outFile,"recreate");
    fout->cd();
    //--------------------------------------------------
    // initializa
    //--------------------------------------------------
    init();
    bookHistograms();

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
    int charmC = 0;

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

	    float dcaP = TMath::Sqrt(dca1*dca1+dca2*dca2)*0.5;
	    float dphi = dau1.Phi()-dau2.Phi();
	    if(dphi<-0.5*PI) dphi+=2.*PI;
	    if(dphi>1.5*PI) dphi-=2.*PI;

	    float deta = eta1-eta2;

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
	    } else if(parentPid1>=12037 && parentPid1<=12044) {
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

	    int index1 = getIndex(parentPid1);
	    int index2 = getIndex(parentPid2);

	    if(index1<0 || index2<0) continue;
	    //if(index1 != index2 || index1<0 || index2<0) continue;

	    hDcaXYVsPt[index1]->Fill(dca1XY, pt1);
	    hDcaZVsPt[index1]->Fill(dca1Z, pt1);
	    hDcaVsPt[index1]->Fill(dca1, pt1);

	    hDcaXYVsPt[index2]->Fill(dca2XY, pt2);
	    hDcaZVsPt[index2]->Fill(dca2Z, pt2);
	    hDcaVsPt[index2]->Fill(dca2, pt2);

	    //if(index1 != index2) continue;
	    if(pt1>0.5 && pt2>0.5) {
		hDcaPVsMass0[index1]->Fill(dcaP, pairM);
		hDcaPVsMass1[index1]->Fill(dcaP, pairM);

		hDeltaPhiVsMass[index1]->Fill(dphi, pairM);
		hDeltaEtaVsMass[index1]->Fill(deta, pairM);

		if(parentPid1>12100) {
		    hMassVsParentRc -> Fill(pairM , indexP1-3);
		} else if(parentPid1>=12037 && parentPid1<=12044) {
		    hMassVsParentRc -> Fill(pairM , 1);
		} else if(parentPid1 == 0 && parentPid2 ==0) {
		    hMassVsParentRc -> Fill(pairM, 0);
		} else {
		    cout<<"Error : Wrong pair? parentPid1 "<<parentPid1<<" parentPid2 "<<parentPid2<<endl;
		}
	    }
	}
    }
    cout<<"number of pairs : "<<counter<<endl;
    cout<<"number of charm pairs: " << charmC<<endl;

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

int getIndex(int pid) {
    if(pid == 0) return 1;
    else if(pid >= 12037 && pid <12100)  return 0;
    else if(pid > 12100) return 2;
    else return -999;
}











