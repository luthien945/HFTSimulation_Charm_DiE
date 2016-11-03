#ifndef StPicoMcEvent_hh
#define StPicoMcEvent_hh

#include "TObject.h"
#include "StThreeVectorF.hh"
#include <map>

class StMcEvent;
class StPicoMcEvent : public TObject {

 public:
  StPicoMcEvent();
  ~StPicoMcEvent(){ ; };
  StPicoMcEvent( StMcEvent const* mcEvent, map<Int_t, Int_t> mcKey2PicoId, map<Int_t, Int_t> picoId2McKey);
  StThreeVectorF const& pVertex() const;
  map<Int_t, Int_t> const mcKey2PicoId() const;
  map<Int_t, Int_t> const picoId2McKey() const;
 private:
  StThreeVectorF mVertex;
  map<Int_t, Int_t> mMcKey2PicoId;
  map<Int_t, Int_t> mPicoId2McKey;


  ClassDef(StPicoMcEvent,1)
};
inline StThreeVectorF const& StPicoMcEvent::pVertex() const { return mVertex; };
inline map<Int_t, Int_t> const StPicoMcEvent::mcKey2PicoId() const { return mMcKey2PicoId;};
inline map<Int_t, Int_t> const StPicoMcEvent::picoId2McKey() const { return mPicoId2McKey;};


#endif
