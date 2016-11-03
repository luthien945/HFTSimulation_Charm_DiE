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
	TH2F *hVxVy;
	TH1F *hVz;
	TH1F *hRefMult;
	TH1F *hgRefMult;

	// inclusive track info
	TH2F *hPid_GenIdVsParentGenId;
	TH1F *hPid_GenIdPrimary;

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

	// hist for e+e-
	TH2F *hMc_NevsNp;
	TH2F *hMc_incl_DcaVsPt;
	TH2F *hMc_incl_DcaVsPhi;
	TH2F *hMc_incl_DcaVsEta;
	TH2F *hMc_charm_DcaVsPt;
	TH2F *hMc_charm_DcaVsPhi;
	TH2F *hMc_charm_DcaVsEta;

	TH2F *hMc_incl_DcaXYVsPt;
	TH2F *hMc_incl_DcaXYVsPhi;
	TH2F *hMc_incl_DcaXYVsEta;
	TH2F *hMc_charm_DcaXYVsPt;
	TH2F *hMc_charm_DcaXYVsPhi;
	TH2F *hMc_charm_DcaXYVsEta;

	TH2F *hMc_incl_DcaZVsPt;
	TH2F *hMc_incl_DcaZVsPhi;
	TH2F *hMc_incl_DcaZVsEta;
	TH2F *hMc_charm_DcaZVsPt;
	TH2F *hMc_charm_DcaZVsPhi;
	TH2F *hMc_charm_DcaZVsEta;

	TH2F *hRc_NevsNp;
	TH2F *hRc_incl_DcaVsPt;
	TH2F *hRc_incl_DcaVsPhi;
	TH2F *hRc_incl_DcaVsEta;
	TH2F *hRc_charm_DcaVsPt;
	TH2F *hRc_charm_DcaVsPhi;
	TH2F *hRc_charm_DcaVsEta;

	TH2F *hRc_incl_DcaXYVsPt;
	TH2F *hRc_incl_DcaXYVsPhi;
	TH2F *hRc_incl_DcaXYVsEta;
	TH2F *hRc_charm_DcaXYVsPt;
	TH2F *hRc_charm_DcaXYVsPhi;
	TH2F *hRc_charm_DcaXYVsEta;

	TH2F *hRc_incl_DcaZVsPt;
	TH2F *hRc_incl_DcaZVsPhi;
	TH2F *hRc_incl_DcaZVsEta;
	TH2F *hRc_charm_DcaZVsPt;
	TH2F *hRc_charm_DcaZVsPhi;
	TH2F *hRc_charm_DcaZVsEta;

	// pair 
	TH1F *hPairMass;
};
#endif
