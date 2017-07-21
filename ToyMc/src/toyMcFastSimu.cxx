// include head files
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "sys/types.h"
#include "dirent.h"
#include "math.h"
#include "string.h"
#include "toyMcFastSimu.h"
#include "TMath.h"
//Add the data structure
#include "mTreeDst.h"
#include <iomanip>

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

void main(int argc, char *argv) 
{
    if(argc != 1 && argc != 4) 
    {
	cout<<"Usage: analysis [infile] [output] [option]"<<endl;
	return ;
    }

    inFileName.Form("test.list");
    outFileName.Form("test.root");

    doCharmFlag = false;

    if(argc >= 3)
    {
	inFile = argv[1];
	outFileName.Form("%s", argv[2]);
	doCharmFlag = atoi(argv[3]);
    }

    fout = TFile::Open(outFileName.Data(), "recreate");

    init();

    int it = 0;
    const int nwanted = 1000000;
    int nEntries = chain->GetEntries();

    while(!doCharmFlag) 
    {
	if(it >= nwanted) break;
	TLorentzVector ePmom(0,0,0,0);
	TLorentzVector eMmom(0,0,0,0);
	TVector3 ePpos(0,0,0);
	TVector3 eMpos(0,0,0);
	getDaughters(doCharmFlag, it, ePmom, ePpos, eMmom, eMpos);
	fill(doCharmFlag, ePmom, ePpos, eMmom, eMpos);
	it++;
    }

    it = 0;
    while(doCharmFlag)
    {
	if(it >= nEntries) break;
	TLorentzVector ePmom(0,0,0,0);
	TLorentzVector eMmom(0,0,0,0);
	TVector3 ePpos(0,0,0);
	TVector3 eMpos(0,0,0);
	getDaughters(doCharmFlag, it, ePmom, ePpos, eMmom, eMpos);
	fill(doCharmFlag, ePmom, ePpos, eMmom, eMpos);
	it++;
    }

    fout->Write();
    fout->Close();
}

//---------------------------------------------------------
void init()
{
    loadData("pathtofile");
    // book output object
    bookObject();
    chain = new TChain("mTreeDst");
    gRandom = new TRandom3();
    gRandom->SetSeed(0);

    if(!doCharmFlag) 
    {
	cout<<"No charm only VP !!!"<<endl;
	return;
    }

    cout<<"Init for charm tree chain..."<<endl;

    int ifile = 0;
    char filename[512];
    ifstream *inputStream = new ifstream;
    inputStream->open(inFile);

    if(!(inputStream))
    {
	cout<<"Error : Can't open list file!!"<<endl;
	return;
    }
    while(inputStream->good())
    {
	inputStream->getline(filename,512);
	if(inputStream->good())
	{
	    TFile *ftmp = new TFile(filename);
	    if(!ftmp||!(ftmp->IsOpen())||!(ftmp->GetNkeys()))
	    {
		cout<<"Error : Can't open file or file has no key!!"<<endl;
	    }
	    else 
	    {
		cout<<"  read in"<<ifile<<"th file: "<<filename<<endl;
		chain->Add(filename);
		ifile++;
	    }
	    delete ftmp;
	}
    }
    delete inputStream;
    tree = new mTreeDst(chain);
    cout<<"Init for charm tree chain... done"<<endl;
}

void loadData(char *filename) 
{
    TString lazy;
    TFile *fData = TFile::Open(filename);
    h1Vz = (TH1D *)fData->Get("h1Vz");
    for(int iEta=0; iEta<nEtas; iEta++) 
	for(int iVz=0; iVz<nVzs; iVz++) 
	    for(int iPt=0; iPt<nPtBins; iPt++)
	    {
		lazy.Form("h2Dca_Eta%i_Vz%i_Pt%i", iEta, iVz, iPt);
		h2Dca[iEta][iVz][iPt] = (TH1I *) fData->Get(lazy.Data());
	    }

}

void bookObject(TFile *fout) 
{
    fout->cd();
    int BufSize = (int)pow(2., 16.);
    nt = new TNtuple(
	    "nt", "", 
	    "vx:vy:vz"  // event info
	    "pid:m:pt:eta:phi:y" // Mc parent
	    "rm:rpt:reta:rphi:ry"
	    "dcaPair:decayL:dcaV0:"
	    "ePpt:ePeta:ePphi:ePx:ePy:ePz"
	    "eMpt:eMeta:eMphi:eMx:eMy:eMz"
	    "ePRpt:ePReta:ePRphi:ePRx:ePRy:ePRz"
	    "eMRpt:eMReta:eMRphi:eMRx:eMRy:eMRz"
	    "ePDca:ePRDca:ePRDcaXY:ePRDcaZ"
	    "eMDca:eMRDca:eMRDcaXY:eMRDcaZ"
	    "ePDcaPosX:ePDcaPosY:ePDcPosZ"
	    "eMDcaPosX:eMDcaPosY:eMDcPosZ"
	    ,BufSize);
}

