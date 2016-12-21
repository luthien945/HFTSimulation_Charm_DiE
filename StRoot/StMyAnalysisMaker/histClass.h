#ifndef histClass_H
#define histClass_H

class TH1F;
class TH2F;
class TFile;
class TList;

class histClass {
    public:
	histClass();
	histClass(char * filename);
	histClass(TFile *outFile);
	~histClass();
	int initHist();
	int writeHist();
	int deleteHist();
	TList *listHist();

	TFile *mFile;
	TList *mList;
	// Add histogram
	TH1F *hEvtCounter; // 0 nevt, 1~50 GeantId
	TH2F *hMc_VxVy;
	TH1F *hMc_Vz;
	TH2F *hRc_VxVy;
	TH1F *hRc_Vz;
	TH1F *hVxDiff;
	TH1F *hVyDiff;
	TH1F *hVzDiff;
	TH1F *hRefMult;
	TH1F *hgRefMult;

	// inclusive track info
	TH2F *hPid_GenIdVsParentGenId;
	TH1F *hPid_GenIdPrimary;
	TH2F *hPid_GenIdVsCharmPid;
	TH1F *hPid_CharmPid;

	TH2F *hParDisCharm;
	TH2F *hParDisMeson;

	TH2F *hDecayLMcVsP_charm;
	TH2F *hDecayLMcVsP_meson;

	TH2F *hRc_DcaVsPt;
	TH2F *hRc_DcaVsPhi;
	TH2F *hRc_DcaVsEta;

	TH2F *hRc_DcaXYVsPt;
	TH2F *hRc_DcaXYVsPhi;
	TH2F *hRc_DcaXYVsEta;

	TH2F *hRc_DcaZVsPt;
	TH2F *hRc_DcaZVsPhi;
	TH2F *hRc_DcaZVsEta;

	TH2F *hMc_DcaVsPt;
	TH2F *hMc_DcaVsPhi;
	TH2F *hMc_DcaVsEta;

	TH2F *hMc_DcaXYVsPt;
	TH2F *hMc_DcaXYVsPhi;
	TH2F *hMc_DcaXYVsEta;

	TH2F *hMc_DcaZVsPt;
	TH2F *hMc_DcaZVsPhi;
	TH2F *hMc_DcaZVsEta;

	TH2F *hRc_HFTMatched_DcaVsPt   ;
	TH2F *hRc_HFTMatched_DcaXYVsPt ;
	TH2F *hRc_HFTMatched_DcaZVsPt  ;

	TH2F *hPtVsDiffPt ;

	TH2F *hDcaVsPt[4];
	TH2F *hDcaXYVsPt[4];
	TH2F *hDcaZVsPt[4];

	// HFT match test
	TH2F *hMc_PtVsPhi;
	TH2F *hMc_EtaVsPhi;
	TH2F *hMc_PtVsHFTMatch; // 0 All, 1 pix1, 2, pix1&&pix2, 3 pix1&&pix2&&IST

	TH2F *hRc_PtVsPhi;
	TH2F *hRc_EtaVsPhi;
	TH2F *hRc_HFTMatched_PtVsPhi;
	TH2F *hRc_HFTMatched_EtaVsPhi;

	TH2F *hRc_PtVsHFTMatch; // 0 All, 1 pix1, 2, pix1&&pix2, 3 pix1&&pix2&&IST
	TH2F *hRc_charm_PtVsHFTMatch; // 0 All, 1 pix1, 2, pix1&&pix2, 3 pix1&&pix2&&IST

	// hist for e+e-
	TH2F *hMc_NevsNp;
	TH2F *hMc_incl_DcaVsPt;
	TH2F *hMc_charm_DcaVsPt;

	TH2F *hMc_incl_DcaXYVsPt;
	TH2F *hMc_charm_DcaXYVsPt;

	TH2F *hMc_incl_DcaZVsPt;
	TH2F *hMc_charm_DcaZVsPt;

	TH2F *hRc_NevsNp;

	TH2F *hRc_incl_DcaVsPt;
	TH2F *hRc_charm_DcaVsPt;

	TH2F *hRc_incl_DcaXYVsPt;
	TH2F *hRc_charm_DcaXYVsPt;

	TH2F *hRc_incl_DcaZVsPt;
	TH2F *hRc_charm_DcaZVsPt;

	TH2F *hRc_incl_DcaVsPt_TrueM;
	TH2F *hRc_charm_DcaVsPt_TrueM;

	TH2F *hRc_incl_DcaXYVsPt_TrueM;
	TH2F *hRc_charm_DcaXYVsPt_TrueM;

	TH2F *hRc_incl_DcaZVsPt_TrueM;
	TH2F *hRc_charm_DcaZVsPt_TrueM;

	// pair 
	TH1F *hPairMass;
};
#endif
