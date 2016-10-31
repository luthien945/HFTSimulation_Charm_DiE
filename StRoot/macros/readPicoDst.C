/* **************************************************
 *  A macro to run StPicoMcNpeAnaMaker
 *
 *  Authors:  **Kunsu OH (kunsu OH)
 *
 *  **Code Maintainer
 *
 * **************************************************
 */

#include <TSystem>
#include "TString.h"
#include <fstream>

class StMaker;
class StChain;
class StPicoDstMaker;

StChain * mChain;

void readPicoDst(const TString inFile="test.list", const TString outFileName="test.root")
{
    
    //string SL_version = "SL15k";
    //string env_SL = getenv("STAR");
    //if (env_SL.find(SL_version) == string::npos)
    //{
    //    cout << "Environment Star Library does not match the requested library in runPicoNpeEventMaker.C. Exiting..." << endl;
    //    exit(1);
    //}

    TString inputFile;
    if(inFile.Contains(".root")) {
	inputFile = inFile;
    }else {
	ifstream ifile;
	ifile.open(inFile.Data());
	if(ifile.good()) {
	    char line[256];
	    ifile.getline(line, 256);
	    TString myline = line;
	    if(myline.Contains(".list") || myline.Contains(".lis")) inputFile = myline;
	    else inputFile = inFile;
	}
	else prinf("Can't open list file %s\n", inFile.Data());
    }

    printf("InputFile : %s\n", inFile.Data());
    
    gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
    loadSharedLibraries();
    
//    gSystem->Load("StBTofUtil");
    gSystem->Load("StPicoDstMaker");
    gSystem->Load("StMyAnalysisMaker");

    mChain = new StChain();
    
    StPicoDstMaker* picoDstMaker = new StPicoDstMaker(0,inputFile.Data(),"picoDst");
    StMyAnalysisMaker* mAnaMaker = new StMyAnalysisMaker("myAnaMaker", picoDstMaker, outFileName.Data());
    
    // -------------- USER variables -------------------------
    
    // add your cuts here.
    mChain->Init();
    int nEntries = picoDstMaker->chain()->GetEntries();
    cout << " Total entries = " << nEntries << endl;
    
    for (int iEvent = 0; iEvent < nEntries; ++iEvent)
    {
	if(iEvent%1==0) cout << "Working on eventNumber " << iEvent << endl;
	else cout << "Working on eventNumber " << iEvent << endl;
	mChain->Clear();
	int iret = mChain->Make();
	if (iret)
	{
	    cout << "Bad return code!" << iret << endl;
	    break;
	}
    }


    mChain->Finish();
    delete mChain;
}
