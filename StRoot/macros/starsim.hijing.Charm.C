class St_geant_Maker;
St_geant_Maker *geant_maker = 0;

class StarGenEvent;
StarGenEvent   *event       = 0;

class StarPrimaryMaker;
StarPrimaryMaker *_primary = 0;

class StarKinematics;
StarKinematics *kinematics = 0;

class StarGenEventReader;
StarGenEventReader *eventreader = 0;

class StarFilterMaker;
class StarParticleFilter;

class StarPythia6;

TF1 *ptDist  = 0;
TF1 *yDist = 0;

//Initialize the settings:
Float_t vx     = 0.;
Float_t vy     = 0.;
Float_t vz     = 0.;
Float_t vx_sig = 0.01;
Float_t vy_sig = 0.01;
Float_t vz_sig = 3.8;
//Float_t minVz  = -5.0;
//Float_t maxVz  = +5.0;
Float_t minPt  = +0.1;
Float_t maxPt  = +5.0;
Float_t minY   = -1.0;
Float_t maxY   = +1.0;

// charmed meson id
const int nc = 9;
const int charmPid[nc] = {411,421,413,423,431,4122,441,443,445}; // D+, D0, D*,Ds+,Ds,L_c,eta_c,J/psi,chi_2c

// ----------------------------------------------------------------------------
void geometry( TString tag, Bool_t agml=true )
{
    TString cmd = "DETP GEOM "; cmd += tag;
    if ( !geant_maker ) geant_maker = (St_geant_Maker *)chain->GetMaker("geant");
    geant_maker -> LoadGeometry(cmd);
    //  if ( agml ) command("gexec $STAR_LIB/libxgeometry.so");
}
// ----------------------------------------------------------------------------
void command( TString cmd )
{
    if ( !geant_maker ) geant_maker = (St_geant_Maker *)chain->GetMaker("geant");
    geant_maker -> Do( cmd );
}
// ----------------------------------------------------------------------------
void vertex()
{
    //z sampling 
    double vz = -999.0;
    while( std::fabs(vz) > 6.0 )
	vz = StarRandom::Instance().gauss(vz_sig);

    double vx = StarRandom::Instance().gauss(vx_sig);
    double vy = StarRandom::Instance().gauss(vy_sig);
    _primary->SetVertex(vx,vy,vz);
    _primary->SetSigma ( 0, 0, 0);
};
// ----------------------------------------------------------------------------
void trig( Int_t n=0, Int_t mode )
{
    for ( Int_t i=1; i<n+1; i++ ) {
	chain->Clear();
	vertex();
	chain->Make();
	_primary->event()->Print();
	//    command("gprint kine");
    }
}
// ----------------------------------------------------------------------------
void myKine()
{ 
    gSystem->Load("StarKinematics.so");
    kinematics = new StarKinematics("D0");  
    _primary -> AddGenerator(kinematics);
}

// ----------------------------------------------------------------------------
void myEventReader(char *infilename) 
{
    gSystem->Load("StarGenEventReader.so");
    eventreader = new StarGenEventReader();
    eventreader->SetInputFile(infilename,"genevents","primaryEvent");
    _primary->AddGenerator(eventreader);
}


// ----------------------------------------------------------------------------
void myPythia8() 
{
    gSystem->Load("Pythia8_1_62.so");
    StarPythia8 *pythia8 = new StarPythia8();
    pythia8->SetFrame("CMS",200.0);
    pythia8->SetBlue("proton");
    pythia8->SetYell("proton");

    pythia8->Set("HardQCD:all=on");
    pythia8->Set("TimeShower:alphaSvalue=0.18");
    pythia8->Set("PhaseSpace:pTHatMin=1.3");


    // set decay channel
    TString name;
    for(int i=0; i<nc; i++) {
	name.Form("%i:onMode = off", charmPid[i]);
	pythia8->Set(name.Data());
	name.Form("%i:onIfAny = 11 -11", charmPid[i]);
	pythia8->Set(name.Data());

	name.Form("-%i:onMode = off", charmPid[i]);
	pythia8->Set(name.Data());
	name.Form("-%i:onIfAny = 11 -11", charmPid[i]);
	pythia8->Set(name.Data());
    }

    _primary->AddGenerator(pythia8);

}

