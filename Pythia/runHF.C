void runHF(int irun=1, int Nevt = 20000, float energy = 200){
    gSystem->Load("RunHF/runhfevent_C.so");
    int iseed = sqrt(irun*10000);
    //int Nevt =5e6;
    cout<<Nevt<<" "<<iseed<<" "<<energy<<endl;
    runhfevent(irun, Nevt, iseed, energy,"");
}
