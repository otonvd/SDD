//#include "SDD.h"

//best siddhartino degrader (WhichData=0)
//void runCalib(char *filein ="20210616_1229_0616_1657_xray_20kv_100ua_beams.root", int quick = 0, int fWhichData = 0)

//S2 run He (WhichData=1)
//void runCalib(char *filein ="20220414_1137_0414_1229+20220414_1603_0414_1726_xray_25kv_50+70ua_tube1.root", int quick = 0, int fWhichData = 1)
//void runCalib(char *filein ="20220415_1750_0415_1900_xray_25kv_70ua_tube1.root", int quick = 0, int fWhichData = 1)
//void runCalib(char *filein ="20220419_xray_25kv_70ua_tube1_p1_p2_p3.root", int quick = 0, int fWhichData = 1)
//void runCalib(char *filein ="20220422_xray_25kv_70ua_tube1_p1_p2_p3.root", int quick = 0, int fWhichData = 1)
//.... something missing....
void runCalib(char *filein ="20220510_1737_0510_1814_xray_25kV_50mA_tube1.root", int quick = 0, int fWhichData = 3)



{ 
//cout<<gSystem->GetIncludePath()<<endl;
//gSystem->AddIncludePath("-I. -I$ROOTSYS/include -I$ALICE_ROOT -I$ALICE_ROOT/include -I$ALICE_PHYSICS/include");

//gSystem->Load("SDD_C.so");
R__LOAD_LIBRARY(SDD_C);


//gSystem->AddLinkedLibs("-L/home/oton/ALICE/sw/ubuntu2004_x86-64/ROOT/v6-20-08-alice1-1/lib -l*");

//SDD* ttt = new SDD();
SDD* ttt = new SDD(filein,"",fWhichData);

if(quick) ttt->SetQuick();
//ttt->SetInputOutputFile("cosa");

  ttt->Loop();
}