// ----------------------------------------------------------------------------
void myPythia6()
{
    cout<<"YiSaid : Init Pythia6......."<<endl;
    gSystem->Load( "libPythia6_4_28.so");
    StarPythia6 *mPythia = new StarPythia6("Pythia6");

    //mPythia->SetTitle("Pythia 6.4.23");
    mPythia->SetFrame("CMS",200.0);
    mPythia->SetBlue("proton");
    mPythia->SetYell("proton");

    PySubs_t &pysubs = mPythia->pysubs();
    pysubs.msel = 1;

    PyPars_t &pypars = mPythia->pypars();
    pypars.parp(91) = 1.0; // <kT>
    pypars.parp(67) = 1.0; // mstp*4

    PyDat3_t &pydat3 = mPythia->pydat3();
    // close decay channels other than simi-leptonic decays
    for(int i=684; i<=735; i++)   pydat3.mdme(i   , 1) = 0; //D+
    for(int i=755; i<=807; i++)   pydat3.mdme(i   , 1) = 0; //D0
    pydat3.mdme(818 , 1) = 0; //Ds
    for(int i=824; i<=850; i++)   pydat3.mdme(i   , 1) = 0; //Ds
    for(int i=857; i<=862; i++)   pydat3.mdme(i   , 1) = 0; //eta_c , J/psi , chi_2c
    for(int i=1097; i<=1165; i++) pydat3.mdme(i   , 1) = 0; //Lc

    _primary -> AddGenerator(mPythia);
    cout<<"YiSaid : Init Pythia6 done !!"<<endl;
}


