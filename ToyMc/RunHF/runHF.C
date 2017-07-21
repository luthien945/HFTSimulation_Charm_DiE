void runHF(int irun=1, float energy = 200.,int Nevt=5e6){
    gSystem->Load("runhfevent_C.so");
    int iseed = sqrt(irun*10000);
    cout<<Nevt<<" "<<iseed<<" "<<energy<<endl;
    runhfevent(irun, Nevt, iseed, energy,"");
}
