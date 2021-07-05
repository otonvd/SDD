//#include "SDD.h"



//void run(char *filein ="sum_wrong_degr_empty_20210530_2240_20210607_1005.root",char *filecalib="foutSDD_20210601_0113_0601_0930_20kV_100uA_xray_beams.root"){ // EMPTY target WRONG degrader
//void run(char *filein ="sum_degr_rot1_empty_20210607_2149_20210609_1146.root",char *filecalib="foutSDD_20210601_0113_0601_0930_20kV_100uA_xray_beams.root"){ // EMPTY target REVERSE degrader
//void run(char *filein ="sum_nodegr_empty_20210609_2037_20210615_0856.root",char *filecalib="foutSDD_20210601_0113_0601_0930_20kV_100uA_xray_beams.root"){ // EMPTY target NO degrader
//void run(char *filein ="sum_degr2_empty_20210615_1831_20210621_0758.root",char *filecalib="foutSDD_20210601_0113_0601_0930_20kV_100uA_xray_beams.root"){ // EMPTY target DEG2 degrader

//void run(char *filein ="he1_degnumber2.root",char *filecalib="foutSDD_20210624_1622_0624_1806_xray_20kv_100ua.root"){ //he first part, deg#2
void run(char *filein ="he2_degrotnumber1.root",char *filecalib="foutSDD_20210624_1622_0624_1806_xray_20kv_100ua.root"){ //he second part, degrot#1, current

	








	
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

