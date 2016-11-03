#include "StPicoMcVertex.h"
#include "StPicoCut.h"
#include "StPicoConstants.h"
#include "StPicoDstMaker.h"
#include "StMcEvent/StMcEvent.hh"
#include "StMcEvent/StMcTrack.hh"
#include "StMcEvent/StMcVertex.hh"


ClassImp(StPicoMcVertex)
//----------------------------------------------- 
StPicoMcVertex::StPicoMcVertex() 
    : mIndex(Pico::SHORTMAX), mParentId(Pico::SHORTMAX), mPosition(0,0,0) 
{
} 
//----------------------------------------------- 
StPicoMcVertex::StPicoMcVertex(StPicoMcVertex *v) 
    : mIndex(v->mIndex), mParentId(v->mParentId), mPosition(v->mPosition), 
    mDaughterId(v->mDaughterId) 
{
}
//----------------------------------------------- 
StPicoMcVertex::StPicoMcVertex(const StMcVertex *mcVtx) 
{
    mIndex  = mcVtx->key();
    mPosition = mcVtx->position();
    const StMcTrack *parentTrack = mcVtx->parent();
    if(parentTrack) mParentId = parentTrack->key();
    else mParentId = Pico::SHORTMAX;

    UShort_t tempId = Pico::SHORTMAX;
    for(int it=0; it<mcVtx->numberOfDaughters(); it++) {
        const StMcTrack *tmpTrk = mcVtx->daughter(it);
	if(!tmpTrk) continue;
	tempId = tmpTrk->key();
	mDaughterId.push_back(tempId);
    }
}

//----------------------------------------------- 
StPicoMcVertex::~StPicoMcVertex() 
{
    mDaughterId.clear();
}

//----------------------------------------------- 
void StPicoMcVertex::print(const StMcVertex * mcVtx) const {
    int mParGid = -999;
    const StMcTrack *parentTrack = mcVtx->parent();
    if(!parentTrack) return;
    mParGid = parentTrack->geantId();
    if(mParGid<12037 || mParGid>12044) return;
    cout<<"//============================================"<<endl;
    cout<<"test McVertex : "<<mIndex<<endl;
    cout<<"test ParentId : "<<mParentId<<endl;
    cout<<"test ParentGeId : "<<mParGid<<endl;
    cout<<"test number of daughter: "<<nDaughters()<<endl;
    for(int it=0; it<mcVtx->numberOfDaughters(); it++) {
        const StMcTrack *tmpTrk = mcVtx->daughter(it);
	if(!tmpTrk) continue;
	cout<<"  daughter"<<it+1<<" GeId : "<< tmpTrk->geantId()<<endl;
    }
}
