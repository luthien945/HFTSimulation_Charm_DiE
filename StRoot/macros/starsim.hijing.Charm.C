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

class StarDecayManager;
class StarPythia8Decayer;
class StarFilterMaker;
class StarParticleFilter;

class StarPythia6;

TF1 *ptDist_flat10  = 0;
TF1 *ptDist_flat20  = 0;
TF1 *ptDist_flat30  = 0;
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
const int nc = 4;
const int charmPid[nc] = {411,421,431,4122}; // D+, D0,Ds,L_c

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
void trig( Int_t n=0, Int_t mode)
{
    for ( Int_t i=1; i<n+1; i++ ) {
	chain->Clear();
	vertex();
	//if(doFlag) kinematics->Dist(100, "pi0_Dalitz", ptDist_flat30, yDist);
	//if(doFlag) kinematics->Dist(100, "eta_Dalitz", ptDist_flat30, yDist);
	chain->Make();
	_primary->event()->Print();
	if(i==1) command("gprint kine");
    }
}
// ----------------------------------------------------------------------------
void myKine()
{ 
    gSystem->Load("StarKinematics.so");
    kinematics = new StarKinematics();  
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
    cout<<"YiSaid : Init Pythia8......."<<endl;
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
	name.Form("-%i:onMode = off", charmPid[i]);
	pythia8->Set(name.Data());

	if(!decayOutSidePythia) {
	    name.Form("%i:onIfAny = 11 -11", charmPid[i]);
	    pythia8->Set(name.Data());
	    name.Form("-%i:onIfAny = 11 -11", charmPid[i]);
	    pythia8->Set(name.Data());
	}
    }

    _primary->AddGenerator(pythia8);
    cout<<"YiSaid : Init Pythia8...done"<<endl;

}

// ----------------------------------------------------------------------------
void myPythia6(bool mode = kFALSE)
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

    if(mode) {
	// use an outside decayer
	// turn off the decays in pythia
	pydat3.mdcy(122, 1) = 0; // D0 (411)
	pydat3.mdcy(125, 1) = 0; // D+ (421)
	pydat3.mdcy(128, 1) = 0; // Ds (431)
	pydat3.mdcy(181, 1) = 0; // Lc (4122)
    }
    else {
	// close decay channels other than simi-leptonic decays
	for(int i=684; i<=735; i++)   pydat3.mdme(i   , 1) = 0; //D+
	for(int i=755; i<=807; i++)   pydat3.mdme(i   , 1) = 0; //D0
	pydat3.mdme(818 , 1) = 0; //Ds
	for(int i=824; i<=850; i++)   pydat3.mdme(i   , 1) = 0; //Ds
	for(int i=857; i<=862; i++)   pydat3.mdme(i   , 1) = 0; //eta_c , J/psi , chi_2c
	for(int i=1097; i<=1165; i++) pydat3.mdme(i   , 1) = 0; //Lc
    }

    _primary -> AddGenerator(mPythia);
    cout<<"YiSaid : Init Pythia6 done !!"<<endl;
}


// ----------------------------------------------------------------------------
void Hijing(bool decayMode = kFALSE)
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
    hijing->SetImpact(0.0, 20.);       // 0-80% central
    
    hijing->hiparnt().ihpr2(4) = 0;     // Jet quenching (1=yes/0=no)       0
    hijing->hiparnt().ihpr2(3) = 0;     // Hard scattering (1=yes/0=no)
    hijing->hiparnt().hipr1(10) = 2.0;  //    pT jet
    hijing->hiparnt().ihpr2(8)  = 10;   // Max number of jets / nucleon
    hijing->hiparnt().ihpr2(11) = 1;    // Set baryon production
    hijing->hiparnt().ihpr2(12) = 0;    // Turn on/off decay of particles [1=recommended]
    hijing->hiparnt().ihpr2(18) = 1;    // Turn on/off B production
    hijing->hiparnt().hipr1(7) = 5.35;  // Set B production ???? Not really used... Really ????

    // For more configuration options, see the HIJING manual
    // http://ntc0.lbl.gov/~xnwang/hijing/doc.html

    // if use outside decayer, switch off charm meson decays
    if(decayMode) {
	cout<<"YiSaid : turn off the decay of charm mesons"<<endl;
	hijing->ludat3().mdcy(122, 1) = 0; // D0 (411)
	hijing->ludat3().mdcy(125, 1) = 0; // D+ (421)
	hijing->ludat3().mdcy(128, 1) = 0; // Ds (431)
	hijing->ludat3().mdcy(181, 1) = 0; // Lc (4122)
    }

    _primary -> AddGenerator(hijing);
    _primary -> SetCuts( 1.0E-6 , -1., -2.6, 2.6 );

    cout<<"YiSaid : Init Hijing done !!"<<endl;

}

