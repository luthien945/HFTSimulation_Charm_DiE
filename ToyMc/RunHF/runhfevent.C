#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TChain.h"
#include "TSystem.h"
#include "TMath.h"
#include "TTree.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "TObjArray.h"
#include "TBenchmark.h"
#include "TRandom.h"
#include "TPythia6.h"
#include "TMCParticle.h"
#include <iostream>
#include <vector>
#include <string>
#include "histogram.h"
//#include "TPythia6/TPythia6.h"
//#include "TPythia6/TMCParticle6.h"
using std::cout;
using std::endl;

#define eMass 0.000511
#define charmMass 1.5

void runhfevent(int irun=1, int Nevt=10, int iseed=789456, float energy = 62.4, char *folder="out_test"){
    TStopwatch*   stopWatch = new TStopwatch();
    stopWatch->Start();
    gSystem->Load("libPhysics");
    //gSystem->Load("libEGi.so");
    gSystem->Load("libEGPythia6.so");
    gSystem->Load("libPythia6.so");
    //gSystem->Load("/home/yfzhang/work/MC/pythia6/libPythia6.so");
    //    gSystem->Load("TPythia6/TPythia6.so");

    TH1D *hnEvents = new TH1D("hnEvents","hnEvents",10,0,10);
    TH2D *heeMvsPt_AllAcc = new TH2D("heeMvsPt_AllAcc","cc2ee mass vs pt;p_{T};m_{ee} GeV/c^{2}",1000,0.,10.,1000,0.,10.);
    TH2D *heeMvsPt_TrgAcc = new TH2D("heeMvsPt_TrgAcc","cc2ee mass vs pt;p_{T};m_{ee} GeV/c^{2}",1000,0.,10.,1000,0.,10.);
    TH2D *heeMvsPt_TrgAcc0 = new TH2D("heeMvsPt_TrgAcc0","cc2ee mass vs pt;p_{T};m_{ee} GeV/c^{2}",1000,0.,10.,1000,0.,10.);
    TH1D *heedPhi_Acc0 = new TH1D("heedPhi_Acc0","d#phi GeV/c^{2}",1000,-6.3,6.3);  
    TH1D *heedPhi_Acc = new TH1D("heedPhi_Acc","d#phi GeV/c^{2}",1000,-6.3,6.3);
    TH1D *heedPhi_All = new TH1D("heedPhi_All","d#phi GeV/c^{2}",1000,-6.3,6.3);
    //========================= initial trees ==========================
    eTree = new TTree("meTree","eTree");
    eTree->SetAutoSave(1000000);

    cout << "Initialize the eTree ... " << endl;

    eTree->Branch("EventId"       , &meTree.EventId       , "EventId/I"       );
    eTree->Branch("cPt"           , &meTree.cPt           , "cPt/F"           );
    eTree->Branch("cEta"          , &meTree.cEta          , "cEta/F"          );
    eTree->Branch("cPhi"          , &meTree.cPhi          , "cPhi/F"          );
    eTree->Branch("cY"            , &meTree.cY            , "cY/F"            );

    eTree->Branch("cbarPt"        , &meTree.cbarPt        , "cbarPt/F"        );
    eTree->Branch("cbarEta"       , &meTree.cbarEta       , "cbarEta/F"       );
    eTree->Branch("cbarPhi"       , &meTree.cbarPhi       , "cbarPhi/F"       );
    eTree->Branch("cbarY"         , &meTree.cbarY         , "cbarY/F"         );

    eTree->Branch("eePt"          , &meTree.eePt          , "eePt/F"          );
    eTree->Branch("eeEta"         , &meTree.eeEta         , "eeEta/F"         );
    eTree->Branch("eePhi"         , &meTree.eePhi         , "eePhi/F"         );
    eTree->Branch("eeRapidity"    , &meTree.eeRapidity    , "eeRapidity/F"    );
    eTree->Branch("eeM"           , &meTree.eeM           , "eeM/F"           );
    eTree->Branch("ePosParentGID" , &meTree.ePosParentGID , "ePosParentGID/I" );
    eTree->Branch("ePosPt"        , &meTree.ePosPt        , "ePosPt/F"        );
    eTree->Branch("ePosEta"       , &meTree.ePosEta       , "ePosEta/F"       );
    eTree->Branch("ePosPhi"       , &meTree.ePosPhi       , "ePosPhi/F"       );
    eTree->Branch("ePosX"         , &meTree.ePosX         , "ePosX/F"         );
    eTree->Branch("ePosY"         , &meTree.ePosY         , "ePosY/F"         );
    eTree->Branch("ePosZ"         , &meTree.ePosZ         , "ePosZ/F"         );
    eTree->Branch("eNegParentGID" , &meTree.eNegParentGID , "eNegParentGID/I" );
    eTree->Branch("eNegPt"        , &meTree.eNegPt        , "eNegPt/F"        );
    eTree->Branch("eNegEta"       , &meTree.eNegEta       , "eNegEta/F"       );
    eTree->Branch("eNegPhi"       , &meTree.eNegPhi       , "eNegPhi/F"       );
    eTree->Branch("eNegX"         , &meTree.eNegX         , "eNegX/F"         );
    eTree->Branch("eNegY"         , &meTree.eNegY         , "eNegY/F"         );
    eTree->Branch("eNegZ"         , &meTree.eNegZ         , "eNegZ/F"         );

    //eTree->Branch("ePaPosPt",&meTree.ePaPosPt,"ePaPosPt/F");
    //eTree->Branch("ePaPosEta",&meTree.ePaPosEta,"ePaPosEta/F");
    //eTree->Branch("ePaPosPhi",&meTree.ePaPosPhi,"ePaPosPhi/F");
    //eTree->Branch("ePaPosY",&meTree.ePaPosY,"ePaPosY/F");
    //eTree->Branch("ePaNegPt",&meTree.ePaNegPt,"ePaNegPt/F");
    //eTree->Branch("ePaNegEta",&meTree.ePaNegEta,"ePaNegEta/F");
    //eTree->Branch("ePaNegPhi",&meTree.ePaNegPhi,"ePaNegPhi/F");
    //eTree->Branch("ePaNegY",&meTree.ePaNegY,"ePaNegY/F");
    //======================== initial parameters ==========================

    // Initialize Pythia
    TPythia6 *myPythia6 = new TPythia6();
    myPythia6->SetMSEL(1);// pp mb 
    myPythia6->SetPARP(91,1.0);  //Kt
    myPythia6->SetPARP(67,1.0);    //kfactor 

    //myPythia6->SetMSEL(1); //call PYGIVE('msel = 1')   ! pp min. bias. 
    //myPythia6->SetMSEL(4);//c trigger
    //    myPythia6->SetMSEL(5);//b trigger
    //myPythia6->SetMSTP(81,1);//call PYGIVE('mstp(81) = 1') ! multiple interaction on.
    //myPythia6->SetMSTP(82,4);//call PYGIVE('mstp(82) = 4') ! impact parameter choice.
    //myPythia6->SetMSTP(2, 2);//call PYGIVE('mstp(2) = 2') ! 2nd order running of alpha_s.
    //myPythia6->SetMSTP(33,3);//call PYGIVE('mstp(33) = 3') ! K-factors.

    //double guass distribution
    //PARP(83)=0.2
    //PARP(84)=0.5
    //myPythia6->SetPARP(83,0.2);
    //myPythia6->SetPARP(84,0.5);

    //myPythia6->SetPARP(82,3.2);//call PYGIVE('parp(82) = 3.2') ! PT0 multiple distribution tail.
    //myPythia6->SetCKIN(3,5);//Set the high pT trigger 4GeV/c    

    //tuned parameters:
    //myPythia6->SetMSTJ(11,3);//Peterson FF
    //myPythia6->SetPARJ(54,-5.e-03);//mstj11 epsilon, make spectrum hard D=-0.05, 0.005 is between them
    //myPythia6->SetPARJ(55,-5.e-03);
    //myPythia6->SetPARJ(54,-1e-05);//mstj11 epsilon, make spectrum hard D=-0.05, -1e-5 more hard, -0.005 is between them
    //myPythia6->SetPARJ(55,-1e-05);
    //myPythia6->SetMSTP(33,1);//common kfactor stored in parp31
    //myPythia6->SetMSTP(32,4);//Q^2 scale D:8
    //myPythia6->SetMSTP(51,7);//CTEQ5L PDF
    //myPythia6->SetPARP(31,3.5); //D:1.5
    //myPythia6->SetPARP(91,1.);//<kt>
    //    myPythia6->SetPARP(93,10);//maximum pt broadening D:5
    //myPythia6->SetPMAS(4,1,1.25);//c-quark mass D=1.5
    //    myPythia6->SetPMAS(5,1,4.8);//b-quark mass  D=4.8
    //    myPythia6->SetPARP(67,1);//mstp32*4 high pT tuned parameter

    //CDF tuned parameters:
    //myPythia6->SetMSTP(51,7);
    //myPythia6->SetMSTP(82,4);
    //myPythia6->SetPARP(82,2.0);
    //myPythia6->SetPARP(83,0.5);
    //myPythia6->SetPARP(84,0.4);
    //myPythia6->SetPARP(85,0.9);
    //myPythia6->SetPARP(86,0.95);
    //myPythia6->SetPARP(89,1800);
    //myPythia6->SetPARP(90,0.25);
    //myPythia6->SetPARP(91,1.0);
    //myPythia6->SetPARP(67,4.0);

    //Xin's tune:
    //myPythia6->SetMSTP(51,7);
    //myPythia6->SetMSTP(82,4);
    //myPythia6->SetPARP(82,2.0);
    //myPythia6->SetPARP(83,0.5);
    //myPythia6->SetPARP(84,0.4);
    //myPythia6->SetPARP(85,0.9);
    //myPythia6->SetPARP(86,0.95);
    //myPythia6->SetPARP(89,1800);
    //myPythia6->SetPARP(90,0.25);  

    //myPythia6->SetPARP(91,1.);//<kt>
    //myPythia6->SetPARP(67,1);//mstp32*4 high pT tuned parameter

    //====================== particle decay mode ==========================

    //switch off non-electron decays
    for(int i=684; i<=735; i++) myPythia6->SetMDME(i,1,0); //D+
    for(int i=755; i<=807; i++) myPythia6->SetMDME(i,1,0); //D0
    myPythia6->SetMDME(818,1,0); //Ds
    for(int i=824; i<=850; i++) myPythia6->SetMDME(i,1,0); //Ds
    for(int i=857; i<=862; i++) myPythia6->SetMDME(i,1,0); //eta_c,J/psi,chi_2c
    for(int i=1097; i<=1165; i++) myPythia6->SetMDME(i,1,0); //Lc

    //============================ initial run =============================

    myPythia6->SetMRPY(1,iseed);
    myPythia6->Initialize("CMS","p","p",energy);

    myPythia6->Pystat(2);

    //============================ run events ==============================

    int nevent = 0;
    for(int i = 1;i<=Nevt;i++){
	myPythia6->GenerateEvent();

	//myPythia6->Pylist(1);
	//if(i<10) myPythia6->Pylist(1);

	TObjArray *particles = myPythia6->GetListOfParticles();
	int nParticles = particles->GetEntries();

	if(i%100000==0) {
	    cout<<"on event #"<<i<<endl;
	    cout << "nParticles = " << nParticles << endl;
	}

	int nstring = 0;
	hnEvents->Fill(0);

	bool charm = kFALSE;
	for(int l=0;l<nParticles;l++){

	    TMCParticle *mParticle = (TMCParticle*)particles->At(l);
	    if(!mParticle) continue;
	    int kf= abs( mParticle->GetKF() );
	    if( kf == 4 ) charm = kTRUE;

	}
	if(!charm) continue;
	hnEvents->Fill(1);

	for(int l=0;l<nParticles;l++){
	    TMCParticle *mParticle = (TMCParticle*)particles->At(l);
	    if(!mParticle) continue;

	    int stringpid = mParticle->GetKF();

	    if(stringpid != 92) continue; //string

	    int string_parentID = mParticle->GetParent()-1;
	    TMCParticle *mStringParent = (TMCParticle*)particles->At(string_parentID);
	    if(!mStringParent) continue;

	    int charmpid = mStringParent->GetKF();
	    if(abs(charmpid) != 4) continue; //string comes from c-cbar

	    /*
	       int string_daughterID = mParticle->GetFirstChild()-1;
	       TMCParticle *mStringDaughter = (TMCParticle*)particles->At(string_daughterID);
	       if(!mStringDaughter) continue;

	       int daughterpid = mStringDaughter->GetKF();

	       cout << "daughter pID:  " << daughterpid << endl;
	     */
	    nstring ++;

	}

	if(nstring != 2) continue;
	hnEvents->Fill(2);

	int   ne         = 0;
	int   ePos_ppid  = 0;
	float ePos_Pt    = -999.;
	float ePos_Eta   = -999.;
	float ePos_Phi   = -999.;
	float ePos_x     = -999.;
	float ePos_y     = -999.;
	float ePos_z     = -999.;

	int   eNeg_ppid  = 0;
	float eNeg_Pt    = -999.;
	float eNeg_Eta   = -999.;
	float eNeg_Phi   = -999.;
	float eNeg_x     = -999.;
	float eNeg_y     = -999.;
	float eNeg_z     = -999.;


	float ePaPos_Pt  = -999.;
	float ePaPos_Eta = -999.;
	float ePaPos_Phi = -999.;
	float ePaPos_Y   = -999.;
	float ePaNeg_Pt  = -999.;
	float ePaNeg_Eta = -999.;
	float ePaNeg_Phi = -999.;
	float ePaNeg_Y   = -999.;

	float cPt        = -999.;
	float cEta       = -999.;
	float cPhi       = -999.;
	float cY         = -999.;

	float cbarPt     = -999.;
	float cbarEta    = -999.;
	float cbarPhi    = -999.;
	float cbarY      = -999.;

	for(int l=0;l<nParticles;l++){
	    TMCParticle *mElectron = (TMCParticle*)particles->At(l);

	    if(!mElectron) continue;


	    int cid  = mElectron->GetKF();
	    int c_ks = mElectron->GetKS();

	    if(cid==4&&c_ks==12){
		float cpx = mElectron->GetPx();
		float cpy = mElectron->GetPy();
		float cpz = mElectron->GetPz();
		float cmass = mElectron->GetMass();
		TLorentzVector cMom(0,0,0,0);
		//cout<<"cmass:"<<cmass<<endl;
		cMom.SetXYZM(cpx,cpy,cpz,cmass);
		cPt  = cMom.Pt();
		cEta = cMom.PseudoRapidity();
		cPhi = cMom.Phi();
		cY   = cMom.Rapidity();
	    }
	    if(cid==-4&&c_ks==12){
		float cpx = mElectron->GetPx();
		float cpy = mElectron->GetPy();
		float cpz = mElectron->GetPz();
		float cmass = mElectron->GetMass();
		TLorentzVector cbarMom(0,0,0,0);
		cbarMom.SetXYZM(cpx,cpy,cpz,cmass);
		cbarPt  = cbarMom.Pt();
		cbarEta = cbarMom.PseudoRapidity();
		cbarPhi = cbarMom.Phi();
		cbarY   = cbarMom.Rapidity();
	    }


	    int epid = mElectron->GetKF();

	    if(abs(epid) != 11) continue; //e+: -11,  e-: 11
	    hnEvents->Fill(3);

	    int e_parentID = mElectron->GetParent()-1;
	    TMCParticle *meParent = (TMCParticle*)particles->At(e_parentID);
	    if(!meParent) continue;

	    int e_parentpid = abs(meParent->GetKF());
	    if(e_parentpid != 411 &&
		    e_parentpid != 421 &&
		    e_parentpid != 431 &&
		    e_parentpid != 4122)  continue;

	    hnEvents->Fill(4);

	    float px = mElectron->GetPx();
	    float py = mElectron->GetPy();
	    float pz = mElectron->GetPz();

	    float x = mElectron->GetVx();
	    float y = mElectron->GetVy();
	    float z = mElectron->GetVz();

	    //cout<<x<<" "<<y<<" "<<z<<endl;

	    TVector3 Mom(px,py,pz);
	    float pt  = Mom.Perp();
	    float eta = Mom.PseudoRapidity();
	    float phi = Mom.Phi();


	    float papx = meParent->GetPx();
	    float papy = meParent->GetPy();
	    float papz = meParent->GetPz();

	    TLorentzVector paMom(0,0,0,0);
	    paMom.SetXYZM(papx,papy,papz,charmMass);
	    float papt  = paMom.Perp();
	    float paeta = paMom.PseudoRapidity();
	    float paphi = paMom.Phi();
	    float pay = paMom.Rapidity();

	    //if(fabs(eta)>1.) continue;

	    if(epid == -11) {
		ePos_ppid = e_parentpid;
		ePos_Pt   = pt;
		ePos_Eta  = eta;
		ePos_Phi  = phi;

		ePos_x    = x;
		ePos_y    = y;
		ePos_z    = z;

		ePaPos_Pt = papt;
		ePaPos_Eta = paeta;
		ePaPos_Phi = paphi;
		ePaPos_Y   = pay;
	    }

	    if(epid == 11) {
		eNeg_ppid = e_parentpid;
		eNeg_Pt   = pt;
		eNeg_Eta  = eta;
		eNeg_Phi  = phi;

		eNeg_x    = x;
		eNeg_y    = y;
		eNeg_z    = z;

		ePaNeg_Pt = papt;
		ePaNeg_Eta = paeta;
		ePaNeg_Phi = paphi;
		ePaNeg_Y   = pay;
	    }

	    ne ++;

	}//particles

	if(ne<1) continue;
	//if(ne != 2) continue;
	hnEvents->Fill(5);

	//      cout << "number of strings:  " << nstring << "  number of electrons:  " << ne << endl;

	TLorentzVector ePosMom(0,0,0,0);
	TLorentzVector eNegMom(0,0,0,0);
	TLorentzVector eeMom(0,0,0,0);

	ePosMom.SetPtEtaPhiM(ePos_Pt,ePos_Eta,ePos_Phi,eMass);
	eNegMom.SetPtEtaPhiM(eNeg_Pt,eNeg_Eta,eNeg_Phi,eMass);

	eeMom = ePosMom + eNegMom;
	double eeRapidity = eeMom.Rapidity();
	double eePt = eeMom.Pt();
	double eeM  = eeMom.M();

	if(fabs(eeRapidity)<1.&&fabs(ePos_Eta)< 1.&& fabs(eNeg_Eta)<1.&&ePos_Pt>0.15&&ePos_Pt<0.35&&eNeg_Pt>3.5){
	    heeMvsPt_AllAcc->Fill(eePt,eeM);
	    heedPhi_All->Fill(ePos_Phi-eNeg_Phi);
	}
	if(fabs(eeRapidity)<1.&&fabs(ePos_Eta)< 0.5&& fabs(eNeg_Eta)<0.8&&ePos_Pt>1.8&&eNeg_Pt>2.){
	    heeMvsPt_TrgAcc->Fill(eePt,eeM);
	    heedPhi_Acc->Fill(ePos_Phi-eNeg_Phi);
	}
	if(fabs(ePos_Eta)< 0.5&& fabs(eNeg_Eta)<0.8&&ePos_Pt>1.8&&eNeg_Pt>2.){
	    heeMvsPt_TrgAcc0->Fill(eePt,eeM);
	    heedPhi_Acc0->Fill(ePos_Phi-eNeg_Phi);
	}

	meTree.EventId       = nevent;
	meTree.cPt           = cPt;
	meTree.cEta          = cEta;
	meTree.cPhi          = cPhi;
	meTree.cY            = cY;
	meTree.cbarPt        = cbarPt;
	meTree.cbarEta       = cbarEta;
	meTree.cbarPhi       = cbarPhi;
	meTree.cbarY         = cbarY;

	meTree.eePt          = eeMom.Pt();
	meTree.eeEta         = eeMom.Eta();
	meTree.eePhi         = eeMom.Phi();
	meTree.eeRapidity    = eeMom.Rapidity();
	meTree.eeM           = eeMom.M();
	meTree.ePosParentGID = ePos_ppid;
	meTree.ePosPt        = ePos_Pt;
	meTree.ePosEta       = ePos_Eta;
	meTree.ePosPhi       = ePos_Phi;
	meTree.ePosX         = ePos_x;
	meTree.ePosY         = ePos_y;
	meTree.ePosZ         = ePos_z;
	meTree.eNegParentGID = eNeg_ppid;
	meTree.eNegPt        = eNeg_Pt;
	meTree.eNegEta       = eNeg_Eta;
	meTree.eNegPhi       = eNeg_Phi;
	meTree.eNegX         = eNeg_x;
	meTree.eNegY         = eNeg_y;
	meTree.eNegZ         = eNeg_z;

	//meTree.ePaPosPt        = ePaPos_Pt;
	//meTree.ePaPosEta       = ePaPos_Eta;
	//meTree.ePaPosPhi       = ePaPos_Phi;
	//meTree.ePaPosY         = ePaPos_Y;
	//meTree.ePaNegPt        = ePaNeg_Pt;
	//meTree.ePaNegEta       = ePaNeg_Eta;
	//meTree.ePaNegPhi       = ePaNeg_Phi;
	//meTree.ePaNegY         = ePaNeg_Y;

	eTree->Fill();

	nevent ++;


    }//event
    cout<<"test out loop"<<endl;

    //============================= output ==============================

    char rootfilename[100];
    sprintf(rootfilename,"output_%i/%s/pythiaevent%d%d.root",(int)energy,folder,irun/10,irun%10);
    TFile* file = new TFile(rootfilename,"RECREATE");
    //file->SetCompressionLevel(9);
    file->cd();

    eTree->Write();
    hnEvents->Write();
    heeMvsPt_AllAcc->Write();
    heeMvsPt_TrgAcc->Write();
    heeMvsPt_TrgAcc0->Write();
    heedPhi_All->Write();
    heedPhi_Acc->Write();
    heedPhi_Acc0->Write();

    //file->Close();

    myPythia6->Pystat(1);

    stopWatch->Stop();
    stopWatch->Print();
}
