void runHF(int irun=1, int Nevt = 20, float energy = 200, bool doRandom=kTRUE){
    gSystem->Load("RunHF/runhfevent_C.so");
    int iseed = sqrt(irun*10000);
    //int Nevt =5e6;
    cout<<Nevt<<" "<<iseed<<" "<<energy<<endl;
    runhfevent(irun, Nevt, iseed, energy, doRandom);
}