void getDaughters(bool option, int iter, TLorentzVector &ePmom, TVector3 &ePpos, TLorentzVector &eMmom, TVector3 &eMpos)
{
    if(!option) {
	virtualPhotonDecay(ePmom, eMmom);
	ePpos->SetXYZ(0,0,0);
	eMpos->SetXYZ(0,0,0);
    }
    if(option) {
	loadCharmTree(iter, ePmom. ePpos, eMmom, eMpos);
    }
}

void loadCharmTree(int iter, TLorentzVector &ePmom, TVector3 &ePos, TLorentzVector &eMmom, TVector3 &eMpos) 
{
    if(iter >= chain->GetEntries()) return;
    chain->GetEntry(iter);
    ePmom->SetPtEtaPhiM(tree->ePosPt, tree->ePosEta, tree->ePosPhi, ME);
    ePpos->SetXYZ(tree->ePosX, tree->ePosY, tree->ePosZ);
    eMmom->SetPtEtaPhiM(tree->eNegPt, tree->eNegEta, tree->eNegPhi, ME);
    eMpos->SetXYZ(tree->eNegX, tree->eNegY, tree->eNegZ);
}

void virtualPhotonDecay(TLorentzVector &ePmom, TLorentzVector &eMmom) 
{
    TLorentzVector Gmom;
    double mG = gRandom->Rndm()*6.;
    //double mG = hMassShape->GetRandom();
    if(mG<2.*ME) continue;
    double pT       = gRandom->Rndm()*5.; // funMom->GetRandom(); // gRandom->Rndm()*10.;
    double phi      = gRandom->Rndm()*TMath::Pi()*2.;
    double rapidity = (gRandom->Rndm()*2.-1.)*eeYcut;
    //double mT       = TMath::Sqrt(pT*pT+mG*mG);
    double pz       = mT*TMath::SinH(rapidity);
    double p        = TMath::Sqrt(pT*pT+pz*pz);
    double eta      = 0.5*TMath::Log((p+pz)/(p-pz));
    Gmom.SetPtEtaPhiM(pT, eta, phi, mG);

    double p_pri        = TMath::Sqrt(mG*mG-4.*ME*ME)/2.;
    double phi_pri      = gRandom->Rndm()*TMath::Pi()*2.0;                         // phi   0 - 2pi
    double costheta_pri = gRandom->Rndm()*2.-1;
    double pz_pri       = p_pri*costheta_pri;
    double pT_pri       = TMath::Sqrt(p_pri*p_pri-pz_pri*pz_pri);
    double eta_pri      = 0.5*TMath::Log((p_pri+pz_pri)/(p_pri-pz_pri));
    ePmom.SetPtEtaPhiM(pT_pri, eta_pri, phi_pri, ME);
    eMmom.SetPtEtaPhiM(pT_pri, -eta_pri, phi_pri+TMath::Pi(), ME);

    // transform to the lab frame
    ePmom.Boost(Gmom.BoostVector());
    eMmom.Boost(Gmom.BoostVector());
}

