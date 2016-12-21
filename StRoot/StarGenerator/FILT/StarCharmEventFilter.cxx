#include "StarCharmEventFilter.h"
#include "StarGenerator/EVENT/StarGenParticle.h"
#include "TLorentzVector.h"

ClassImp(StCharmEventFilter);
StCharmEventFilter::StCharmEventFilter() : StarFilterMaker("charmEvent"){
    minPt = 0;
    etaCut = 1.5;
}

StCharmEventFilter::~StCharmEventFilter() {
}

Int_t StCharmEventFilter::Filter(StarGenEvent *mEvent) {
  if(int(mEvent->GetNumberOfParticles()) <= 0)return -1;

  const int nc = 4;
  const int charmPid[nc] = {411,421,431,4122}; // D+, D0,Ds,L_c

  TIter Iterator = mEvent->IterAll();
  StarGenParticle *p = 0;

  int nCharms = 0;
  int nString = 0;

  while( ( p = (StarGenParticle*)Iterator.Next() ) ){
      if(p->GetStatus() != 1) continue;
      int pdgId = p->GetId();
      TLorentzVector mom;
      mom.SetXYZM(p->GetPx(), p->GetPy(), p->GetPz(), p->GetMass());
      if(mom.P()<1e-4) continue;
      if(mom.Perp()<1e-4) continue;

      float rapidity = mom.Rapidity();
      float eta      = mom.Eta();

      bool isCharm = kFALSE;
      for(int i=0; i<nc; i++) {
	  if((abs(pdgId) == charmPid[i])) {
	      isCharm = kTRUE;
	      break;
	  } 
      }

      if(isCharm && fabs(eta)<1.5 && fabs(rapidity)<1.5) nCharms++;
  }

  if(nCharms>=2) return StarGenEvent::kAccept; 
  return StarGenEvent::kReject;
}


