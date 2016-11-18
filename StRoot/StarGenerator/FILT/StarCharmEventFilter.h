#ifndef Star_StCharmEventFilter
#define Star_StCharmEventFilter
#include <vector>
#include <string>
#include "StarGenerator/FILT/StarFilterMaker.h"
#include "StarGenerator/EVENT/StarGenEvent.h"

class StarGenParticleMaster;
class StarGenParticle;
class StarGenEvent;

class StCharmEventFilter : public StarFilterMaker {
    public:
	StCharmEventFilter();
	virtual ~StCharmEventFilter();
	
	Int_t Filter(StarGenEvent *mEvent);

	ClassDef(StCharmEventFilter, 1);
   private:
	float etaCut;
	float minPt;
};
#endif
