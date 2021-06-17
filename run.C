//#include "SDD.h"

void run(char *filein ="20210530_2240_0607_0821_summ_all_wrong_degr.root",char *filecalib="foutSDD_20210601_0113_0601_0930_20kV_100uA_xray_beams.root")
{ 
//gSystem->AddIncludePath("-I. -I$ROOTSYS/include -I$ALICE_ROOT -I$ALICE_ROOT/include -I$ALICE_PHYSICS/include");


//gSystem->Load("SDD_C.so");
R__LOAD_LIBRARY(SDD_C);

//gSystem->AddLinkedLibs("-L/home/oton/ALICE/sw/ubuntu2004_x86-64/ROOT/v6-20-08-alice1-1/lib -l*");

//SDD* ttt = new SDD();
 SDD* ttt = new SDD(filein,filecalib);

//ttt->SetCalib(filecalib);

//  ttt->Loop();
 // Int_t InjectionFlag = 2; // ANALYZE ALL EVENTS
 Int_t InjectionFlag = 0; // reject injection
 //Int_t InjectionFlag = 1; // analyze only injection events
 ttt->Loop(InjectionFlag);


}