void fill(bool option, TLorentzVector const& ePmom, TVector3 const& ePpos, TLorentzVector const& eMmom, TVector3 const& eMpos) 
{
    int const centrality = floor(nCent * gRandom->Rndm());

    TVector3 const vertex = getVertex(centrality);

    TLorentzVector pair = ePmom+ePmom;

    // smear momentum
    TLorentzVector const ePRmom = smearMom(ePmom, fKaonMomResolution);
    TLorentzVector const eMRmom = smearMom(eMmom, fPionMomResolution);

    // smear momentum
    TVector3 const ePRpos = smearPosData(vertex.z(), centrality, ePRmom, ePpos);
    TVector3 const eMRpos = smearPosData(vertex.z(), centrality, eMRmom, eMpos);

    TLorentzVector pairR = ePRmom + eMRmom; 

    float const ePDca    = dca(ePmom.Vect(), ePpos, vertex);
    float const eMDca    = dca(eMmom.Vect(), eMpos, vertex);

    float const ePRDca   = dca(ePRmom.Vect(), ePRpos, vertex);
    float const ePRDcaXY = dcaXY(ePRmom.Vect(), ePRpos, vertex);
    float const ePRDcaZ  = dcaZ(ePRmom.Vect(), ePRpos, vertex);

    float const eMRDca   = dca(eMRmom.Vect(), eMRpos, vertex);
    float const eMRDcaXY = dcaXY(eMRmom.Vect(), eMRpos, vertex);
    float const eMRDcaZ  = dcaZ(eMRmom.Vect(), eMRpos, vertex);

    TVector3 v0; 
    TVector3 posDca1;
    TVector3 posDca2;
    float const dcaPair = dca1To2(ePRmom.Vect(), ePRpos, eMRmom.Vect(), eMRpos, posDca1, posDca2, v0);
    float const decayL = (v0 - vertex).Mag();
    float const dcaV0 = dca(pairR.Vect(), v0, vertex);

    float const mposDca1 = (posDca1 - vertex).Mag();
    float const mposDca2 = (posDca2 - vertex).Mag();

    float arr[100];
    int iter = -1;
    arr[iter++] = vertex.X();
    arr[iter++] = vertex.Y();
    arr[iter++] = vertex.Z();

    arr[iter++] = option? 1 : -1;
    arr[iter++] = pair.M();
    arr[iter++] = pair.Pt();
    arr[iter++] = pair.Eta();
    arr[iter++] = pair.Phi();
    arr[iter++] = pair.Rapidity();
    arr[iter++] = pairR.M();
    arr[iter++] = pairR.Pt();
    arr[iter++] = pairR.Eta();
    arr[iter++] = pairR.Rapidity();
    arr[iter++] = dcaPair;
    arr[iter++] = decayL;
    arr[iter++] = dcaV0;

    arr[iter++] = ePmom.Pt();
    arr[iter++] = ePmom.Eta();
    arr[iter++] = ePmom.Phi();
    arr[iter++] = ePpos.X();
    arr[iter++] = ePpos.Y();
    arr[iter++] = ePpos.Z();

    arr[iter++] = eMmom.Mt();
    arr[iter++] = eMmom.Eta();
    arr[iter++] = eMmom.Mhi();
    arr[iter++] = eMpos.X();
    arr[iter++] = eMpos.Y();
    arr[iter++] = eMpos.Z();

    arr[iter++] = eRPmom.Pt();
    arr[iter++] = eRPmom.Eta();
    arr[iter++] = eRPmom.Phi();
    arr[iter++] = eRPpos.X();
    arr[iter++] = eRPpos.Y();
    arr[iter++] = eRPpos.Z();

    arr[iter++] = eRMmom.Mt();
    arr[iter++] = eRMmom.Eta();
    arr[iter++] = eRMmom.Mhi();
    arr[iter++] = eRMpos.X();
    arr[iter++] = eRMpos.Y();
    arr[iter++] = eRMpos.Z();

    arr[iter++] = ePDca;
    arr[iter++] = ePRDca;
    arr[iter++] = ePRDcaXY;
    arr[iter++] = ePRDcaZ;
    arr[iter++] = eMDca;
    arr[iter++] = eMRDca;
    arr[iter++] = eMRDcaXY;
    arr[iter++] = eMRDcaZ;

    arr[iter++] = posDca1.X(); 
    arr[iter++] = posDca1.Y(); 
    arr[iter++] = posDca1.Z(); 
    arr[iter++] = posDca2.X(); 
    arr[iter++] = posDca2.Y(); 
    arr[iter++] = posDca2.Z(); 

    nt->Fill(arr);
}

float dca(TVector3 const& p, TVector3 const& pos, TVector3 const& vertex)
{
    TVector3 posDiff = pos - vertex;
    return fabs(p.Cross(posDiff.Cross(p)).Unit().Dot(posDiff));
}

float dcaSigned(TVector3 const& p, TVector3 const& pos, TVector3 const& vertex)
{
    TVector3 posDiff = pos - vertex;
    float sign = posDiff.x()*p.y()-posDiff.y()*p.x() > 0 ? +1 : -1; 

    return sign*p.Cross(posDiff.Cross(p)).Unit().Dot(posDiff);
}

float dcaXY(TVector3 const& p, TVector3 const& pos, TVector3 const& vertex)
{
    TVector3 newPos(pos);
    newPos.SetZ(0);

    TVector3 newP(p);
    newP.SetZ(0);

    TVector3 newVertex(vertex);
    newVertex.SetZ(0);

    TVector3 posDiff = newPos - newVertex;
    float sign = posDiff.x()*p.y()-posDiff.y()*p.x() > 0 ? +1 : -1; 
    return sign*newP.Cross(posDiff.Cross(newP)).Unit().Dot(posDiff);
}

float dcaZ(TVector3 const& p, TVector3 const& pos, TVector3 const& vertex)
{
    TVector3 posDiff = pos - vertex;
    if(sin(p.Theta())==0) return 0;
    else return (-posDiff.x()*cos(p.Phi())-posDiff.y()*sin(p.Phi()))*cos(p.Theta())/sin(p.Theta())+posDiff.z();
}

