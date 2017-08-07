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

TRandom3 *mRandom;

TF1 *ptDist_flat10  = 0;
TF1 *ptDist_flat20  = 0;
TF1 *ptDist_flat30  = 0;
TF1 *ptDist_expo    = 0;
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

TH1F *hVx;
TH1F *hVy;
TH1F *hVz;

// charmed meson id
const int nc = 4;
const int charmPid[nc] = {411,421,431,4122}; // D+, D0,Ds,L_c

const int nh = 4;
const int mesonPid[nh] = {111, 221, 223, 333}; // pi0, eta, omega, phi

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
void vertex(int i, int mode)
{
    //z sampling 
    double vz = -999.;
    double vx = -999.;
    double vy = -999.;
    if(mode ==0) {
	vx = 0;
	vy = 0;
	vz = 0;
    } else {
	while( std::fabs(vz) > 6.0 )
	    vz = StarRandom::Instance().gauss(vz_sig);

	vx = StarRandom::Instance().gauss(vx_sig);
	vy = StarRandom::Instance().gauss(vy_sig);
    }
    cout<<"YiSaid : runing with mode "<<mode<<endl;
    cout<<"YiSaid : set vertex to ["
	<<std::setw(5)<<vx<<" "
	<<std::setw(5)<<vy<<" "
	<<std::setw(5)<<vz<<"]"<<endl;
    _primary->SetVertex(vx,vy,vz);
    _primary->SetSigma ( 0, 0, 0);
}
// ----------------------------------------------------------------------------
void trig( Int_t n=0, Int_t mode)
{
    cout<<"YiSaid : runing with mode "<<mode<<endl;
    for ( Int_t i=1; i<n+1; i++ ) {
	chain->Clear();
	vertex(i, mode);
	chain->Make();
	_primary->event()->Print();
	if(i==1) command("gprint kine");
    }
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
    hijing->hiparnt().ihpr2(12) = 1;    // Turn on/off decay of particles [1=recommended]
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
// ----------------------------------------------------------------------------
void starsim( Int_t nevents=1, Int_t Index = 0, Int_t rngSeed=4321)
{ 
    mRandom = new TRandom3();
    mRandom->SetSeed(0);

    gROOT->ProcessLine(".L bfc.C");
    {
	TString simple = "y2014a geant gstar usexgeom agml ";
	//if(mode == 0) simple.Form("tables nodefault");
	bfc(0, simple );
    }

    gSystem->Load( "libVMC.so");

    gSystem->Load( "StarGeneratorUtil.so" );
    gSystem->Load( "StarGeneratorEvent.so" );
    gSystem->Load( "StarGeneratorBase.so" );
    gSystem->Load( "libMathMore.so"   );  
    gSystem->Load( "xgeometry.so"     );
    gSystem->Load( "libKinematics.so");

    // force gstar load/call
    gSystem->Load( "gstar.so" );
    command("call gstar");

    // Setup RNG seed and map all ROOT TRandom here
    StarRandom::seed( rngSeed );
    StarRandom::capture();

    char rootname[100],fzname[100];
    sprintf(rootname,"hijing_bg_%d.starsim.root",Index);
    sprintf(fzname,"gfile o hijing_bg_%d.starsim.fzd",Index);

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

    cout<<"YiSaid : Running at Hijing"<<endl;
    Hijing(0);
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
    command(fzname);

    trig( nevents, 0 );

    //  command("gprint kine");

    command("call agexit");  // Make sure that STARSIM exits properly

}
// ----------------------------------------------------------------------------

