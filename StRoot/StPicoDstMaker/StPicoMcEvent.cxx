#include "StPicoMcEvent.h"

#include "StMcEvent.hh"
#include "StMcVertex.hh"
StPicoMcEvent::StPicoMcEvent() : 
  mVertex()
{
  ;
}
StPicoMcEvent::StPicoMcEvent(StMcEvent const* mcEvent, map<Int_t, Int_t> mcKey2PicoId, map<Int_t, Int_t> picoId2McKey) : 
  mVertex(mcEvent->primaryVertex()->position()),
  mMcKey2PicoId(mcKey2PicoId), 
  mPicoId2McKey(picoId2McKey)
{
  ;
}
