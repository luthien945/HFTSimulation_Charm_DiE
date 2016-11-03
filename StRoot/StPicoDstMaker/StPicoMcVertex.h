#ifndef StPicoMcVertex_H
#define StPicoMcVertex_H

class StMcVertex;
class StPicoDst;
class StMcTrack;

#include "TObject.h"
#include <stdio.h>
#include <math.h>
#include <vector>
#include <map>
#include "StThreeVectorF.hh"

class StPicoMcVertex : public TObject {
    public:
	StPicoMcVertex();
	~StPicoMcVertex();
	StPicoMcVertex(StPicoMcVertex *);
	StPicoMcVertex(const StMcVertex *);

	Int_t index() const;
	Int_t parentId() const;
	StThreeVectorF const & positions() const;
	Int_t daughterId(int it) const;
	Int_t nDaughters() const;
	void print(const StMcVertex *mcVtx) const;


    protected:
	UShort_t mIndex;
	Short_t mParentId;
	//UShort_t numberOfDaughters;
	std::vector<UShort_t> mDaughterId;
	StThreeVectorF mPosition;
	friend class StPicoDst;

	ClassDef(StPicoMcVertex, 1)
};


inline Int_t StPicoMcVertex::index() const {return (Int_t) mIndex;}
inline Int_t StPicoMcVertex::parentId() const {return (Int_t) mParentId;}
inline StThreeVectorF const & StPicoMcVertex::positions() const {return mPosition;}
inline Int_t StPicoMcVertex::daughterId(int it) const {return it>=mDaughterId.size() ? -999 : (Int_t) mDaughterId[it]; }
inline Int_t StPicoMcVertex::nDaughters() const {return (Int_t) mDaughterId.size();}

#endif
