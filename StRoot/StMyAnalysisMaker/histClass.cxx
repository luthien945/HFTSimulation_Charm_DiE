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
	delete mList_EvtQA;
	delete mList_TrackInfo;
	delete mList_DcaInfo;
    }
}

int histClass::initHist() {
    mFile->cd();
    //mList = new TList();
    TString lazy;
    double PI = TMath::Pi();
    mList_EvtQA = new TList();
    mList_TrackInfo = new TList();
    mList_DcaInfo = new TList();

    //============================
    // EventQA
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
    hMc_Charm_NeVsNc = new TH2F("hMc_Charm_NeVsNc","", 20, 0, 20, 20, 0, 20);

    //============================
    // inclusive track info
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

    //============================
    // DcaInfo
    char *name[4] = {"Electron", "Pion", "Kaon", "Proton"};
    for(int i=0; i<4; i++) {
	lazy.Form("hDcaXYVsPti_Neg_%s", name[i]);
	hDcaXYVsPt_Neg[i] = new TH2F(lazy.Data(), "", 500, -0.1, 0.1, 200, 0, 5);
	lazy.Form("hDcaZVsPt_Neg_%s", name[i]);
	hDcaZVsPt_Neg[i]  = new TH2F(lazy.Data(), "", 500, -0.1, 0.1, 200, 0, 5);
	lazy.Form("hDcaVsPt_Neg_%s", name[i]);
	hDcaVsPt_Neg[i]   = new TH2F(lazy.Data(), "", 500, 0, 0.2, 200, 0, 5);

	lazy.Form("hDcaXYVsPti_Pos_%s", name[i]);
	hDcaXYVsPt_Pos[i] = new TH2F(lazy.Data(), "", 500, -0.1, 0.1, 200, 0, 5);
	lazy.Form("hDcaZVsPt_Pos_%s", name[i]);
	hDcaZVsPt_Pos[i]  = new TH2F(lazy.Data(), "", 500, -0.1, 0.1, 200, 0, 5);
	lazy.Form("hDcaVsPt_Pos_%s", name[i]);
	hDcaVsPt_Pos[i]   = new TH2F(lazy.Data(), "", 500, 0, 0.2, 200, 0, 5);
    }

    hMc_PtVsPhi             = new TH2F("hMc_PtVsPhi"             , "" , 300 , -6.  , 6.  , 360 , 0 , 2*PI );
    hMc_EtaVsPhi            = new TH2F("hMc_EtaVsPhi"            , "" , 240 , -1.2 , 1.2 , 360 , 0 , 2*PI );
    hMc_PtVsHFTMatch        = new TH2F("hMc_PtVsHFTMatch"        , "" , 300 , 0    , 6.  , 8   , 0 , 8    );
    hRc_PtVsPhi             = new TH2F("hRc_PtVsPhi"             , "" , 300 , -6.  , 6.  , 360 , 0 , 2*PI );
    hRc_EtaVsPhi            = new TH2F("hRc_EtaVsPhi"            , "" , 240 , -1.2 , 1.2 , 360 , 0 , 2*PI );
    hRc_HFTMatched_PtVsPhi  = new TH2F("hRc_HFTMatched_PtVsPhi"  , "" , 300 , -6.  , 6.  , 360 , 0 , 2*PI );
    hRc_HFTMatched_EtaVsPhi = new TH2F("hRc_HFTMatched_EtaVsPhi" , "" , 240 , -1.2 , 1.2 , 360 , 0 , 2*PI );
    hRc_PtVsHFTMatch        = new TH2F("hRc_PtVsHFTMatch"        , "" , 300 , 0    , 6.  , 8   , 0 , 8    );
    hRc_charm_PtVsHFTMatch  = new TH2F("hRc_charm_PtVsHFTMatch"  , "" , 300 , 0    , 6.  , 8   , 0 , 8    );

    hMc_incl_DcaVsPt    = new TH2F("hMc_incl_DcaVsPt"     , "" , 500 , 0    , 0.1 , 200 , 0 , 5. );
    hMc_incl_DcaXYVsPt  = new TH2F("hMc_incl_DcaXYVsPt"   , "" , 500 , -0.1 , 0.1 , 200 , 0 , 5. );
    hMc_incl_DcaZVsPt   = new TH2F("hMc_incl_DcaZVsPt"    , "" , 500 , -0.1 , 0.1 , 200 , 0 , 5. );

    hMc_charm_DcaVsPt    = new TH2F("hMc_charm_DcaVsPt"   , "" , 500 , -0.1 , 0.1 , 200 , 0 , 5. );
    hMc_charm_DcaXYVsPt  = new TH2F("hMc_charm_DcaXYVsPt" , "" , 500 , -0.1 , 0.1 , 200 , 0 , 5. );
    hMc_charm_DcaZVsPt   = new TH2F("hMc_charm_DcaZVsPt"  , "" , 500 , -0.1 , 0.1 , 200 , 0 , 5. );

    hRc_incl_DcaVsPt    = new TH2F("hRc_incl_DcaVsPt"     , "" , 500 , 0    , 0.1 , 200 , 0 , 5. );
    hRc_incl_DcaXYVsPt  = new TH2F("hRc_incl_DcaXYVsPt"   , "" , 500 , -0.1 , 0.1 , 200 , 0 , 5. );
    hRc_incl_DcaZVsPt   = new TH2F("hRc_incl_DcaZVsPt"    , "" , 500 , -0.1 , 0.1 , 200 , 0 , 5. );

    hRc_charm_DcaVsPt    = new TH2F("hRc_charm_DcaVsPt"   , "" , 500 , 0    , 0.1 , 200 , 0 , 5. );
    hRc_charm_DcaXYVsPt  = new TH2F("hRc_charm_DcaXYVsPt" , "" , 500 , -0.1 , 0.1 , 200 , 0 , 5. );
    hRc_charm_DcaZVsPt   = new TH2F("hRc_charm_DcaZVsPt"  , "" , 500 , -0.1 , 0.1 , 200 , 0 , 5. );

    hRc_incl_DcaVsPt_TrueM    = new TH2F("hRc_incl_DcaVsPt_TrueM"    , "" , 500 , 0    , 0.1 , 200 , 0 , 5. );
    hRc_incl_DcaXYVsPt_TrueM  = new TH2F("hRc_incl_DcaXYVsPt_TrueM"  , "" , 500 , -0.1 , 0.1 , 200 , 0 , 5. );
    hRc_incl_DcaZVsPt_TrueM   = new TH2F("hRc_incl_DcaZVsPt_TrueM"   , "" , 500 , -0.1 , 0.1 , 200 , 0 , 5. );

    hRc_charm_DcaVsPt_TrueM   = new TH2F("hRc_charm_DcaVsPt_TrueM"   , "" , 500 , 0    , 0.1 , 200 , 0 , 5. );
    hRc_charm_DcaXYVsPt_TrueM = new TH2F("hRc_charm_DcaXYVsPt_TrueM" , "" , 500 , -0.1 , 0.1 , 200 , 0 , 5. );
    hRc_charm_DcaZVsPt_TrueM  = new TH2F("hRc_charm_DcaZVsPt_TrueM"  , "" , 500 , -0.1 , 0.1 , 200 , 0 , 5. );

    //============================
    // Simulation Input QA
    hMc_Charm_PtVsPid = new TH2F("hMc_Charm_PtVsPid", "", 500, 0, 10, 20, 12030, 12050);
    hMc_Charm_PhiVsPid = new TH2F("hMc_Charm_PhiVsPid", "", 360, -PI, PI, 20, 12030, 12050);
    hMc_Charm_EtaVsPid = new TH2F("hMc_Charm_EtaVsPid", "", 240, -1.2, 1.2, 20, 12030, 12050);
    hMc_Charm_Corr = new TH2F("hMc_Charm_Corr", "", 360, -0.5*PI, 1.5*PI, 500, -10, 10);

    // Add hist to list
    mList_EvtQA->Add(hEvtCounter);
    mList_EvtQA->Add(hMc_VxVy   );
    mList_EvtQA->Add(hMc_Vz     );
    mList_EvtQA->Add(hRc_VxVy   );
    mList_EvtQA->Add(hRc_Vz     );
    mList_EvtQA->Add(hVxDiff    );
    mList_EvtQA->Add(hVyDiff    );
    mList_EvtQA->Add(hVzDiff    );
    mList_EvtQA->Add(hRefMult   );
    mList_EvtQA->Add(hgRefMult  );
    mList_EvtQA->Add(hMc_Charm_NeVsNc);

    mList_TrackInfo->Add(hPid_GenIdVsParentGenId);
    mList_TrackInfo->Add(hPid_GenIdPrimary      );
    mList_TrackInfo->Add(hPid_GenIdVsCharmPid   );
    mList_TrackInfo->Add(hPid_CharmPid          );
    mList_TrackInfo->Add(hDecayLMcVsP_charm     );
    mList_TrackInfo->Add(hDecayLMcVsP_meson     );
    mList_TrackInfo->Add(hParDisCharm           );
    mList_TrackInfo->Add(hParDisMeson           );
    mList_TrackInfo->Add(hMc_Charm_PtVsPid      );
    mList_TrackInfo->Add(hMc_Charm_PhiVsPid     );
    mList_TrackInfo->Add(hMc_Charm_EtaVsPid     );
    mList_TrackInfo->Add(hMc_Charm_Corr         );

    mList_TrackInfo->Add(hRc_DcaVsPt   );
    mList_TrackInfo->Add(hRc_DcaVsPhi  );
    mList_TrackInfo->Add(hRc_DcaVsEta  );
    mList_TrackInfo->Add(hRc_DcaXYVsPt );
    mList_TrackInfo->Add(hRc_DcaXYVsPhi);
    mList_TrackInfo->Add(hRc_DcaXYVsEta);
    mList_TrackInfo->Add(hRc_DcaZVsPt  );
    mList_TrackInfo->Add(hRc_DcaZVsPhi );
    mList_TrackInfo->Add(hRc_DcaZVsEta );
    mList_TrackInfo->Add(hPtVsDiffPt   );

    mList_TrackInfo->Add(hRc_HFTMatched_DcaVsPt  );
    mList_TrackInfo->Add(hRc_HFTMatched_DcaXYVsPt);
    mList_TrackInfo->Add(hRc_HFTMatched_DcaZVsPt );

    mList_TrackInfo->Add(hMc_DcaVsPt   );
    mList_TrackInfo->Add(hMc_DcaVsPhi  );
    mList_TrackInfo->Add(hMc_DcaVsEta  );
    mList_TrackInfo->Add(hMc_DcaXYVsPt );
    mList_TrackInfo->Add(hMc_DcaXYVsPhi);
    mList_TrackInfo->Add(hMc_DcaXYVsEta);
    mList_TrackInfo->Add(hMc_DcaZVsPt  );
    mList_TrackInfo->Add(hMc_DcaZVsPhi );
    mList_TrackInfo->Add(hMc_DcaZVsEta );

    mList_TrackInfo->Add(hMc_PtVsPhi            );
    mList_TrackInfo->Add(hMc_EtaVsPhi           );
    mList_TrackInfo->Add(hMc_PtVsHFTMatch       );
    mList_TrackInfo->Add(hRc_PtVsPhi            );
    mList_TrackInfo->Add(hRc_EtaVsPhi           );
    mList_TrackInfo->Add(hRc_HFTMatched_PtVsPhi );
    mList_TrackInfo->Add(hRc_HFTMatched_EtaVsPhi);
    mList_TrackInfo->Add(hRc_PtVsHFTMatch       );
    mList_TrackInfo->Add(hRc_charm_PtVsHFTMatch );

    mList_DcaInfo->Add(hMc_incl_DcaVsPt   );
    mList_DcaInfo->Add(hMc_incl_DcaXYVsPt );
    mList_DcaInfo->Add(hMc_incl_DcaZVsPt  );
    mList_DcaInfo->Add(hMc_charm_DcaVsPt   );
    mList_DcaInfo->Add(hMc_charm_DcaXYVsPt );
    mList_DcaInfo->Add(hMc_charm_DcaZVsPt  );

    mList_DcaInfo->Add(hRc_incl_DcaVsPt   );
    mList_DcaInfo->Add(hRc_incl_DcaXYVsPt );
    mList_DcaInfo->Add(hRc_incl_DcaZVsPt  );
    mList_DcaInfo->Add(hRc_charm_DcaVsPt   );
    mList_DcaInfo->Add(hRc_charm_DcaXYVsPt );
    mList_DcaInfo->Add(hRc_charm_DcaZVsPt  );

    mList_DcaInfo->Add(hRc_incl_DcaVsPt_TrueM);
    mList_DcaInfo->Add(hRc_incl_DcaXYVsPt_TrueM);
    mList_DcaInfo->Add(hRc_incl_DcaZVsPt_TrueM);
    mList_DcaInfo->Add(hRc_charm_DcaXYVsPt_TrueM);
    mList_DcaInfo->Add(hRc_charm_DcaVsPt_TrueM);
    mList_DcaInfo->Add(hRc_charm_DcaZVsPt_TrueM);

    for(int i=0; i<4; i++) {
	mList_DcaInfo->Add(hDcaVsPt_Pos[i]);
	mList_DcaInfo->Add(hDcaXYVsPt_Pos[i]);
	mList_DcaInfo->Add(hDcaZVsPt_Pos[i]);

	mList_DcaInfo->Add(hDcaVsPt_Neg[i]);
	mList_DcaInfo->Add(hDcaXYVsPt_Neg[i]);
	mList_DcaInfo->Add(hDcaZVsPt_Neg[i]);
    }
    return 1;
}


int histClass::writeHist() {
    if(!mFile)  {
	cout<<"Not output file!!!"<<endl;
	return 0;
    }
    mFile->cd();
    mFile->mkdir("EvtInfo");
    mFile->cd("EvtInfo");
    mList_EvtQA->Write();
    mFile->cd();
    mFile->mkdir("TrackInfo");
    mFile->cd("TrackInfo");
    mList_TrackInfo->Write();
    mFile->cd();
    mFile->mkdir("DcaInfo");
    mFile->cd("DcaInfo");
    mList_DcaInfo->Write();
}

int histClass::deleteHist() {
    //mList->Delete();
    mList_EvtQA->Delete();
    mList_TrackInfo->Delete();
    mList_DcaInfo->Delete();
}
