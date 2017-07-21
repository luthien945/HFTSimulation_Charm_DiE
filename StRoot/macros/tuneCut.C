
void tuneCut(char *infile = "Pair_Ntuple.root") {
    gROOT->LoadMacro("rootDiE.C");
    rootinit();
    TFile *f = new TFile(infile);
    TFile *fout = new TFile("histrecord.root","recreate");
    //fout->mkdir("tuneHist_Pt_0.2");
    //fout->mkdir("tuneHist_Pt_0.5");
    //fout->mkdir("tuneHist_Pt_0.7");
    //fout->mkdir("tuneHist_Pt_1.0");

    TNtuple *t = (TNtuple *) f->Get("pairT");
    int n = t->GetEntries();

    //float M, dcaV0, dcaPair, decayL, pt1, fPid1, dca1, dcaXY1, dcaZ1, pt2, fPid2, dca2, dcaXY2;

    const int nstep = 20;
    double cM[100];
    double cC[100];
    double cE[100];
    double REM[100];
    double REC[100];
    double REE[100];
    double RM[100];
    double RC[100];
    double RE[100];
    double RRM[100];
    double RRE[100];
    double x[100];
    cout<<n<<endl;
    TString lazy;
    const int Npts = 8;
    float pt[Npts] = {0.2, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
    TCanvas *c = new TCanvas();
    c->SetBottomMargin(0.2);
    c->SetLeftMargin(0.2);
    c->SetRightMargin(0.05);
    c->SetTopMargin(0.05);

    TH1F *htmp;
    TH2F *h2tmp;
    TCut meson = "fPid1>12100 && fPid2>12100";
    TCut charm = "fPid1>12000 && fPid1<12100"; 
    TCut kineE = "fPid1<12000";

    fout->cd();
    TH1F *hPt = new TH1F("hPt", "", 500, 0, 10);
    t->Draw("pt1>>hPt", meson);
    lazy.Form("hPt1_meson");
    htmp = (TH1F *) hPt->Clone(lazy.Data());
    htmp->Write();

    t->Draw("pt1>>hPt", charm);
    lazy.Form("hPt1_charm");
    htmp = (TH1F *) hPt->Clone(lazy.Data());
    htmp->Write();

    t->Draw("pt1>>hPt", kineE);
    lazy.Form("hPt1_kineE");
    htmp = (TH1F *) hPt->Clone(lazy.Data());
    htmp->Write();

    for(int ipt=0; ipt<Npts; ipt++) {
	lazy.Form("tuneHist_Pt_%.2f", pt[ipt]);
	fout->mkdir(lazy.Data());
	fout->cd(lazy.Data());
	float start = 0.02;
	float stop  = 0.;
	float step  = (start-stop)/nstep; 
	lazy.Form("pt1>%.3f && pt2>%.3f", pt[ipt], pt[ipt]);
	TCut ptCut = lazy.Data();
	TCut cut;
	cout<<"Pt cut : "<<lazy.Data()<<endl; 

	TH2F *hDcaV0VsM  = new TH2F("hDcaV0VsM", "", 200, 0., 1., 300, 0., 3.);
	TH2F *hDcaAVsM   = new TH2F("hDcaAVsM", "", 200, 0., 0.1, 300, 0., 3.);
	TH2F *hDecayLVsM = new TH2F("hDecayLVsM", "", 400, -1., 1., 300, 0., 3.);
	TH1F *h = new TH1F("h","", 300,0,3);

	t->Draw("M:dcaV0>>hDcaV0VsM", ptCut&&meson);
	lazy.Form("hDcaV0VsM_Meson");
	h2tmp = (TH2F *)hDcaV0VsM->Clone(lazy.Data());
	cout<<h2tmp->Integral()<<endl;
	h2tmp->Write();
	t->Draw("M:dcaV0>>hDcaV0VsM", ptCut&&charm);
	lazy.Form("hDcaV0VsM_Charm");
	h2tmp = (TH2F *)hDcaV0VsM->Clone(lazy.Data());
	cout<<h2tmp->Integral()<<endl;
	h2tmp->Write();
	t->Draw("M:dcaV0>>hDcaV0VsM", ptCut&&kineE);
	lazy.Form("hDcaV0VsM_KineE");
	h2tmp = (TH2F *)hDcaV0VsM->Clone(lazy.Data());
	cout<<h2tmp->Integral()<<endl;
	h2tmp->Write();

	t->Draw("M:decayL>>hDecayLVsM", ptCut&&meson);
	lazy.Form("hDecayLVsM_Meson");
	h2tmp = (TH2F *)hDecayLVsM->Clone(lazy.Data());
	cout<<h2tmp->Integral()<<endl;
	h2tmp->Write();
	t->Draw("M:decayL>>hDecayLVsM", ptCut&&charm);
	lazy.Form("hDecayLVsM_Charm");
	h2tmp = (TH2F *)hDecayLVsM->Clone(lazy.Data());
	cout<<h2tmp->Integral()<<endl;
	h2tmp->Write();
	t->Draw("M:decayL>>hDecayLVsM", ptCut&&kineE);
	lazy.Form("hDecayLVsM_KineE");
	h2tmp = (TH2F *)hDecayLVsM->Clone(lazy.Data());
	cout<<h2tmp->Integral()<<endl;
	h2tmp->Write();

	t->Draw("M:0.5*TMath::Sqrt(dca1*dca1+dca2*dca2)>>hDcaAVsM", ptCut&&meson);
	lazy.Form("hDcaAVsM_Meson");
	h2tmp = (TH2F *)hDcaAVsM->Clone(lazy.Data());
	cout<<h2tmp->Integral()<<endl;
	h2tmp->Write();
	t->Draw("M:0.5*TMath::Sqrt(dca1*dca1+dca2*dca2)>>hDcaAVsM", ptCut&&charm);
	lazy.Form("hDcaAVsM_Charm");
	h2tmp = (TH2F *)hDcaAVsM->Clone(lazy.Data());
	cout<<h2tmp->Integral()<<endl;
	h2tmp->Write();
	t->Draw("M:0.5*TMath::Sqrt(dca1*dca1+dca2*dca2)>>hDcaAVsM", ptCut&&kineE);
	lazy.Form("hDcaAVsM_KineE");
	h2tmp = (TH2F *)hDcaAVsM->Clone(lazy.Data());
	cout<<h2tmp->Integral()<<endl;
	h2tmp->Write();

	//if(test) continue;
	for(int i=0; i<nstep; i++) {
	    float dcaCut = stop+step*(i+1); 
	    x[i] = dcaCut;
	    lazy.Form("dca1<%.3f && dca2<%.3f", dcaCut, dcaCut);
	    cut = lazy.Data();
	    cout<<"Cut: "<<lazy.Data()<<endl;


	    t->Draw("M>>h", ptCut&&meson&&cut);
	    cM[i] = h->Integral();
	    cout<<"  meson: "<<cM[i]<<endl;
	    lazy.Form("meson_dca_%.3f", dcaCut);
	    htmp = (TH1F *)h->Clone(lazy.Data());
	    htmp->Write();

	    t->Draw("M>>h", ptCut&&charm&&cut);
	    cC[i] = h->Integral();
	    cout<<"  charm: "<<cC[i]<<endl;
	    lazy.Form("charm_dca_%.3f", dcaCut);
	    htmp = (TH1F *)h->Clone(lazy.Data());
	    htmp->Write();

	    t->Draw("M>>h", ptCut&&kineE&&cut);
	    cE[i] = h->Integral();
	    cout<<"  kineE: "<<cE[i]<<endl;
	    lazy.Form("kineE_dca_%.3f", dcaCut);
	    htmp = (TH1F *)h->Clone(lazy.Data());
	    htmp->Write();
	} // cut step
	for(int i=0; i<nstep; i++) {
	    Ratio_Err(cM[i], cM[nstep-1], &RM[i], &REM[i]);
	    Ratio_Err(cC[i], cC[nstep-1], &RC[i], &REC[i]);
	    Ratio_Err(cE[i], cE[nstep-1], &RE[i], &REE[i]);
	    RRM[i] = RC[i]>1e-8? RM[i]/RC[i] : 0;
	    RRE[i] = RC[i]>1e-8? RE[i]/RC[i] : 0;
	}

	TGraphErrors *gM  = new TGraphErrors(nstep, x, RM, 0, REM);
	TGraphErrors *gC  = new TGraphErrors(nstep, x, RC, 0, REC);
	TGraphErrors *gE  = new TGraphErrors(nstep, x, RE, 0, REE);
	TGraphErrors *gRM = new TGraphErrors(nstep, x, RRM, 0, 0);
	TGraphErrors *gRE = new TGraphErrors(nstep, x, RRE, 0, 0);

	lazy.Form("Ratio_Meson_Pt_%.3f", pt[ipt]);
	gM->SetName(lazy.Data());
	lazy.Form("Ratio_Charm_Pt_%.3f", pt[ipt]);
	gC->SetName(lazy.Data());
	lazy.Form("Ratio_KineE_Pt_%.3f", pt[ipt]);
	gE->SetName(lazy.Data());

	gM->Write();
	gC->Write();
	gE->Write();

	c->cd();
	c->SetLogy(0);
	TH1F *hF = setFrameHist("hF","dcaCut","Ratio", 0, 0.022, 0, 1.5);
	hF->Draw();
	setGraphStyle(gM, 20, 1, 1, 1, 1, 2);
	setGraphStyle(gC, 20, 2, 1, 1, 2, 2);
	setGraphStyle(gE, 20, 4, 1, 1, 4, 2);
	gM->Draw("eplsame");
	gC->Draw("eplsame");
	gE->Draw("eplsame");
	lazy.Form("outfig/dcaTune_pt_%.3f.pdf", pt[ipt]);
	c->SaveAs(lazy.Data());
	c->cd();
	c->SetLogy();
	TH1F *hF = setFrameHist("hF","dcaCut","S/B", 0, 0.022, 1e-3, 10);
	hF->Draw();
	setGraphStyle(gRM, 20, 1, 1, 1, 1, 2);
	setGraphStyle(gRE, 20, 2, 1, 1, 2, 2);
	gRM->Draw("eplsame");
	gRE->Draw("eplsame");
	lazy.Form("outfig/sbratio_pt_%.3f.pdf", pt[ipt]);
	c->SaveAs(lazy.Data());

	delete hDcaV0VsM;
	delete hDcaAVsM;
	delete hDecayLVsM;
	delete h;

	fout->cd();
    } // ipt
}