float dca1To2(TVector3 const& p1, TVector3 const& pos1, TVector3 const& p2, TVector3 const& pos2, TVector3& posDca1, TVector3& posDca2, TVector3& v0)
{
    TVector3 posDiff = pos2 - pos1;
    TVector3 pu1 = p1.Unit();
    TVector3 pu2 = p2.Unit();
    double pu1Pu2 = pu1.Dot(pu2);
    double g = posDiff.Dot(pu1);
    double k = posDiff.Dot(pu2);
    double s2 = (k - pu1Pu2 * g) / (pu1Pu2 * pu1Pu2 - 1.);
    double s1 = g + s2 * pu1Pu2;
    // TVector3 posDca1 = pos1 + pu1 * s1;
    // TVector3 posDca2 = pos2 + pu2 * s2;
    posDca1 = pos1 + pu1 * s1;
    posDca2 = pos2 + pu2 * s2;
    v0 = 0.5 * (posDca1 + posDca2);
    return (posDca1 - posDca2).Mag();
}

TLorentzVector smearMom(TLorentzVector const& b, TF1 const * const fMomResolution)
{
    float const pt = b.Perp();
    float const sPt = gRandom->Gaus(pt, pt * fMomResolution->Eval(pt));

    TLorentzVector sMom;
    sMom.SetXYZM(sPt * cos(b.Phi()), sPt * sin(b.Phi()), sPt * sinh(b.PseudoRapidity()), b.M());
    return sMom;
}

TVector3 smearPos(TLorentzVector const& mom, TLorentzVector const& rMom, TVector3 const& pos)
{
    float thetaMCS = 13.6 / mom.Beta() / rMom.P() / 1000 * sqrt(pxlLayer1Thickness / fabs(sin(mom.Theta())));
    float sigmaMCS = thetaMCS * 28000 / fabs(sin(mom.Theta()));
    float sigmaPos = sqrt(pow(sigmaMCS, 2) + pow(sigmaPos0, 2));

    return TVector3(gRandom->Gaus(pos.X(), sigmaPos), gRandom->Gaus(pos.Y(), sigmaPos), gRandom->Gaus(pos.Z(), sigmaPos));
}

TVector3 smearPosData(double const vz, int cent, TLorentzVector const& rMom, TVector3 const& pos)
{
    int const iEtaIndex = getEtaIndex(rMom.PseudoRapidity());
    int const iVzIndex = getVzIndex(vz);
    int const iPtIndex = getPtIndex(rMom.Perp());

    double sigmaPosZ = 0;
    double sigmaPosXY = 0;

    if(cent == 8) cent = 7;

    gRandom->SetSeed(0);
    h2Dca[iEtaIndex][iVzIndex][iPtIndex]->GetRandom2(sigmaPosXY,sigmaPosZ);
    sigmaPosZ *= 1.e4;
    sigmaPosXY *= 1.e4;
    /*if (h1DcaZ1[iParticleIndex][iEtaIndex][iVzIndex][cent][iPtIndex]->ComputeIntegral())
      {
      do sigmaPosZ = h1DcaZ1[iParticleIndex][iEtaIndex][iVzIndex][cent][iPtIndex]->GetRandom() * 1e4;
      while (fabs(sigmaPosZ) > 1.e3);
      }

      if (h1DcaXY1[iParticleIndex][iEtaIndex][iVzIndex][cent][iPtIndex]->ComputeIntegral())
      {
      do sigmaPosXY = h1DcaXY1[iParticleIndex][iEtaIndex][iVzIndex][cent][iPtIndex]->GetRandom() * 1e4;
      while (fabs(sigmaPosXY) > 1.e3);
      }
     */

    TVector3 newPos(pos);
    newPos.SetZ(0);
    TVector3 momPerp(-rMom.Vect().Y(), rMom.Vect().X(), 0.0);
    newPos -= momPerp.Unit() * sigmaPosXY;

    return TVector3(newPos.X(), newPos.Y(), pos.Z() + sigmaPosZ);
}

int getPtIndex(double pT)
{
    for (int i = 0; i < nPtBins; i++)
    {
	if ((pT >= ptEdge[i]) && (pT < ptEdge[i + 1]))
	    return i;
    }
    return nPtBins - 1 ;
}

int getEtaIndex(double Eta)
{
    for (int i = 0; i < nEtas; i++)
    {
	if ((Eta >= EtaEdge[i]) && (Eta < EtaEdge[i + 1]))
	    return i;
    }
    return nEtas - 1 ;
}

int getVzIndex(double Vz)
{
    for (int i = 0; i < nVzs; i++)
    {
	if ((Vz >= VzEdge[i]) && (Vz < VzEdge[i + 1]))
	    return i;
    }
    return nVzs - 1 ;
}

