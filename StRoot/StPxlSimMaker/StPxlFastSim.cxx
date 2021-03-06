/*
 *
 * Author: A. Rose, LBL, Y. Fisyak, BNL, M. Miller, MIT, M. Mustafa
 *
 *
 **********************************************************
 * $Log: StPxlFastSim.cxx,v $
 * Revision 1.4  2014/03/13 17:00:19  mstftsm
 * StPxlSimMaker has a method to switch on random seed for StRandom generatos in simulators. Default is not a random seed.
 *
 * Revision 1.2  2013/11/14 19:10:27  mstftsm
 * StMcPxlHit has been changed to be on local coordinates. We no longer transfor from global to local before smearing
 *
 * Revision 1.1  2013/05/12 21:43:32  jeromel
 * Initial revision, code peer review closed 2013/05/06
 *
 * Revision 1.5  2013/05/09 02:58:36  mstftsm
 * Fixed a bug which called for sensor local Z value.
 *
 *
 */

#include <stdio.h>

#include "StMessMgr.h"
#include "Stypes.h"
#include "Stiostream.h"
#include "StPxlFastSim.h"
#include "StEvent/StPxlHit.h"
#include "StEvent/StPxlHitCollection.h"
#include "StMcEvent/StMcPxlHitCollection.hh"
#include "StMcEvent/StMcPxlHit.hh"
#include "tables/St_HitError_Table.h"
#include "StarClassLibrary/StRandom.hh"
#include "StThreeVectorF.hh"
#include "StPxlDbMaker/StPxlDb.h"
#include "StPxlUtil/StPxlConstants.h"
#include "StPxlUtil/StPxlDigiHit.h"

#include "TGeoManager.h"
#include "TGeoMatrix.h"

#include "TDataSet.h"
#include "TObjectSet.h"
// Sensor status table
#include "pxlStat.h"

