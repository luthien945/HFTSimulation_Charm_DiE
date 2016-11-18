#include "histClass.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TList.h"
#include "TFile.h"
#include "TMath.h"
#include <iostream>
using std::cout;
using std::endl;

histClass::histClass() 
: mFile(NULL), mList(NULL)
{}

histClass::histClass(char *filename) 
{
    mFile = TFile::Open(filename, "recreate");
}

histClass::histClass(TFile *outFile) 
{
    mFile = outFile;
}

histClass::~histClass() {
    if(mList) {
	deleteHist();
	delete mList;
    }
}

int histClass::initHist() {
    mFile->cd();
    double PI = TMath::Pi();
    mList = new TList();

    hEvtCounter = new TH1F("hEvtCounter" , "" , 10    , 0   , 10                       );
    hMc_VxVy    = new TH2F("hMc_VxVy"    , "" , 200   , -10 , 10      , 200 , -10 , 10 );
    hMc_Vz      = new TH1F("hMc_Vz"      , "" , 200   , -10 , 10                       );
    hRc_VxVy    = new TH2F("hRc_VxVy"    , "" , 200   , -10 , 10      , 200 , -10 , 10 );
    hRc_Vz      = new TH1F("hRc_Vz"      , "" , 200   , -10 , 10                       );
    hVxDiff     = new TH1F("hVxDiff"     , "" , 200   , -10 , 10                       );
    hVyDiff     = new TH1F("hVyDiff"     , "" , 200   , -10 , 10                       );
    hVzDiff     = new TH1F("hVzDiff"     , "" , 200   , -10 , 10                       );
    hRefMult    = new TH1F("hRefMult"    , "" , 1000  , 0   , 1000                     );
    hgRefMult   = new TH1F("hgRefMult"   , "" , 1000  , 0   , 1000                     );

    hPid_GenIdVsParentGenId = new TH2F("hPid_GenIdVsParentGenId" , "" , 50  , 0     , 50      , 200 , 0     , 200   );
    hPid_GenIdVsCharmPid    = new TH2F("hPid_GenIdVsCharmPid"    , "" , 50  , 0     , 50      , 20  , 12030 , 12050 );
    hPid_CharmPid           = new TH1F("hPid_CharmPid"           , "" , 20  , 12030 , 12050                         );
    hPid_GenIdPrimary       = new TH1F("hPid_GenIdPrimary"       , "" , 200 , 0     , 200                           );
    hDecayLMcVsP_charm      = new TH2F("hDecayLMcVsP_charm"      , "" , 200 , 0     , 0.05    , 200 , 0     , 6     );
    hDecayLMcVsP_meson      = new TH2F("hDecayLMcVsP_meson"      , "" , 200 , 0     , 0.05    , 200 , 0     , 6     );

    hParDisCharm            = new TH2F("hParDisCharm"            , "" , 200 , 0     , 0.05    , 6   , 0     ,6      );
    hParDisMeson            = new TH2F("hParDisMeson"            , "" , 200 , 0     , 0.05    , 6   , 0     ,6      );

    hRc_DcaVsPt    = new TH2F("hRc_DcaVsPt"    , "" , 500 , -0.25, 0.25 , 200 , 0    , 6.   );
    hRc_DcaVsPhi   = new TH2F("hRc_DcaVsPhi"   , "" , 500 , -0.25, 0.25 , 360 , 0    , 2*PI );
    hRc_DcaVsEta   = new TH2F("hRc_DcaVsEta"   , "" , 500 , -0.25, 0.25 , 240 , -1.2 , 1.2  );
    hRc_DcaXYVsPt  = new TH2F("hRc_DcaXYVsPt"  , "" , 500 , -0.25, 0.25 , 200 , 0    , 6.   );
    hRc_DcaXYVsPhi = new TH2F("hRc_DcaXYVsPhi" , "" , 500 , -0.25, 0.25 , 360 , 0    , 2*PI );
    hRc_DcaXYVsEta = new TH2F("hRc_DcaXYVsEta" , "" , 500 , -0.25, 0.25 , 240 , -1.2 , 1.2  );
    hRc_DcaZVsPt   = new TH2F("hRc_DcaZVsPt"   , "" , 500 , -0.25, 0.25 , 200 , 0    , 6.   );
    hRc_DcaZVsPhi  = new TH2F("hRc_DcaZVsPhi"  , "" , 500 , -0.25, 0.25 , 360 , 0    , 2*PI );
    hRc_DcaZVsEta  = new TH2F("hRc_DcaZVsEta"  , "" , 500 , -0.25, 0.25 , 240 , -1.2 , 1.2  );

    hPtVsDiffPt    = new TH2F("hPtVsDiffPt"    , "" , 200 , 0, 4., 200, -0.5, 0.5);

    hRc_HFTMatched_DcaVsPt    = new TH2F("hRc_HFTMatched_DcaVsPt"    , "" , 500 , -0.25, 0.25 , 200 , 0    , 6.   );
    hRc_HFTMatched_DcaXYVsPt  = new TH2F("hRc_HFTMatched_DcaXYVsPt"  , "" , 500 , -0.25, 0.25 , 200 , 0    , 6.   );
    hRc_HFTMatched_DcaZVsPt   = new TH2F("hRc_HFTMatched_DcaZVsPt"   , "" , 500 , -0.25, 0.25 , 200 , 0    , 6.   );

    hMc_DcaVsPt    = new TH2F("hMc_DcaVsPt"    , "" , 500 , -0.25, 0.25 , 200 , 0    , 6.   );
    hMc_DcaVsPhi   = new TH2F("hMc_DcaVsPhi"   , "" , 500 , -0.25, 0.25 , 360 , 0    , 2*PI );
    hMc_DcaVsEta   = new TH2F("hMc_DcaVsEta"   , "" , 500 , -0.25, 0.25 , 240 , -1.2 , 1.2  );
    hMc_DcaXYVsPt  = new TH2F("hMc_DcaXYVsPt"  , "" , 500 , -0.25, 0.25 , 200 , 0    , 6.   );
    hMc_DcaXYVsPhi = new TH2F("hMc_DcaXYVsPhi" , "" , 500 , -0.25, 0.25 , 360 , 0    , 2*PI );
    hMc_DcaXYVsEta = new TH2F("hMc_DcaXYVsEta" , "" , 500 , -0.25, 0.25 , 240 , -1.2 , 1.2  );
    hMc_DcaZVsPt   = new TH2F("hMc_DcaZVsPt"   , "" , 500 , -0.25, 0.25 , 200 , 0    , 6.   );
    hMc_DcaZVsPhi  = new TH2F("hMc_DcaZVsPhi"  , "" , 500 , -0.25, 0.25 , 360 , 0    , 2*PI );
    hMc_DcaZVsEta  = new TH2F("hMc_DcaZVsEta"  , "" , 500 , -0.25, 0.25 , 240 , -1.2 , 1.2  );


    hMc_PtVsPhi             = new TH2F("hMc_PtVsPhi"             , "" , 300 , -6.  , 6.  , 360 , 0 , 2*PI );
    hMc_EtaVsPhi            = new TH2F("hMc_EtaVsPhi"            , "" , 240 , -1.2 , 1.2 , 360 , 0 , 2*PI );
    hMc_PtVsHFTMatch        = new TH2F("hMc_PtVsHFTMatch"        , "" , 300 , 0    , 6.  , 8   , 0 , 8    );
    hRc_PtVsPhi             = new TH2F("hRc_PtVsPhi"             , "" , 300 , -6.  , 6.  , 360 , 0 , 2*PI );
    hRc_EtaVsPhi            = new TH2F("hRc_EtaVsPhi"            , "" , 240 , -1.2 , 1.2 , 360 , 0 , 2*PI );
    hRc_HFTMatched_PtVsPhi  = new TH2F("hRc_HFTMatched_PtVsPhi"  , "" , 300 , -6.  , 6.  , 360 , 0 , 2*PI );
    hRc_HFTMatched_EtaVsPhi = new TH2F("hRc_HFTMatched_EtaVsPhi" , "" , 240 , -1.2 , 1.2 , 360 , 0 , 2*PI );
    hRc_PtVsHFTMatch        = new TH2F("hRc_PtVsHFTMatch"        , "" , 300 , 0    , 6.  , 8   , 0 , 8    );
    hRc_charm_PtVsHFTMatch  = new TH2F("hRc_charm_PtVsHFTMatch"  , "" , 300 , 0    , 6.  , 8   , 0 , 8    );


    hRc_incl_DcaVsPt    = new TH2F("hRc_incl_DcaVsPt"    , "" , 500 , -0.1 , 0.1 , 200 , 0    , 6.   );
    hRc_incl_DcaXYVsPt  = new TH2F("hRc_incl_DcaXYVsPt"  , "" , 500 , -0.1 , 0.1 , 200 , 0    , 6.   );
    hRc_incl_DcaZVsPt   = new TH2F("hRc_incl_DcaZVsPt"   , "" , 500 , -0.1 , 0.1 , 200 , 0    , 6.   );

    hMc_incl_DcaVsPt    = new TH2F("hMc_incl_DcaVsPt"    , "" , 500 , -0.1 , 0.1 , 200 , 0    , 6.   );
    hMc_incl_DcaXYVsPt  = new TH2F("hMc_incl_DcaXYVsPt"  , "" , 500 , -0.1 , 0.1 , 200 , 0    , 6.   );
    hMc_incl_DcaZVsPt   = new TH2F("hMc_incl_DcaZVsPt"   , "" , 500 , -0.1 , 0.1 , 200 , 0    , 6.   );

    hRc_charm_DcaVsPt    = new TH2F("hRc_charm_DcaVsPt"    , "" , 500 , -0.1 , 0.1 , 200 , 0    , 6.   );
    hRc_charm_DcaXYVsPt  = new TH2F("hRc_charm_DcaXYVsPt"  , "" , 500 , -0.1 , 0.1 , 200 , 0    , 6.   );
    hRc_charm_DcaZVsPt   = new TH2F("hRc_charm_DcaZVsPt"   , "" , 500 , -0.1 , 0.1 , 200 , 0    , 6.   );

    hMc_charm_DcaVsPt    = new TH2F("hMc_charm_DcaVsPt"    , "" , 500 , -0.1 , 0.1 , 200 , 0    , 6.   );
    hMc_charm_DcaXYVsPt  = new TH2F("hMc_charm_DcaXYVsPt"  , "" , 500 , -0.1 , 0.1 , 200 , 0    , 6.   );
    hMc_charm_DcaZVsPt   = new TH2F("hMc_charm_DcaZVsPt"   , "" , 500 , -0.1 , 0.1 , 200 , 0    , 6.   );

    if(!mList) cout<<"no list"<<endl;

    mList->Add(hEvtCounter);
    mList->Add(hMc_VxVy   );
    mList->Add(hMc_Vz     );
    mList->Add(hRc_VxVy   );
    mList->Add(hRc_Vz     );
    mList->Add(hVxDiff    );
    mList->Add(hVyDiff    );
    mList->Add(hVzDiff    );
    mList->Add(hRefMult   );
    mList->Add(hgRefMult  );

    mList->Add(hPid_GenIdVsParentGenId);
    mList->Add(hPid_GenIdPrimary      );
    mList->Add(hPid_GenIdVsCharmPid   );
    mList->Add(hPid_CharmPid          );
    mList->Add(hDecayLMcVsP_charm     );
    mList->Add(hDecayLMcVsP_meson     );
    mList->Add(hParDisCharm           );
    mList->Add(hParDisMeson           );

    mList->Add(hRc_DcaVsPt   );
    mList->Add(hRc_DcaVsPhi  );
    mList->Add(hRc_DcaVsEta  );
    mList->Add(hRc_DcaXYVsPt );
    mList->Add(hRc_DcaXYVsPhi);
    mList->Add(hRc_DcaXYVsEta);
    mList->Add(hRc_DcaZVsPt  );
    mList->Add(hRc_DcaZVsPhi );
    mList->Add(hRc_DcaZVsEta );
    mList->Add(hPtVsDiffPt   );

    mList->Add(hRc_HFTMatched_DcaVsPt  );
    mList->Add(hRc_HFTMatched_DcaXYVsPt);
    mList->Add(hRc_HFTMatched_DcaZVsPt );

    mList->Add(hMc_DcaVsPt   );
    mList->Add(hMc_DcaVsPhi  );
    mList->Add(hMc_DcaVsEta  );
    mList->Add(hMc_DcaXYVsPt );
    mList->Add(hMc_DcaXYVsPhi);
    mList->Add(hMc_DcaXYVsEta);
    mList->Add(hMc_DcaZVsPt  );
    mList->Add(hMc_DcaZVsPhi );
    mList->Add(hMc_DcaZVsEta );

    mList->Add(hMc_PtVsPhi            );
    mList->Add(hMc_EtaVsPhi           );
    mList->Add(hMc_PtVsHFTMatch       );
    mList->Add(hRc_PtVsPhi            );
    mList->Add(hRc_EtaVsPhi           );
    mList->Add(hRc_HFTMatched_PtVsPhi );
    mList->Add(hRc_HFTMatched_EtaVsPhi);
    mList->Add(hRc_PtVsHFTMatch       );
    mList->Add(hRc_charm_PtVsHFTMatch );

    mList->Add(hRc_incl_DcaVsPt   );
    mList->Add(hRc_incl_DcaXYVsPt );
    mList->Add(hRc_incl_DcaZVsPt  );

    mList->Add(hMc_incl_DcaVsPt   );
    mList->Add(hMc_incl_DcaXYVsPt );
    mList->Add(hMc_incl_DcaZVsPt  );

    mList->Add(hRc_charm_DcaVsPt   );
    mList->Add(hRc_charm_DcaXYVsPt );
    mList->Add(hRc_charm_DcaZVsPt  );

    mList->Add(hMc_charm_DcaVsPt   );
    mList->Add(hMc_charm_DcaXYVsPt );
    mList->Add(hMc_charm_DcaZVsPt  );


    return 1;
}


int histClass::writeHist() {
    if(!mFile)  {
	cout<<"Not output file!!!"<<endl;
	return 0;
    }
    mFile->cd();
    mList->Write();
}

int histClass::deleteHist() {
    mList->Delete();
}
