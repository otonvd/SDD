//#include "SDD.h"

//Siddhartino best degrader higher yield (WhichData=0)
//void run(char *filein ="sum9july2021.root",char *filecalib="foutSDD_20210616_1229_0616_1657_xray_20kv_100ua_beams.root",int fWhichData = 0, float fLuminositypb = 4.577){ 


//S2 run He (WhichData =1)    rot#1 475um
//void run(char *filein ="S2He_1.root",char *filecalib="foutSDD_20220414_1137_0414_1229+20220414_1603_0414_1726_xray_25kv_50+70ua_tube1.root",int fWhichData = 1, float fLuminositypb = 1.756){ 
//void run(char *filein ="S2He_2.root",char *filecalib="foutSDD_20220415_1750_0415_1900_xray_25kv_70ua_tube1.root",int fWhichData = 1, float fLuminositypb = 3.96){ 

//S2 run He1 (removed part of the degrader) (WhichData =2)    rot#1 350um
//void run(char *filein ="S2He1_1.root",char *filecalib="foutSDD_20220422_xray_25kv_70ua_tube1_p1_p2_p3.root",int fWhichData = 2, float fLuminositypb = 2.004){ 
//void run(char *filein ="S2He1_2.root",char *filecalib="foutSDD_20220427_1412_0427_1426_xray_25kv_50ua_tube1.root",int fWhichData = 2, float fLuminositypb = 2.316){ 


//S2 run He2 (added degrader) (WhichData =2)    rot#1 600um
//void run(char *filein ="S2He2_1.root",char *filecalib="foutSDD_20220429_1700_0429_1803_25kv_50ua_xray_tube1_p1_p2.root",int fWhichData = 2, float fLuminositypb = 3.178){ 
//void run(char *filein ="S2He2_2.root",char *filecalib="foutSDD_20220502_1154_0502_1257_xray_25kv_50ua_tube1.root",int fWhichData = 2, float fLuminositypb = 2.375){

//S2 run He3 (back to 475um degrader) (WhichData =2)    rot#1 475um
//void run(char *filein ="S2He3_1.root",char *filecalib="foutSDD_20220502_1154_0502_1257_xray_25kv_50ua_tube1.root",int fWhichData = 2, float fLuminositypb = 2.888){
//void run(char *filein ="S2He3_2.root",char *filecalib="foutSDD_20220506_1345_0506_1430_xray_25kV_50mA_tube1.root",int fWhichData = 2, float fLuminositypb = 3.615){

//S2 run He4 (back to 475um degrader, AFTER CHANGES IN TRIGGER) (WhichData =3)    rot#1 475um
void run(char *filein ="S2He4_1.root",char *filecalib="foutSDD_20220510_1737_0510_1814_xray_25kV_50mA_tube1.root",int fWhichData = 3, float fLuminositypb = 1.599){


	
//gSystem->AddIncludePath("-I. -I$ROOTSYS/include -I$ALICE_ROOT -I$ALICE_ROOT/include -I$ALICE_PHYSICS/include");
//gSystem->Load("SDD_C.so");
R__LOAD_LIBRARY(SDD_C);

//gSystem->AddLinkedLibs("-L/home/oton/ALICE/sw/ubuntu2004_x86-64/ROOT/v6-20-08-alice1-1/lib -l*");

//SDD* ttt = new SDD();
 SDD* ttt = new SDD(filein,filecalib,fWhichData,fLuminositypb);

//ttt->SetCalib(filecalib);

//  ttt->Loop();
 // Int_t InjectionFlag = 2; // ANALYZE ALL EVENTS
 Int_t InjectionFlag = 0; // reject injection
 //Int_t InjectionFlag = 1; // analyze only injection events
 ttt->Loop(InjectionFlag);


}

