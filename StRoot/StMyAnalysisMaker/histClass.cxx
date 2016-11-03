#include "histClass.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TList.h"
#include "TFile.h"

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

int histClass::initHist() {
    mList = new TList();
}