StPxlFastSim::~StPxlFastSim()
{
   if (mRandom) delete mRandom;
   if (mPxlDb) delete mPxlDb;
}
//____________________________________________________________
Int_t StPxlFastSim::initRun(const TDataSet& calib_db, const TObjectSet* pxlDbDataSet, const Int_t run)
{
   // run is not used in the current implementation, but might be necessary in the future.
   LOG_INFO << "StPxlFastSim::init()" << endm;

   if(pxlDbDataSet != 0)
   {
	   mPxlDb = (StPxlDb *)pxlDbDataSet->GetObject();
	   if (!mPxlDb)
	   {
		   LOG_ERROR << "StPxlFastSim - E - mPxlDb is not available" << endm;
		   return kStErr;
	   }
	   else
	   {
		   LOG_INFO << "StPxlFastSim - Using geometry from pxlDB" <<endm;
	   }
   }
   else
   {
	   LOG_INFO << "StPxlFastSim - Using ideal geometry" <<endm;
   }

   if (!mRandom) mRandom = new StRandom();
   if(mUseRandomSeed)
   {
	   Int_t seed = time(NULL);
	   mRandom->setSeed(seed);
	   LOG_INFO << "StPxlFastSim - smearing random generator is using random seed = " << seed <<endm;
   }
   else
   {
	   LOG_INFO << "StPxlFastSim - smearing random generator is using default seed" <<endm;
   }

   St_HitError *pxlTableSet = (St_HitError*)calib_db.Find("PixelHitError");

   if (!pxlTableSet)
   {
      LOG_ERROR << "StPxlFastSim - E - PixelHitError is not available" << endm;
      return kStErr;
   }

   HitError_st* pxlHitError = pxlTableSet->GetTable();

   if (!pxlHitError)
   {
      LOG_ERROR << "StPxlFastSim - E - pxl hit table is not available in PixelHitError" << endm;
      return kStErr;
   }

   // please note that what is called local Y in the PXL sensor design
   // is actually called Z in STAR coordinates convention
   if (pxlHitError->coeff[0] <= 0 || pxlHitError->coeff[3] <= 0)
   {
      LOG_ERROR << "StPxlFastSim - E - negative or corrupted PXL hits errors in DB" << endm;
      return kStErr;
   }

   mResXPix = 0.0008;//sqrt(pxlHitError->coeff[0]); // local x
   mResZPix = 0.0008;//sqrt(pxlHitError->coeff[3]); // local Y
   mResYPix = 0;//sqrt(pxlHitError->coeff[2]); // needs to be updated in the DB later

   //Lomnitz :: changes that could be used:
   //mDetUpInner = 0.97;
    //mDetUpOuter = 0.97;
    //mDetUpInner = 0.85;
    //mDetUpOuter = 0.95;
    //mDetEff = 0.85;
    mDetEff = 0.97;
    pxlStat thisrun;    
   for(int i=0;i<400;i++) {
     if(thisrun.sensor[i] > 9 && thisrun.sensor[i] <= 0)
       mSensorMap[i] = 0;
     if(thisrun.sensor[i] == 1)
       mSensorMap[i] =1;
     if(thisrun.sensor[i]==2)
	   mSensorMap[i] =0.95;
     if(thisrun.sensor[i]==3)
       mSensorMap[i] = 0.25;
     //int ladder = (i%40)/10 + 1;
     //mSensorMap[i] = 1;
     //double detUp = (ladder==1) ? mDetUpInner : mDetUpOuter;
     //if(mRandom->flat()>detUp) mSensorMap[i] = 0;
   }

   cout << " ==== PXL Sensor Status Map ==== "<<endl;
   float innerAcc = 0;
   float outerAcc =0;
   for(int i=0;i<40;i++) {
     if( i%4==0)
       cout<<" ------------------ "<<endl;
     for(int j=0;j<10;j++) {
       cout << mSensorMap[i*10+j] << " ";
       if( i%4==0 )
	 innerAcc+=mSensorMap[i*10+j]/100.0;
       else
	 outerAcc+=mSensorMap[i*10+j]/300.0;
     }
     cout << endl;
   }
   cout << endl;
   cout<<"Total masked out"<<endl;
   cout<<"Inner layer: "<<innerAcc<<"\t Outer layer: "<<outerAcc<<endl;

   return kStOk;
}
//____________________________________________________________
Int_t StPxlFastSim::addPxlHits(const StMcPxlHitCollection& mcPxlHitCol,
                               StPxlHitCollection& pxlHitCol)
{
   Float_t smearedX = 0, smearedY = 0, smearedZ = 0;


   // Loop over sectors
   for (UInt_t iSec = 0; iSec < mcPxlHitCol.numberOfSectors(); iSec++)
   {
      const StMcPxlSectorHitCollection* mcPxlSectorHitCol = mcPxlHitCol.sector(iSec);
      if (!mcPxlSectorHitCol) continue;

      for (UInt_t iLad = 0; iLad < mcPxlSectorHitCol->numberOfLadders(); iLad++)
      {
         const StMcPxlLadderHitCollection* mcPxlLadderHitCol = mcPxlSectorHitCol->ladder(iLad);
         if (!mcPxlLadderHitCol) continue;

         for (UInt_t iSen = 0; iSen < mcPxlLadderHitCol->numberOfSensors(); iSen++)
         {
            const StMcPxlSensorHitCollection* mcPxlSensorHitCol = mcPxlLadderHitCol->sensor(iSen);
            if (!mcPxlSensorHitCol) continue;

            UInt_t nSenHits = mcPxlSensorHitCol->hits().size();
            LOG_DEBUG << "Sector/Ladder/Sensor = " << iSec + 1 << "/" << iLad + 1 << "/" << iSen + 1 << ". Number of sensor hits = " << nSenHits << endm;

            // Loop over hits in the sensor
            for (UInt_t iHit = 0; iHit < nSenHits; iHit++)
            {
               StMcPxlHit* mcPix = mcPxlSensorHitCol->hits()[iHit];

               //Long_t volId = mcPix->volumeId();
               Int_t sector = mcPix->sector();
               Int_t ladder = mcPix->ladder();
               Int_t sensor = mcPix->sensor();

               Int_t index = (sector-1)*40 + (ladder-1)*10 + (sensor-1);
               if(mRandom->flat()>mSensorMap[index]) continue;  // sensor status determined in the whole run

               if(mRandom->flat()>mDetEff) continue;  // detection efficiency, determined hit-by-hit
               
               Double_t localPixHitPos[3] = {mcPix->position().x(), mcPix->position().y(), mcPix->position().z()};

               LOG_DEBUG << "localPixHitPos = " << localPixHitPos[0] << " " << localPixHitPos[1] << " " << localPixHitPos[2] << endm;
               // please note that what is called local Y in the PXL sensor design
               // is actually called Z in STAR coordinates convention and vice-versa
               smearedX = distortHit(localPixHitPos[0], mResXPix, PXL_ACTIVE_X_LENGTH / 2.0);
               smearedZ = distortHit(localPixHitPos[2], mResZPix, PXL_ACTIVE_Y_LENGTH / 2.0);
               if (mResYPix) smearedY = distortHit(localPixHitPos[1], mResYPix, 0.0020); // Not properly constrained yet
               else smearedY = localPixHitPos[1];
               localPixHitPos[0] = smearedX;
               localPixHitPos[2] = smearedZ;
               localPixHitPos[1] = smearedY;
               LOG_DEBUG << "smearedlocal = " << localPixHitPos[0] << " " << localPixHitPos[1] << " " << localPixHitPos[2] << endm;
               Double_t smearedGlobalPixHitPos[3] = {0, 0, 0};
	       localToMatser(localPixHitPos,smearedGlobalPixHitPos,iSec+1,iLad+1,iSen+1);

               StThreeVectorF gpixpos(smearedGlobalPixHitPos);
               StThreeVectorF mRndHitError(0., 0., 0.);

               UInt_t hw = sector * 10 + ladder; // needs to be updated later after clustering alogrithms are finalized

               unsigned short idTruth = mcPix->parentTrack() ? mcPix->parentTrack()->key() : -999;
               unsigned short quality = mcPix->parentTrack() ? 100 : 0;

               StPxlDigiHit* tempHit = new StPxlDigiHit(localPixHitPos, iSec+1, mcPix->ladder(), mcPix->sensor(),
                  gpixpos, mRndHitError, hw, mcPix->dE(), 0, idTruth, quality, mcPix->key());


               LOG_DEBUG << "key() : " << mcPix->key() - 1 << " idTruth: " << mcPix->parentTrack()->key() << endm;
               LOG_DEBUG << "from StMcPxlHit : x= " << mcPix->position().x() << ";  y= " << mcPix->position().y() << ";  z= " << mcPix->position().z() << endm;
               LOG_DEBUG << "pxlHit location x= " << tempHit->position().x() << "; y= " << tempHit->position().y() << "; z= " << tempHit->position().z() << endm;

               pxlHitCol.addHit(tempHit);
            }
         }
      }
   }

   LOG_INFO << " Total number of McPxlHitCollection = " << mcPxlHitCol.numberOfHits() << endm;
   LOG_INFO << " Total number of PxlHitCollection = " << pxlHitCol.numberOfHits() << endm;

   return kStOK;
}