// ----------------------------------------------------------------------------
void Hijing()
{
    cout<<"YiSaid : Init Hijing......."<<endl;
    gSystem->Load( "libHijing1_383.so");
    StarHijing *hijing = new StarHijing("hijing");
    hijing->SetTitle("Hijing 1.383");

    // Setup collision frame, energy and beam species
    hijing->SetFrame("CMS",200.0);
    hijing->SetBlue("Au");
    hijing->SetYell("Au");  
    //hijing->SetImpact(0.0, 30.0);       // Impact parameter min/max (fm)    0.   30.
    //hijing->SetImpact(0.0, 4.5);       // 0-10% central
    hijing->SetImpact(0.0, 4.5);       // 0-80% central
    hijing->hiparnt().ihpr2(4) = 0;     // Jet quenching (1=yes/0=no)       0
    hijing->hiparnt().ihpr2(3) = 0;     // Hard scattering (1=yes/0=no)
    hijing->hiparnt().hipr1(10) = 2.0;  //    pT jet
    hijing->hiparnt().ihpr2(8)  = 10;   // Max number of jets / nucleon
    hijing->hiparnt().ihpr2(11) = 1;    // Set baryon production
    hijing->hiparnt().ihpr2(12) = 1;    // Turn on/off decay of particles [1=recommended]
    hijing->hiparnt().ihpr2(18) = 1;    // Turn on/off B production
    hijing->hiparnt().hipr1(7) = 5.35;  // Set B production ???? Not really used... Really ????

    // For more configuration options, see the HIJING manual
    // http://ntc0.lbl.gov/~xnwang/hijing/doc.html

    _primary -> AddGenerator(hijing);
    _primary -> SetCuts( 1.0E-6 , -1., -2.6, 2.6 );

    cout<<"YiSaid : Init Hijing done !!"<<endl;

}
// ----------------------------------------------------------------------------
void myFilter(int saveMode) {
    gSystem->Load( "StarGeneratorFilt.so" );
    StarParticleFilter *filter = new StarParticleFilter();
    for(int i=0;i<nc;i++) {
	filter->AddTrigger(charmPid[i], 0, -1, -6, 6 );
	filter->AddTrigger(-1*charmPid[i], 0, -1, -6, 6 );
    }
    _primary -> AddFilter( filter );

    // If set to 1, tracks will be saved in the tree on events which were
    // rejected.  If the tree size is too big (because the filter is too
    // powerful) you may want to set this equal to zero.  In which case
    // only header information is saved for the event.
    _primary->SetAttr("FilterKeepAll",     int(saveMode));
    _primary->SetAttr("FilterKeepHeader",  int(saveMode));

    // By default, the primary maker enters an infinite loop and executes
    // the event generator until it yields an event which passes the filter.
    // The big full chain treats this as a single event.
    //
    // If you want the BFC to see an empty event, set the FilterSkipRejects
    // attribute on the primary maker and give it the priveledge it needs
    // to kill the event. 
    //_primary->SetAttr("FilterSkipRejects", int(1) ); // enables event skipping 
    //chain->SetAttr(".Privilege",1,"StarPrimaryMaker::*" );
//
}


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void starsim( Int_t nevents=1, Int_t Index = 0, Int_t rngSeed=4321 , Int_t mode = 0 , char* inPythia6File = "")
{ 

    gROOT->ProcessLine(".L bfc.C");
    {
	TString simple = "y2014a geant gstar usexgeom agml ";
	bfc(0, simple );
    }

    gSystem->Load( "libVMC.so");

    gSystem->Load( "StarGeneratorUtil.so" );
    gSystem->Load( "StarGeneratorEvent.so" );
    gSystem->Load( "StarGeneratorBase.so" );
    gSystem->Load( "libMathMore.so"   );  
    gSystem->Load( "xgeometry.so"     );

    //gMessMgr->SetLevel(999);

    // force gstar load/call
    gSystem->Load( "gstar.so" );
    command("call gstar");

    // Setup RNG seed and map all ROOT TRandom here
    StarRandom::seed( rngSeed );
    StarRandom::capture();

    char rootname[100],fzname[100];
    switch(mode) {
	case 0:
	    sprintf(rootname,"pythia6_charm_%d.starsim.root",Index);
	    sprintf(fzname,"gfile o pythia6_charm_%d.starsim.fzd",Index);
	    break;
	default:
	    sprintf(rootname,"hijing_charm_%d.starsim.root",Index);
	    sprintf(fzname,"gfile o hijing_charm_%d.starsim.fzd",Index);
	    break;
    }
    //
    // Create the primary event generator and insert it
    // before the geant maker
    //
    _primary = new StarPrimaryMaker();
    {
	_primary -> SetFileName(rootname);
	chain -> AddBefore( "geant", _primary );
    }

    //
    // These should be adjusted to your best vertex estimates
    //
    //_primary -> SetVertex( vx,vy,vz );
    //_primary -> SetSigma( vx_sig,vy_sig,vz_sig );




    switch(mode) {
	// Setup pythia 
	case 0 :
	    cout<<"Running at Pythia6 Standalone mode"<<endl; 
	    myPythia6();
	    myFilter(0);
	    break; 
	    // Setup an event generator
	case 1 : 
	    cout<<"Running at Hijing+StarGenEventReader mode"<<endl;
	    TString infilename;
	    infilename.Form(inPythia6File);
	    Hijing();
	    myEventReader(infilename);
	    //myFilter(0);
	    break;
	case 2 :
	    cout<<"Running at Hijing+Pythia8 mode"<<endl;
	    Hijing();
	    myPythia8();
	    break;
	case 3 :
	    myPythia8();
	    myFilter(0);
	    break;
	default :
	    cout<<"You must choose a mode!"<<endl;
	    cout<<"0 for Pythia6" << endl;
	    cout<<"1 for Hijing+Pythia6 from a root file"<<endl;
	    cout<<"2 for Hijing+Pythia8"<<endl;
	    cout<<"3 for Pythia8"<<endl;
	    break;
    }


    //
    // Initialize primary event generator and all sub makers
    //
    _primary -> Init();

    //
    // Setup geometry and set starsim to use agusread for input
    //
    //geometry("y2014");
    command("gkine -4 0");
    if(mode !=0 && mode !=3) command(fzname);

    //Double_t pt0 = 3.0;
    //ptDist = new TF1("ptDist","(x/[0])/(1+(x/[0])^2)^6",0.0,10.0);
    //ptDist->SetParameter(0, pt0);
    //ptDist->Draw();
    //Exponential
    //ptDist = new TF1("ptDist","[0]*x*TMath::Exp(-x/[1])",minPt,maxPt); //dN/pT/dpT is exp 
    //ptDist->SetParameters(1.,1.);//slope = 1.;
    //Low pT optimized
    //ptDist = new TF1("ptDist","[0]+[1]*x+[2]*x^2",minPt,maxPt); //dN/pT/dpT is exp 
    //ptDist->SetParameters(0.975923,-0.00841225,-0.0185871);
    //yDist = new TF1("yDist","-TMath::Erf(x+2.6)*TMath::Erf(x-2.6)",minY,maxY);
    //yDist = new TF1("yDist","pol0",minY,maxY);
    //yDist->SetParameter(0,1.);

    //phi, default 0 ~ TMath::TwoPi() flat

    //
    // Trigger on nevents
    //
    trig( nevents, mode );


    //  command("gprint kine");

    command("call agexit");  // Make sure that STARSIM exits properly
}
// ----------------------------------------------------------------------------

