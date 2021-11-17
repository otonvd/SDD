//#include "SDD.h"

void runCalib(char *filein ="20211111_1438_1111_1938_xray_25kv_20ua_tube1.root", int quick = 0)



{ 
//cout<<gSystem->GetIncludePath()<<endl;
//gSystem->AddIncludePath("-I. -I$ROOTSYS/include -I$ALICE_ROOT -I$ALICE_ROOT/include -I$ALICE_PHYSICS/include");

//gSystem->Load("SDD_C.so");
R__LOAD_LIBRARY(SDD_C);


//gSystem->AddLinkedLibs("-L/home/oton/ALICE/sw/ubuntu2004_x86-64/ROOT/v6-20-08-alice1-1/lib -l*");

//SDD* ttt = new SDD();
SDD* ttt = new SDD(filein,"");

if(quick) ttt->SetQuick();
//ttt->SetInputOutputFile("cosa");

  ttt->Loop();
}