// ----------------------------------------------------------------------------
void myFilter(int saveMode) {
    cout<<"YiSaid : Init Filter....."<<endl;
    gSystem->Load( "StarGeneratorFilt.so" );
    StarParticleFilter *filter = new StarParticleFilter();

    for(int i=0;i<nc;i++) {
	filter->AddTrigger(charmPid[i]    , 1.E-6 , -1. , -4. , 4.);
	filter->AddTrigger(-1*charmPid[i] , 1.E-6 , -1. , -4. , 4.);
	cout<<"YiSaid : add filter for particle "<<charmPid[i]<<endl;
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
    cout<<"YiSaid : Init Filter done!!"<<endl;
}

// ----------------------------------------------------------------------------
void myDecayer() {
    cout<<"YiSaid : Init Decayer....."<<endl;
    //
    // Setup decay manager
    //
    gSystem->Load( "StarGeneratorDecay.so" );
    gSystem->Load( "libPythia8_1_62.so");
    StarDecayManager   *decayMgr = AgUDecay::Manager();
    StarPythia8Decayer *decayPy8 = new StarPythia8Decayer();
    decayMgr->AddDecayer( 0, decayPy8 ); // Handle any decay requested 
    decayPy8->SetDebug(1);
    decayPy8->Set("WeakSingleBoson:all = on");
    //decayPy8->Set("HardQCD:all=on");
    //decayPy8->Set("TimeShower:alphaSvalue=0.18");
    //decayPy8->Set("PhaseSpace:pTHatMin=1.3");

    // set decay channel for charmed meson
    TString name;
    for(int i=0; i<nc; i++) {
	name.Form("%i:onMode = off", charmPid[i]);
	decayPy8->Set(name.Data());
	name.Form("-%i:onMode = off", charmPid[i]);
	decayPy8->Set(name.Data());

	name.Form("%i:onIfAny = 11 -11", charmPid[i]);
	decayPy8->Set(name.Data());
	name.Form("-%i:onIfAny = 11 -11", charmPid[i]);
	decayPy8->Set(name.Data());
    }
    cout<<"YiSaid : Init Decayer done"<<endl;
}
// ----------------------------------------------------------------------------
void myParticle() {
}


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void starsim( Int_t nevents=1, Int_t Index = 0, Int_t rngSeed=4321 , Int_t mode = 0 , char* inPythia6File = "")
{ 
    bool decayOutSidePythia = kTRUE;
    bool doDalitz = kFALSE;

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
	    sprintf(fzname,"gfile o hijing_charm_%d.starsim.fzd",Index);
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
	    cout<<"YiSaid : Running at Pythia6 Standalone mode"<<endl; 
	    myPythia6(decayOutSidePythia);
	    myFilter(0);
	    break; 
	    // Setup an event generator
	case 1 : 
	    cout<<"YiSaid : Running at Hijing+StarGenEventReader mode"<<endl;
	    TString infilename;
	    infilename.Form(inPythia6File);
	    myEventReader(infilename);
	    Hijing(decayOutSidePythia);
	    break;
	case 2 :
	    cout<<"YiSaid : Running at Hijing+Pythia8 mode"<<endl;
	    Hijing(0);
	    myPythia8();
	    break;
	case 3 :
	    cout<<"YiSaid : Running at Pythia8+StarGenEventReader mode"<<endl;
	    myPythia8();
	    TString infilename;
	    infilename.Form(inPythia6File);
	    myEventReader(infilename);
	    break;
	case 4 :
	    cout<<"YiSaid : Running at Hijing Standalone mode"<<endl; 
	    Hijing();
	    break; 
	case 5 : 
	    cout<<"YiSaid : Running at Pythia6+StarGenEventReader mode"<<endl;
	    TString infilename;
	    infilename.Form(inPythia6File);
	    myPythia6(decayOutSidePythia);
	    myFilter(0);
	    myEventReader(infilename);
	    break;
	default :
	    cout<<"YiSaid : You must choose a mode!"<<endl;
	    cout<<"0 for Pythia6" << endl;
	    cout<<"1 for Hijing+Pythia6 from a root file"<<endl;
	    cout<<"2 for Hijing+Pythia8"<<endl;
	    cout<<"3 for test"<<endl;
	    break;
    }


    if(decayOutSidePythia && mode != 0 && mode != 4){
    //if(decayOutSidePythia){
	myDecayer();
	// Particle data
	StarParticleData& data = StarParticleData::instance();
	//  One can add a particle to G3 using...
	data.AddParticleToG3( "MyD+"    , 0.1870E+01 , 1.04000E-12 , 1  , 3 , 411   , 12037 , 0 , 0 );
	data.AddParticleToG3( "MyD-"    , 0.1870E+01 , 1.04000E-12 , -1 , 3 , -411  , 12038 , 0 , 0 );
	data.AddParticleToG3( "MyD0"    , 0.1865E+01 , 0.42800E-12 , 0. , 3 , 421   , 12039 , 0 , 0 );
	data.AddParticleToG3( "MyD0bar" , 0.1865E+01 , 0.42800E-12 , 0. , 3 , -421  , 12040 , 0 , 0 );
	data.AddParticleToG3( "MyDs+"   , 0.1968E+01 , 0.50000E-12 , 1  , 3 , 431   , 12041 , 0 , 0 );
	data.AddParticleToG3( "MyDs-"   , 0.1968E+01 , 0.50000E-12 , -1 , 3 , -431  , 12042 , 0 , 0 );
	data.AddParticleToG3( "MyLc+"   , 0.2286E+01 , 0.20000E-12 , 1  , 3 , 4122  , 12043 , 0 , 0 );
	//data.AddParticleToG3( "MyLc-"   , 0.2286E+01 , 0.20000E-12 , -1 , 3 , -4122 , 12044 , 0 , 0 );
    }


    //
    // Initialize primary event generator and all sub makers
    //
    //_primary -> SetCuts( 1.0E-6 , -1., -2.6, 2.6 );
    _primary -> Init();



    //
    // Setup geometry and set starsim to use agusread for input
    //
    //geometry("y2014");
    command("gkine -4 0");
    if(mode !=0 && mode !=3) command(fzname);
    //command(fzname);

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


    ptDist_flat10 = new TF1("ptDist_flat","pol0",0,10);
    ptDist_flat10->SetParameter(0,1.);
    ptDist_flat20 = new TF1("ptDist_flat","pol0",0,20);
    ptDist_flat20->SetParameter(0,1.);
    ptDist_flat30 = new TF1("ptDist_flat","pol0",0,30);
    ptDist_flat30->SetParameter(0,1.);

    //yDist = new TF1("yDist","-TMath::Erf(x+2.6)*TMath::Erf(x-2.6)",minY,maxY);
    yDist = new TF1("yDist","pol0",minY,maxY);
    yDist->SetParameter(0,1.);

    //phi, default 0 ~ TMath::TwoPi() flat

    //
    // Trigger on nevents
    //
    trig( nevents, mode );


    //  command("gprint kine");

    command("call agexit");  // Make sure that STARSIM exits properly
}
// ----------------------------------------------------------------------------