//____________________________________________________________
Double_t StPxlFastSim::distortHit(Double_t x, Double_t res, Double_t constraint)
{
   Double_t test;

   test = x + mRandom->gauss(0, res);

   while (fabs(test) > constraint)
   {
      test = x + mRandom->gauss(0, res);
   }

   return test;
}
//____________________________________________________________
void StPxlFastSim::localToMatser(Double_t* local,Double_t* master,Int_t sector,Int_t ladder,Int_t sensor)
{
	if(mPxlDb)
	{
               TGeoHMatrix *combP = (TGeoHMatrix *)mPxlDb->geoHMatrixSensorOnGlobal(sector, ladder,sensor);
               combP->LocalToMaster(local, master);
	}
	else
	{
		TString Path("");
		LOG_DEBUG << endm;
		Path = Form("/HALL_1/CAVE_1/TpcRefSys_1/IDSM_1/PXMO_1/PXLA_%i/LADR_%i/PXSI_%i/PLAC_1", sector, ladder, sensor);

		gGeoManager->RestoreMasterVolume();
		gGeoManager->CdTop();
		gGeoManager->cd(Path);

		gGeoManager->GetCurrentMatrix()->LocalToMaster(local, master);
	}
}
/*
 *
 * Author: M. Mustafa
 *
 *
 **********************************************************
 * $Log: StPxlFastSim.cxx,v $
 * Revision 1.4  2014/03/13 17:00:19  mstftsm
 * StPxlSimMaker has a method to switch on random seed for StRandom generatos in simulators. Default is not a random seed.
 *
 * Revision 1.2  2013/11/14 19:10:27  mstftsm
 * StMcPxlHit has been changed to be on local coordinates. We no longer transfor from global to local before smearing
 *
 * Revision 1.1  2013/05/12 21:43:32  jeromel
 * Initial revision, code peer review closed 2013/05/06
 *
 * Revision 1.5  2013/05/09 02:58:36  mstftsm
 * Fixed a bug which called for sensor local Z value.
 *
 */

