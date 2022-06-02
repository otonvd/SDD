#define SDD_cxx
#include "SDD.h"
#include <TRandom3.h>
#include <TH2.h>
#include <THStack.h>
#include <TF1.h>
#include <TGraphErrors.h>
#include <TCutG.h>
#include <TFile.h>
#include <TLine.h>
#include <TSpectrum.h>
#include <TLorentzVector.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <deque>
#include <vector>

void SDD::Loop(Int_t InjectionFlag){
//see how you handle this


// --- HISTO BOOKING AND GLoBAL STUFF  --- //
//______________________________________________________________________________________________//

//ADC histo booking
int nbinsadc = 8000;
int minadc = 0.;
int maxadc = 8000.;
 //rebin:
 int rebinFactor = 8;
 nbinsadc = nbinsadc/rebinFactor;
TH1F* hADC[nBUS][nSDD];
TH2F* hADCpre[nBUS][nSDD];
for(int iBUS=0;iBUS<nBUS;iBUS++){
 for(int iSDD=0;iSDD<nSDD;iSDD++){
  hADC[iBUS][iSDD] = new TH1F(Form("ADC_%i_%i",iBUS,iSDD),"",nbinsadc,minadc,maxadc);
  hADC[iBUS][iSDD]->GetXaxis()->SetTitle("ADC");
  hADC[iBUS][iSDD]->SetLineColor(2);
  hADC[iBUS][iSDD]->SetLineWidth(2);
  hADCpre[iBUS][iSDD] 
   = new TH2F(Form("ADCpre_%i_%i",iBUS,iSDD),"",nbinsadc,minadc,maxadc,nbinsadc,minadc,maxadc);
  hADCpre[iBUS][iSDD]->GetXaxis()->SetTitle("ADC");
  hADCpre[iBUS][iSDD]->GetYaxis()->SetTitle("ADC previous hit");
 }
}

TH1F* hADCtrig[nBUS][nSDD];
TH1F* hADCgood[nBUS][nSDD];
TH2F* hADCpreGOOD[nBUS][nSDD];
TH2F* hADCpreBAD[nBUS][nSDD];
for(int iBUS=0;iBUS<nBUS;iBUS++){
 for(int iSDD=0;iSDD<nSDD;iSDD++){
  hADCtrig[iBUS][iSDD]=(TH1F*)hADC[iBUS][iSDD]->Clone(Form("ADCtrig_%i_%i",iBUS,iSDD));
  hADCgood[iBUS][iSDD]=(TH1F*)hADC[iBUS][iSDD]->Clone(Form("ADCgood_%i_%i",iBUS,iSDD));
  hADCgood[iBUS][iSDD]->SetLineColor(4);
  hADCpreGOOD[iBUS][iSDD]=(TH2F*)hADCpre[iBUS][iSDD]->Clone(Form("ADCpreGOOD_%i_%i",iBUS,iSDD));
  hADCpreBAD[iBUS][iSDD]=(TH2F*)hADCpre[iBUS][iSDD]->Clone(Form("ADCpreBAD_%i_%i",iBUS,iSDD));
 }
}

//ADC arrays:
const int MaxArrays = 100;
int nArrADC = 0;
TObjArray* ArrADC[MaxArrays];
ArrADC[nArrADC]=new TObjArray();ArrADC[nArrADC]->SetName("ADC");nArrADC++;    //same order here...
ArrADC[nArrADC]=new TObjArray();ArrADC[nArrADC]->SetName("ADCpre");nArrADC++;
ArrADC[nArrADC]=new TObjArray();ArrADC[nArrADC]->SetName("ADCpreGOOD");nArrADC++;
ArrADC[nArrADC]=new TObjArray();ArrADC[nArrADC]->SetName("ADCpreBAD");nArrADC++;
ArrADC[nArrADC]=new TObjArray();ArrADC[nArrADC]->SetName("ADCtrig");nArrADC++;
ArrADC[nArrADC]=new TObjArray();ArrADC[nArrADC]->SetName("ADCgood");nArrADC++;
for(int iBUS=0;iBUS<nBUS;iBUS++){
 for(int iSDD=0;iSDD<nSDD;iSDD++){
  int iarr =0;
  ArrADC[iarr]->Add(hADC[iBUS][iSDD]);iarr++;                                 //... and here
  ArrADC[iarr]->Add(hADCpre[iBUS][iSDD]);iarr++;
  ArrADC[iarr]->Add(hADCpreGOOD[iBUS][iSDD]);iarr++;
  ArrADC[iarr]->Add(hADCpreBAD[iBUS][iSDD]);iarr++;
  ArrADC[iarr]->Add(hADCtrig[iBUS][iSDD]);iarr++;
  ArrADC[iarr]->Add(hADCgood[iBUS][iSDD]);iarr++;
 }
}


//Kaon trigger plots:
TH2F* hktwide = new TH2F("hktwide","",100,0,8000,100,0,8000);
TH2F* hkt = new TH2F("hkt","",1500,3500,5000,1500,3500,5000);
TH1F* hktrot = new TH1F("hktrot","",1000,5500,6500);
 hktwide->GetXaxis()->SetTitle("TDC1+TDC2");
 hktwide->GetYaxis()->SetTitle("TDC3+TDC4");
 hkt->GetXaxis()->SetTitle("TDC1+TDC2");
 hkt->GetYaxis()->SetTitle("TDC3+TDC4");
 hktrot->GetXaxis()->SetTitle("rotated TDC");
TH1F* hKAONhits = new TH1F("hKAONhits","",6000,0,6000);
TH1F* hMIPhits = new TH1F("hMIPhits","",6000,0,6000);
TH1F* hKAONtriggerhits = new TH1F("hKAONtriggerhits","",10,0,10);
TH1F* hMIPtriggerhits = new TH1F("hMIPtriggerhits","",10,0,10);
TH1F* hDriftALL = new TH1F("hDriftALL","",1000,-33000,-31000);
TH1F* hDriftTDC = new TH1F("hDriftTDC","",1000,-33000,-31000);
TH1F* hDrift = new TH1F("hDrift","",1000,-33000,-31000);
TH1F* hDriftPeak = new TH1F("hDriftPeak","",1000,-33000,-31000);
TH1F* hDriftBkg = new TH1F("hDriftBkg","",1000,-33000,-31000);
TH1F* hDriftUnzoom = new TH1F("hDriftUnzoom","",2000,-33000,33000);
 hDriftALL->GetXaxis()->SetTitle("Drift time (1 channel = 8.3e-3 #mus)");
 hDriftTDC->GetXaxis()->SetTitle("Drift time (1 channel = 8.3e-3 #mus)");
 hDrift->GetXaxis()->SetTitle("Drift time (1 channel = 8.3e-3 #mus)");
 hDriftPeak->GetXaxis()->SetTitle("Drift time (1 channel = 8.3e-3 #mus)");
 hDriftBkg->GetXaxis()->SetTitle("Drift time (1 channel = 8.3e-3 #mus)");
 hDriftUnzoom->GetXaxis()->SetTitle("Drift time (1 channel = 8.3e-3 #mus)");
//Kaon trigger array:
int nArrKT = 0;
TObjArray* ArrKT;ArrKT =new TObjArray();ArrKT->SetName("KaonTrigger");
ArrKT->Add(hktwide);
ArrKT->Add(hkt);
ArrKT->Add(hktrot);
ArrKT->Add(hKAONhits);
ArrKT->Add(hMIPhits);
ArrKT->Add(hKAONtriggerhits);
ArrKT->Add(hMIPtriggerhits);
ArrKT->Add(hDriftALL);
ArrKT->Add(hDriftTDC);
ArrKT->Add(hDrift);
ArrKT->Add(hDriftUnzoom);
ArrKT->Add(hDriftPeak);
ArrKT->Add(hDriftBkg);



// Energy plots booking:
float xminE = 0.;
float xmaxE = 24000.;
int nbinsE = 1200;

hEsum = new TH1F("hEsum","",nbinsE,xminE,xmaxE);
hEsum->GetXaxis()->SetTitle("E (eV)");
hEsum->SetLineColor(4);
hEsum->SetLineWidth(2);

hEsum_trig=(TH1F*)hEsum->Clone("hEsum_trig");
hEgoodsum=(TH1F*)hEsum->Clone("hEgoodsum");
hEgoodsum_trig=(TH1F*)hEsum->Clone("hEgoodsum_trig");
hEgoodsum_bkg=(TH1F*)hEsum->Clone("hEgoodsum_bkg");

for(int iBUS=0;iBUS<nBUS;iBUS++){
 for(int iSDD=0;iSDD<nSDD;iSDD++){
  hE[iBUS][iSDD] = (TH1F*)hEsum->Clone(Form("hE_%i_%i",iBUS,iSDD));
  hEgood[iBUS][iSDD] = (TH1F*)hEsum->Clone(Form("hEgood_%i_%i",iBUS,iSDD));
  hE_trig[iBUS][iSDD] = (TH1F*)hEsum->Clone(Form("hE_trig_%i_%i",iBUS,iSDD));
  hEgood_trig[iBUS][iSDD] = (TH1F*)hEsum->Clone(Form("hEgood_trig_%i_%i",iBUS,iSDD));
 }
}

// Energy plots Arrays:
//TObjArray* ArrhE = new TObjArray();
//TObjArray* ArrhE_trig = new TObjArray();


// (old: now done from input) DEFINE HERE WHICH DATA (TO SET TDC AND DRIFT TIME CUTS
//-------------------------------------------------------
//int WhichData = 0; // siddhartino
//    WhichData = 1; // S2He


//some variables definitions
int preSDD,preADC,postSDD,postADC,theSDD,theADC,timediff,evdiff,t1,t2,iepbin,ratebin;
int iep,CPbin,oldCPbin,theBUS,preBUS;
float CP;
float PI = 3.14159265359;
bool ISGOODtimediff,ISGOODevdiff,IsGOOD,goodhit;
float tmin,tmax,tminbkg,tmaxbkg,tdcmin1,tdcmax1,tdcmin2,tdcmax2;

//DEFINE HERE TDC kaons window:
//-----------------------------
 tdcmin1 = 5900.; // siddhartino
 tdcmax1 = 5955.; 
 tdcmin2 = 6120.;
 tdcmax2 = 6155.; 
 if(WhichData==1){ //S2He (degrot475)
  tdcmin1 = 5780.; 
  tdcmax1 = 5880.; 
  tdcmin2 = 6000.; 
  tdcmax2 = 6090.; 
 }else if(WhichData==2){ //S2He1,2,3 (degrot350,600,475again)
  tdcmin1 = 5820.; 
  tdcmax1 = 5900.; 
  tdcmin2 = 6030.; 
  tdcmax2 = 6120.; 
 }else if(WhichData==3){ //S2He4 475again after trigger change
  tdcmin1 = 5810.;
  tdcmax1 = 5890.;
  tdcmin2 = 6030.;
  tdcmax2 = 6110.;
 }

//DEFINE HERE Drift time cut
//--------------------------
 tmin = -32580;// define drift time window best siddhartino
 tmax = -32340;// 
 if(WhichData==1 || WhichData==2 || WhichData==3){ //S2He (degrot475) || S2He1,2,3 (degrot350,600,475again) || S2He4 (475again after trigger change)
  tmin = -32600;
  tmax = -32460;
 }
 float twindow = tmax-tmin;
 tminbkg = -32300;
 tmaxbkg = tminbkg+twindow;

//Sferas definition:
//------------------
// 4 Sferas for file  20210209_0831_0209_1243_xray_25kv_100ua.root
// according to https://agenda.infn.it/event/25043/contributions/126916/attachments/78848/102558/17Feb_SIDD_DAFNE.pdf
int nSferas = 4;
//Sfera#0 1-16 (in siddhartino is Sfera #4, see ppt by Florin)
firstSDD[nSferas] = 1;
lastSDD[nSferas] = 16;
nSferas++;
//Sfera#1 17-32 (in siddhartino is Sfera #3, see ppt by Florin)
firstSDD[nSferas] = 17;
lastSDD[nSferas] = 32;
nSferas++;
//Sfera#2 33-48 (in siddhartino is Sfera #1, see ppt by Florin)
firstSDD[nSferas] = 33;
lastSDD[nSferas] = 48;
nSferas++;
//Sfera#3 49-64 (in siddhartino is Sfera #1, see ppt by Florin)
firstSDD[nSferas] = 49;
lastSDD[nSferas] = 64;
nSferas++;
int whichSfera[nSDD];
for(int iSfera=0;iSfera<nSferas;iSfera++){
 for(int iSDD=firstSDD[iSfera];iSDD<=lastSDD[iSfera];iSDD++){
  whichSfera[iSDD] = iSfera;
 }
}

//get calibration if available:
float UseG[nBUS][nSDD] = {};
float UseG0[nBUS][nSDD] = {};
TGraphErrors *gG;
TGraph *gG0;
TGraph *gIsGood;
Double_t x,y;
Int_t ix;
NGoodCal = 0;
//float minG = 2.9; //definition of GoodCal                             => obsolete, decision is done now at calibration stage
//float maxG = 3.9; //range of accepted gain for the SDD to be summed   => obsolete
int nG = 3; //number of graphs in calb tobjarra, first:G, second:G0, third:IsGoodCal
if(CalibArray->GetEntries()>0){ //DO WE HAVE A CALIBRATION LOADED?
 for(int iB=0;iB<nBUS;iB++){  // loop over buses
  gG = (TGraphErrors*) CalibArray->At(iB*nG+0);//first graph is G
  gG0 = (TGraph*) CalibArray->At(iB*nG+1);//second graph is G0
  gIsGood = (TGraph*) CalibArray->At(iB*nG+2);//third:IsGoodCal
  for(int ip=0;ip<gG->GetN();ip++){  // loop over calibrated SDD's in this bus
   gG->GetPoint(ip,x,y);
   ix = (Int_t)x;
   UseG[iB][ix] = y;
   //if(UseG[iB][ix]>minG&&UseG[iB][ix]<maxG) GoodCal[iB][ix] = true; //old obsolete
     gIsGood->GetPoint(ip,x,y);
     if(y==0)GoodCal[iB][ix] = false;
     if(y==1)GoodCal[iB][ix] = true;
     if(GoodCal[iB][ix]) NGoodCal++;
   gG0->GetPoint(ip,x,y);
   ix = (Int_t)x;
   UseG0[iB][ix] = y;
  }
  gG->Delete();
  gG0->Delete();
 }
 cout<<endl<<" CALIB LOADED With # "<<NGoodCal<<" SDD's with Good Calibration"<<endl<<endl;
}//calib loaded



// --- MAIN BUFFERS LOOP --- //
//______________________________________________________________________________________________//

if (fChain == 0) return;
Long64_t nentries = fChain->GetEntries();
Long64_t nbytes = 0, nb = 0;
cout<<"--> Nentries "<<nentries<<endl;
for (Long64_t jentry=0; jentry<nentries;jentry++) {
//for (Long64_t jentry=0; jentry<100;jentry++) {
 Long64_t ientry = LoadTree(jentry);
 if (ientry < 0) break;
 nb = fChain->GetEntry(jentry);   nbytes += nb;
 // if (Cut(ientry) < 0) continue;
 if (jentry % 50000 == 0) cout <<"Reading entry "<<jentry<< endl;


 // !!! SKIP LOOP if nhits == 0  !!!!
 if(nhits<1) {
  //cout<<"SKIP ENTRY "<<jentry<<" WITH nhits="<<nhits<<endl;
  //continue;
 }
 // !!! SKIP LOOP if nhits > MAXHITS-1  !!!!
 if(nhits>MAXHITS-1) {
  cout<<"SKIP ENTRY: MAX HITS REACHED! "<<jentry<<" WITH nhits="<<nhits<<endl;
  continue;
 }



 // --- EVENT RELATED STUFF --- //
 //-------------------------------//

 //get current:
 //-------------
 iep = ie+ip;
 iepbin = (int)iep/100;
 if(iep>=1500) iepbin = 15;
 CP = ie*ip;//current product
 CPbin = CP/10000; // 50 bins up to 500e3
 if(CP>500e3)CPbin=50;

 //Analyze events with injection?? 
 //===============================
 //InjectionFlag == 2   => Analyze all events
 //InjectionFlag == 1   => Analyze only INJECTION EVENTS
 //InjectionFlag == 0   => Analyze only NOT-INJECTION EVENTS
 if(InjectionFlag==1&&dum>=0)continue;
 if(InjectionFlag==0&&dum<0)continue;
 if(InjectionFlag==0&&CP==0)continue;//by DIANA, if InjectionFlag==0 analize only collision events

 //kaon trigger plots:
 hktwide->Fill(kt[0]+kt[1],kt[1]+kt[2]);
 hkt->Fill(kt[0]+kt[1],kt[1]+kt[2]);
 float ktrot = cos(PI/4)*(kt[0]+kt[1])+sin(PI/4)*(kt[2]+kt[3]);//x axis of 90deg rotation
 hktrot->Fill(ktrot);

 // Decide here if it passes TDC kaons cut
 //========================
 bool TDCkaons = false;
 if((ktrot>tdcmin1&&ktrot<tdcmax1)||(ktrot>tdcmin2&&ktrot<tdcmax2)) TDCkaons =true;

 if(TDCkaons){
  hKAONhits->Fill(nhits);
 }else{
  hMIPhits->Fill(nhits);
 }
 // Decide here if it passes Drift time window cut (and bkg)
 //=================================================

 // --- HITS LOOP --- //
 //-------------------//

 int ntrig = 0;// number of trigger hits
 for (int ihit = 0;ihit<nhits;ihit++){
  bool DriftPeak = false;
  bool DriftBkg = false;

  //get bus, sdd and adc:
  theBUS=bus[ihit];
  theADC=adc[ihit];
  theSDD=sdd[ihit];

  //get pre hits:
  //-------------
  preBUS = -1; if(ihit>0) preBUS=bus[ihit-1];
  preSDD = -1; if(ihit>0) preSDD=sdd[ihit-1];
  preADC = -1; if(ihit>0) preADC=adc[ihit-1];
  
  //SKIP FOR NOW EVENTS WITH NO "PRE" HIT !!!!!
  if(preADC>0){

   //get event diff:
   //---------------
   evdiff =-1;if(ihit>0) evdiff = evnr[ihit] - evnr[ihit-1];

   //get time diff
   //-------------
   timediff = -99999.;
   t1 = drift[ihit]+32768.;
   t2 = drift[ihit-1]+32768.;
   if(t1>t2) timediff = t1-t2;
   //if(t2>t1) timediff = t1+(32768.*2)-t2;
   if(t2>t1) timediff = t2-t1;

   //Decide if IsGOOD based in time/event_number (CROSSTALK REMOVAL)
   //--------------------------------------------===================
   ISGOODevdiff = true;
   if(evdiff==0) ISGOODevdiff=false; 
   ISGOODtimediff = true;
   if(timediff>0.&&timediff<620.) ISGOODtimediff = false;//std 5microseconds
   //if(timediff>0.&&timediff<4000.) ISGOODtimediff = false;//test ~20microseconds
   IsGOOD = true;
   // ==>  HERE DECIDE IF USE TIME OF EVENT AS CLASSIFIER !!!
   if(!ISGOODevdiff) IsGOOD = false; 
   //if(!ISGOODtimediff) IsGOOD = false; 
   // Flag the hit as good or not, considering also the Sfera
   goodhit = false;
   if(whichSfera[theSDD]>-1){
    if(whichSfera[theSDD]==whichSfera[preSDD]&&theBUS==preBUS){//same sfera and bus
     if(IsGOOD) goodhit = true;
    }else{//different sfera, just use the hit
     goodhit=true;
    }
   }


   // --- ADC plots --- //
   //-------------------//
   hADC[theBUS][theSDD]->Fill(theADC);
   hADCpre[theBUS][theSDD]->Fill(theADC,preADC);
   if(IsGOOD){
    hADCpreGOOD[theBUS][theSDD]->Fill(theADC,preADC);  //Watch out, here I do not req goodhit!!!
   }else{
    hADCpreBAD[theBUS][theSDD]->Fill(theADC,preADC);
   }
   if(goodhit) hADCgood[theBUS][theSDD]->Fill(theADC);

   // --- TRIGGERED hits ADC and DRIFT TIME PLOTS --- //
   //---------------------------------------------------
   if(trigg[ihit]==1){
    ntrig++;
    hADCtrig[theBUS][theSDD]->Fill(theADC);
    hDriftALL->Fill(drift[ihit]);

    //here check if the drfit time is in the signal time window:
    if(drift[ihit]>tmin&&drift[ihit]<tmax) DriftPeak = true; 
    if(drift[ihit]>tminbkg&&drift[ihit]<tmaxbkg) DriftBkg = true; 

    if(TDCkaons){
     hDriftTDC->Fill(drift[ihit]);
     if(GoodCal[theBUS][theSDD]&&goodhit) {  // VERY IMPORTANT: hDrift for goodcal and goodhit(no crosstalk) only hits!
      hDrift->Fill(drift[ihit]); 
      hDriftUnzoom->Fill(drift[ihit]);
      if(DriftPeak) hDriftPeak->Fill(drift[ihit]);
      if(DriftBkg) hDriftBkg->Fill(drift[ihit]);
     }
    }// if(TDCkaons) 
   }//end triggered hits



   // --- Energy plots --- //
   //----------------------//
   if(UseG[theBUS][theSDD]>0.){//we have calib for this sdd
    float theE = UseG0[theBUS][theSDD]+UseG[theBUS][theSDD]*theADC;
    // Energy plots NON-triggered:
    if(trigg[ihit]!=1){ // non-triggered
     hE[theBUS][theSDD]->Fill(theE);
     if(GoodCal[theBUS][theSDD]) hEsum->Fill(theE);
     if(goodhit){
      hEgood[theBUS][theSDD]->Fill(theE);
      if(GoodCal[theBUS][theSDD]) hEgoodsum->Fill(theE);
     }
    // Energy plots TRIGGERED:
    //              ----------
    }else{ // TRIGGERED:


     //FOR THE MOMENT, triggered spectra includes cut in TDCkaons and DriftTime
     //FOR THE MOMENT, triggered spectra includes cut in TDCkaons and DriftTime
     //FOR THE MOMENT, triggered spectra includes cut in TDCkaons and DriftTime
     //FOR THE MOMENT, triggered spectra includes cut in TDCkaons and DriftTime

//TDCkaons=true;//hack to include all triggered hits
//DriftPeak=true;//hack to include all triggered hits

     if(TDCkaons&&DriftPeak){
      hE_trig[theBUS][theSDD]->Fill(theE);
      if(GoodCal[theBUS][theSDD]) hEsum_trig->Fill(theE);  
      if(goodhit){
       hEgood_trig[theBUS][theSDD]->Fill(theE);
       if(GoodCal[theBUS][theSDD]) hEgoodsum_trig->Fill(theE);
      }
     }// FOR THE MOMENT, triggered spectra includes cut in TDCkaons and DriftTime
     
     //fill now also a bkg drift time window:
     if(TDCkaons&&DriftBkg){
      if(goodhit){
       if(GoodCal[theBUS][theSDD]) hEgoodsum_bkg->Fill(theE);
      }
     }
  
    }//end triggered
   }//we have calib


  }//if(preADC>0)  SKIP FOR NOW EVENTS WITH NO "PRE" HIT !!!

 }//end ihit loop
  
}//jentry loop





//---------------------------------------------------
// ==> IN THE END...Give some numbers as output:
//---------------------------------------------------

 // # of kaons with TDC
 Kint = 0.;
 int ibin_tdcmin =  hktrot->FindBin(tdcmin1);
 int ibin_tdcmax =  hktrot->FindBin(tdcmax1);
 Kint  = Kint + hktrot->Integral(ibin_tdcmin,ibin_tdcmax);
 ibin_tdcmin =  hktrot->FindBin(tdcmin2);
 ibin_tdcmax =  hktrot->FindBin(tdcmax2);
 Kint  = Kint + hktrot->Integral(ibin_tdcmin,ibin_tdcmax);
 cout<<endl<<" TDC Kaon Peaks Integral (simpler) = "<<Kint<<endl;
 TGraph* gTDCkaonsInt = new TGraph(); gTDCkaonsInt->SetName("gTDCkaonsInt");
 gTDCkaonsInt->SetPoint(0,1,Kint);
 //Integrate also drift time window:
 float DTint = 0.;
 DTint = hDriftPeak->Integral(1, hDriftPeak->GetNbinsX());
 DTint = DTint - hDriftBkg->Integral(1, hDriftBkg->GetNbinsX());
 cout<<" Drift Time Integral (bkg-subtracted) = "<<DTint<<endl;
 TGraph* gDriftPeakInt = new TGraph(); gDriftPeakInt->SetName("gDriftPeakInt");
 gDriftPeakInt->SetPoint(0,1,DTint);


 //define signal region:
 float Emin = 4000.;   // from 4-12 keV
 float Emax = 12000.;  // from 4-12 keV

 FUNCTION_NUMBERS();

 //print
 cout<<endl<<"# "<<NGoodCal<<" SDD's with GoodCalibration"<<endl;
 cout<<"# "<<nSDDon<<" SDD's are ON"<<endl;
 cout<<"# "<<nSDDGon<<" SDD's are with GoodCal and ON "<<endl;
 TGraph* gSDDon = new TGraph();
 gSDDon->SetName("gSDDon");
 gSDDon->SetPoint(0,1,nSDDGon);// gSDDon filled with SDD's that are on (see criteria avobe) and goodcal!

 cout<<endl<<"   DriftPeak ("<<tmin<<" to "<<tmax<<")  Entries = "<<hDriftPeak->GetEntries()<<endl;
 cout<<"   DriftBkg ("<<tminbkg<<" to "<<tmaxbkg<<")  Entries = "<<hDriftBkg->GetEntries()<<endl;
 //print it all
 printf("\n Tot K4He Lalpha yield = %.1f, per SDD = %.1f\n",TotHeYield,HeYieldperSDD);
 printf("                    /cm2 = %.1f\n",HeYieldpercm2);
 printf("                 /cm2/MK = %.1f\n",HeYieldpercm2MK);
 printf("               /cm2/pb-1 = %.1f\n\n",HeYieldpercm2pb);



 


























// --- Calibration ---
//______________________________________________________________________________________________________//
double eTi_KA = 4508.83;
double eTi_KA_esc = 3690.0;//?
double eTi_KB = 4931.81;
double eCu_KA = 8041.05;
double eCu_KB = 8905.29;
double eMn_KA = 5895.23;
double eFe_KA = 6399.47;
double eFe_KB = 7058.0;
double eW_KA = 8631.10;
double eW_KG = 11285.9;
double eBr_KA = 11908.26;
double eBr_KB = 13292.0;
double eBi_KA = 10828.1;//?
double eBi_KB = 13011.6;
double eBi_KG = 15247.7;
double eSr_KA = 14142.04;
double eSr_KB = 15836.0;
double eSr_es = 14006.29;//?
double ePb_KA = 10541.39;//?
double ePb_KB = 12616.14;
double ePb_KG = 14764.4;
//mine:
double eCa_KA = 3689.92;
double eAu_LA = 9670.65;
double eBi_LA = 10784.86;
double eAu_LB = 11513.5;


//Peak Finder configuration:
//==========================
const int nPFPeaksMAX = 13;//number of MAX peaks for peak finder
const int nPFPeaks = 3;//number DESIRED of peaks for peak finder
 // ==> SELECT HERE THE RANGE OF THE PeakFinder
 float xminPeakFinder = 1600;// Region of peak finding
 float xmaxPeakFinder = 3900;
//Ad hoc calib for last siddhartino He run:
//for sdd 45 xmaxPeakFinder=3300;
//for sdd 50 xmaxPeakFinder=3600;
//....and why SDD 41 is not calibrated?
int sigmaPeakFinder = 20; //sigma for peak Finder, in adc channels
sigmaPeakFinder=sigmaPeakFinder/rebinFactor;
if(IsCalib)cout<<"sigma peak finder "<<sigmaPeakFinder<<endl;
float InitThresholdPeakFinder = 0.01; //initial thresholdPar for peak finder, std in TSpectrum is 0.05
float InitTolerance = 0.05; // 5% -> Tolerance to check that peak assumption is correct
Double_t *xpeaks;
Double_t *ypeaks;
//float* xpeaks;
//float* ypeaks;
float xadc[nPFPeaksMAX] ={};
float yadc[nPFPeaksMAX] ={};
float PFPeakE[nPFPeaks] ={};
TGraph* gPeakFinderG[nBUS]; 
TGraph* gPeakFinderG0[nBUS];
for(int iBUS=0;iBUS<nBUS;iBUS++){
 gPeakFinderG[iBUS]= new TGraph();
 gPeakFinderG[iBUS]->SetName(Form("gPeakFinderG_%i",iBUS));
 gPeakFinderG0[iBUS]= new TGraph();
 gPeakFinderG0[iBUS]->SetName(Form("gPeakFinderG0_%i",iBUS));
}
TString* PFPeakName[nPFPeaks];
TF1* fPeakFinder[nBUS][nSDD];
TH1F* hPRECAL[nBUS][nSDD];
TGraphErrors* gPRECAL_linfit[nBUS][nSDD];
TGraphErrors* gPRECAL_linearity[nBUS][nSDD];
TGraphErrors* gPRECALG[nBUS]; 
TGraph* gPRECALG0[nBUS];
TGraph* gIsGoodCal[nBUS];
for(int iBUS=0;iBUS<nBUS;iBUS++){
 gPRECALG[iBUS]= new TGraphErrors();
 gPRECALG[iBUS]->SetName(Form("gPRECALG_%i",iBUS));
 gPRECALG0[iBUS]= new TGraph();
 gPRECALG0[iBUS]->SetName(Form("gPRECALG0_%i",iBUS));
 gIsGoodCal[iBUS]= new TGraph();
 gIsGoodCal[iBUS]->SetName(Form("gIsGoodCal_%i",iBUS));
}
int ipoint = 0;
TF1* fPRECAL[nBUS][nSDD];
int IsGoodCal[nBUS][nSDD] = {};
TSpectrum *spectrum = new TSpectrum(nPFPeaksMAX);
TObjArray* fPeakFinderArray = new TObjArray();
TObjArray* fPRECALArray = new TObjArray();
TObjArray* hPRECALArray = new TObjArray();
TObjArray* PRECALlinfitArray = new TObjArray();

//IsCalib = false; //hack to voluntarily skip calib

if(IsCalib){
 // Assumption for the PeakFinder peaks:  TiA - CuA - CuB
 //-------------------------------------------------------
 PFPeakE[0] = eTi_KA;
 PFPeakName[0] = new TString("Ti_KA");
 PFPeakE[1] = eCu_KA;
 PFPeakName[1] = new TString("Cu_KA");
 PFPeakE[2] = eCu_KB;
 PFPeakName[2] = new TString("Cu_KB");


 //PRECAL configuration:
 //==============================
 int BkgPars = 3; // BkgPars for bkg here: p0+exp(p1+p2*x)
 int gausnpar = 3;
 TString fstr ("");
 const int nPeaksMAX = 99;
 float PeakE[nPeaksMAX] ={-1.};
 TString* PeakName[nPeaksMAX];
 float AddPeakE[nPeaksMAX] = {-1.};
 TString * AddPeakName[nPeaksMAX];
 int nPeaks = 0;  
 int iInt[nPeaksMAX] = {-1};
 int iMean[nPeaksMAX] = {-1};
 int iSigma[nPeaksMAX] = {-1}; 
 int ip0 = -1;
 int ip1 = -1;
 int ip2 = -1;
 float xminPre[nPeaksMAX] = {-1.};
 float xmaxPre[nPeaksMAX] = {-1.};
 // ADD PEAKS for pre and final calib HERE:
 //----------------------------------------
 //firstly add PeakFinder peaks:
 for(int i=0;i<nPFPeaks;i++){
  AddPeakE[nPeaks]=PFPeakE[i];AddPeakName[nPeaks]=PFPeakName[i];nPeaks++;
 }
 //then add whatever you want:
 AddPeakE[nPeaks] = eTi_KB; AddPeakName[nPeaks] = new TString("Ti_KB");nPeaks++;
 //AddPeakE[nPeaks] = eCa_KA; AddPeakName[nPeaks] = new TString("Ca_KA"); nPeaks++;
 //AddPeakE[nPeaks] = eMn_KA; AddPeakName[nPeaks] = new TString("Mn_KA"); nPeaks++;
 //AddPeakE[nPeaks] = eFe_KA; AddPeakName[nPeaks] = new TString("Fe_KA"); nPeaks++;
 ////AddPeakE[nPeaks] = eFe_KB; AddPeakName[nPeaks] = new TString("Fe_KB"); nPeaks++;
 //AddPeakE[nPeaks] = eAu_LA; AddPeakName[nPeaks] = new TString("Au_LA"); nPeaks++;
 //AddPeakE[nPeaks] = eBi_LA; AddPeakName[nPeaks] = new TString("Bi_LA"); nPeaks++;
 //AddPeakE[nPeaks] = eAu_LB; AddPeakName[nPeaks] = new TString("Au_LB"); nPeaks++;
 
 //Ordered list of peaks for pre and final calibration:
 // -----------------------------------------------------
 //order it and save in PeakE and NamePeak:
 bool skip[nPeaksMAX] = {false};
 int PeakOrder[nPeaksMAX] ={-1};
 float themin = 999999.;
 int imin = 0;
 for(int i =0;i<nPeaks;i++){
  //find the smallest in the AddPeaks list, write it in PeakOrder and remove it:
  themin = 999999.;
  for(int j =0;j<nPeaks;j++){
   if(skip[j])continue;
   if(AddPeakE[j]<themin){
    themin = AddPeakE[j];
    imin = j;
   }
  }
  PeakOrder[i]=imin;
  cout<<" PeakOder["<<i<<"]="<<imin<<endl;
  skip[imin]=true;
 }
 //now actually reorder and print them 
 cout<<"Number of Peaks "<<nPeaks<<endl;
 for(int i =0;i<nPeaks;i++){
  PeakE[i] = AddPeakE[PeakOrder[i]];
  PeakName[i] = AddPeakName[PeakOrder[i]];
  cout<<" peak#"<<i<<" PeakName = "<<PeakName[i]->Data()<<" PeakE = "<<PeakE[i]<<endl;
 }

 //set PRECAL fit function: gaussians + bkg
 fstr = "";
 for(int i = 0; i<nPeaks; i++){
  iInt[i] = gausnpar*i; //define order of the parameters
  iMean[i] = gausnpar*i+1;
  iSigma[i] = gausnpar*i+2;
  if(i!=0) fstr += "+";
  fstr += Form("[%i]*exp(-0.5*pow(x-[%i],2)/pow([%i],2))/sqrt(2.*%f)/[%i]",iInt[i],iMean[i],iSigma[i],PI,iSigma[i]);
 }
 ip0 = gausnpar*nPeaks;
 ip1 = gausnpar*nPeaks+1;
 //ip2 = gausnpar*nPeaks+2;
 //fstr += Form("+exp([%i]+[%i]*x)+[%i]",ip0,ip1,ip2);
 fstr += Form("+[%i]+x*[%i]",ip0,ip1);
 cout<<" PRECAL fit Function is : "<<fstr<<endl;



 //BUS and SDD loop
 //----------------
 int NCalAttempt = 0;
 int NgoodCal = 0;
 for(int iBUS=0;iBUS<nBUS;iBUS++){
  ipoint = 0;
  for(int iSDD=0;iSDD<nSDD;iSDD++){
   IsGoodCal[iBUS][iSDD] = 0;
   TH1F* thehisto; 
   thehisto = (TH1F*) hADCgood[iBUS][iSDD]->Clone("thehisto");thehisto->SetName("thehisto");
   int minstatsForCalib = 1000;
   if(thehisto->GetEntries()<minstatsForCalib) continue;
   NCalAttempt++;

   //  --- Peak Finder Calibration --- //
   //----------------------------------//
   cout<<endl<<endl<<"---PEAK FINDER B"<<iBUS<<" S"<<iSDD<<" stat#"<<thehisto->GetEntries()<<endl;
   
   //restrict to PeakFinder area:
   float xmaxPeakFinder_save = xmaxPeakFinder;
   //Ad hoc calib for last siddhartino He run:
   //for sdd 45 xmaxPeakFinder=3300;
   //for sdd 50 xmaxPeakFinder=3600;
   //....and why SDD 41 is not calibrated?
   //if(iSDD==45) xmaxPeakFinder=3300.;
   //if(iSDD==50) xmaxPeakFinder=3600.;
   thehisto->GetXaxis()->SetRangeUser(xminPeakFinder,xmaxPeakFinder);
   xmaxPeakFinder = xmaxPeakFinder_save;
 
   //peak finder config:
   Int_t nfound = 0;
   float thresholdPeakFinder = InitThresholdPeakFinder;
   int nPFtries = 0;
   while(nfound<nPFPeaks&&nPFtries<50){
    nfound = spectrum->Search(thehisto,sigmaPeakFinder,"",thresholdPeakFinder);
    printf("Found %d candidate peaks to fit\n",nfound);
    thresholdPeakFinder = thresholdPeakFinder*.1;//TSpectrum std=0.05. Change it til it finds the peaks
    nPFtries++;
   }
   if(nPFtries>=50) {
    cout<<" PEAK FINDER DIDNT WORK, ==> CONTINUE"<<endl;
    continue;
   }
   xpeaks = spectrum->GetPositionX();//array with X-positions of the centroids found by TSpectrum 
   ypeaks = spectrum->GetPositionY();//array with X-positions of the centroids found by TSpectrum 
 
   //reorder in adc counts and check if compatible with maximum peaks assumption
   themin = 999999.;
   imin = 0;
   for(int i =0;i<nfound;i++){ //find the smallest, write it in xadc and remove it:
    themin = 999999.;
    for(int j =0;j<nfound;j++){
     if(xpeaks[j]<themin){
      themin = xpeaks[j];
      imin = j;
     }
    }
    xadc[i]=xpeaks[imin];
    yadc[i]=ypeaks[imin];
    xpeaks[imin]=999999.;
   }
   //print them, now ordered:
   for(int i =0;i<nfound;i++){cout<<" found peak ADC="<<xadc[i]<<" height "<<yadc[i]<<endl;}
   cout<<endl;


   //check if the peaks found are compatible with the assumption:
   //------------------------------------------------------------
   float eDist10 = PFPeakE[1]-PFPeakE[0];
   float eDist21 = PFPeakE[2]-PFPeakE[1];
   float Erelation = eDist21/eDist10;
   //make trios out of all found peaks:
   float GPF =0.;
   float G0PF =0;
   bool TestPassed = false;
   int ipeak0 = -1;
   int ipeak1 = -1;
   int ipeak2 = -1;
   for(int i0=0; i0<nfound; i0++){
    for(int i1=i0+1; i1<nfound; i1++){
     for(int i2=i1+1; i2<nfound; i2++){
      cout<<endl<<" -> trying trio "<<i0<<" "<<i1<<" "<<i2<<endl;   
 
      float xDist10 = xadc[i1]-xadc[i0];
      float xDist21 = xadc[i2]-xadc[i1];
      float ADCrelation = xDist21/xDist10;
      cout<<" Check assumption: Energy relation "<<Erelation<<" vs ADC relation "<<ADCrelation<<endl;
 
      //Define tolerance parameter
      float tol = InitTolerance; // 5%
      bool TolerancePass = true;
      if(fabs(1.-(Erelation/ADCrelation))>tol) TolerancePass = false;
      if(!TolerancePass) cout<<" Tolerance not passed: tolERROR!!!  "<<endl;
 
      // Get Peak Finder calibration Offset G0PF and Slope GPF
      float Dadc = xadc[i0]-xadc[i1];
      float De = PFPeakE[0]-PFPeakE[1];
      GPF = De/Dadc;
      G0PF = -1.*xadc[i0]*GPF+PFPeakE[0];
      cout<<"PeakFinder cal, offset G0PF "<<G0PF<<" slope GPF "<<GPF<<endl;
 
      //define an acceptable gain and offset and check if tolerance, G, and G0 are ok:
      float mingoodG = 2.9;float maxgoodG = 3.9;
      float mingoodG0 = -3000;float maxgoodG0 = -1000;
      if(GPF<maxgoodG&&GPF>mingoodG&&G0PF<maxgoodG0&&G0PF>mingoodG0&&TolerancePass){
       cout<<" -- TEST PASSED! --"<<endl<<endl; 
       TestPassed = true;
       IsGoodCal[iBUS][iSDD] = 1; // there will be an additional request that the error in the prefit is below 1%, see IsGoodCal later in the prefit
       ipeak0 = i0;
       ipeak1 = i1;
       ipeak2 = i2;
       //NgoodCal++; // because of the additional request of error in the prefit below 1%, we now do this sum later
      }
      if(TestPassed) break;
     }
     if(TestPassed) break;
    }
    if(TestPassed) break;
   }

   //find also the highest height among the selected ones (ipeak0,ipeak1,ipeak2):
   float highestheight = 0.;
   if(ipeak0>-1){
    if(yadc[ipeak0]>highestheight) highestheight = yadc[ipeak0];
    if(yadc[ipeak1]>highestheight) highestheight = yadc[ipeak1];
    if(yadc[ipeak2]>highestheight) highestheight = yadc[ipeak2];
   }

   // peak finder all done, now a couple of  histograms and function:
   fPeakFinder[iBUS][iSDD] = new TF1(Form("fPeakFinder_%i_%i",iBUS,iSDD),"pol1",0.,8000.);
   fPeakFinder[iBUS][iSDD]->SetParameter(0,G0PF);
   fPeakFinder[iBUS][iSDD]->SetParameter(1,GPF);
   fPeakFinderArray->Add(fPeakFinder[iBUS][iSDD]);
   //G and G0 plots:
   gPeakFinderG[iBUS]->SetPoint(ipoint,iSDD,GPF);
   gPeakFinderG0[iBUS]->SetPoint(ipoint,iSDD,G0PF);


  
   //  ---  Pre-Calibration --- //
   //----------------------------------//
   if(!QuickCalib){
    float sigmaADCguess = 20.;//start with an initial peak resolution in ADC of 20
    //Fit limits:
    for(int i=0;i<nPeaks;i++){
     xminPre[i] = (PeakE[i]-G0PF)/GPF - 2.5*sigmaADCguess;
     xmaxPre[i] = (PeakE[i]-G0PF)/GPF + 3.*sigmaADCguess;
    }
    //put to infinite the errors outside the limits so this region is not fitted:
    for(int ibin=1;ibin<=thehisto->GetNbinsX();ibin++){
     bool keep = false;
     for(int i=0;i<nPeaks;i++){
      if(thehisto->GetBinCenter(ibin)>xminPre[i]&&thehisto->GetBinCenter(ibin)<xmaxPre[i]) keep=true;
     }
     if(!keep) thehisto->SetBinError(ibin,999999.);
    }
    //set function:
    TF1* fPre = new TF1("fPre",fstr,xminPre[0],xmaxPre[nPeaks-1]);
    //set initial parameters:
    for(int i=0;i<nPeaks;i++){
     fPre->SetParameter(iInt[i],highestheight*sigmaADCguess);//initialize: height of highest PFpeak*sigma
     fPre->SetParName(iInt[i],Form("gauss%i_Hei",i));
     //cout<<"fPre->SetParameter("<<iInt[i]<<","<<yadc[0]*sigmaADCguess<<");"<<endl;
     fPre->SetParameter(iMean[i],  (PeakE[i]-G0PF)/GPF   );
     fPre->SetParName(iMean[i],Form("gauss%i_Mea",i));
     //cout<<"fPre->SetParameter("<<iMean[i]<<","<<(PeakE[i]-G0PF)/GPF<<");"<<endl;
     fPre->SetParameter(iSigma[i],  sigmaADCguess   );
     fPre->SetParName(iSigma[i],Form("gauss%i_Sig",i));
     //cout<<"fPre->SetParameter("<<iSigma[i]<<","<<sigmaADCguess<<");"<<endl;
    }
    //set pol:
    fPre->SetParameter(ip0,thehisto->GetEntries()/thehisto->GetNbinsX()/20);
    fPre->SetParLimits(ip0,0.,thehisto->GetEntries()/thehisto->GetNbinsX());
    //cout<<"fPre->SetParameter("<<ip0<<","<<thehisto->GetEntries()/thehisto->GetNbinsX()/10<<");"<<endl;
    fPre->SetParameter(ip1,0.);
    fPre->SetParLimits(ip1,-0.01,0.01);
    //cout<<"fPre->SetParameter("<<ip1<<",0.);"<<endl;
  
    //do the pre-Fit
    //thehisto->Fit(fPre,"","0R",xminPre[0],xmaxPre[nPeaks-1]);
    thehisto->Fit(fPre,"","R",xminPre[0],xmaxPre[nPeaks-1]);
  
    //put back original errors:
    for(int ibin=1;ibin<=thehisto->GetNbinsX();ibin++){
     thehisto->SetBinError(ibin,hADCgood[iBUS][iSDD]->GetBinError(ibin));
    }
 
    //Fit straight line to the peaks to get PRECAL calibration function
    gPRECAL_linfit[iBUS][iSDD] = new TGraphErrors();
    gPRECAL_linfit[iBUS][iSDD]->SetName(Form("gPRECAL_linfit_%i_%i",iBUS,iSDD));
    for(int i=0;i<nPeaks;i++){
     gPRECAL_linfit[iBUS][iSDD]->SetPoint(i, PeakE[i], fPre->GetParameter(iMean[i]) ); 
     gPRECAL_linfit[iBUS][iSDD]->SetPointError(i, 0., fPre->GetParError(iMean[i])); 
    }
    TF1* fline = new TF1("fline","pol1");
    fline->SetParameter(0, -1.*G0PF/GPF);//inital pars from peak finder
    fline->SetParameter(1, 1./GPF);//inital pars from peak finder
    gPRECAL_linfit[iBUS][iSDD]->Fit(fline,"0");
    float G0Pre = -1.*fline->GetParameter(0)/fline->GetParameter(1);
    float GPre = 1./fline->GetParameter(1);
    float GPre_error = fline->GetParError(1)/pow(fline->GetParameter(1),2);
    cout<<endl<<" PRECALIBRATION COMPLETED: G0 = "<<G0Pre<<"    G = "<<GPre<<endl<<endl<<endl;
    gPRECAL_linfit[iBUS][iSDD]->SetMarkerStyle(21);
    PRECALlinfitArray->Add(gPRECAL_linfit[iBUS][iSDD]);
    
 
    //PRECAL histograms and function:
    hPRECAL[iBUS][iSDD] = (TH1F*) thehisto->Clone(Form("hPRECAL_%i_%i",iBUS,iSDD));
//    hPRECAL[iBUS][iSDD]->SetName(Form("hPRECAL_%i_%i",iBUS,iSDD));
//    hPRECAL[iBUS][iSDD]->GetYaxis()->SetRangeUser(.0001,highestheight*1.2);
    fPRECAL[iBUS][iSDD] = new TF1(Form("fPRECAL_%i_%i",iBUS,iSDD),"pol1",0.,8000.);
    fPRECAL[iBUS][iSDD]->SetParameter(0, G0Pre  );
    fPRECAL[iBUS][iSDD]->SetParameter(1, GPre );
    fPRECALArray->Add(fPRECAL[iBUS][iSDD]);
    hPRECALArray->Add(hPRECAL[iBUS][iSDD]);
    //linearity plot:
    gPRECAL_linearity[iBUS][iSDD] = new TGraphErrors();
    gPRECAL_linearity[iBUS][iSDD]->SetName(Form("gPRECAL_linearity_%i_%i",iBUS,iSDD));
    for(int i=0;i<nPeaks;i++){
     gPRECAL_linearity[iBUS][iSDD]->SetPoint(i,PeakE[i],fPre->GetParameter(iMean[i])-fline->Eval(PeakE[i]));
     gPRECAL_linearity[iBUS][iSDD]->SetPointError(i,0.,fPre->GetParError(iMean[i]));
    }
    //G and G0 plots:
    gPRECALG[iBUS]->SetPoint(ipoint,iSDD,GPre);
    gPRECALG[iBUS]->SetPointError(ipoint,0.,GPre_error);
    gPRECALG0[iBUS]->SetPoint(ipoint,iSDD,G0Pre);
    //additional requirement to declare good cal, prefit error below 1%:
    if(GPre_error/GPre>.01) IsGoodCal[iBUS][iSDD]=0.;
    if(IsGoodCal[iBUS][iSDD]==1.) NgoodCal++;
    gIsGoodCal[iBUS]->SetPoint(ipoint,iSDD,IsGoodCal[iBUS][iSDD]);
    ipoint++;
 
   //clean the mess
    fline->Delete();
   }//!QuickCalib
 
   thehisto->Delete();
   printf("SDD calibrated: %i / %i \n",NgoodCal,NCalAttempt);
 
  }//sdd loop
 }//bus loop
 printf("\n %i NICELY CALIBRATED SDD's out of %i with stats, tol=%.0f percent\n\n",NgoodCal,NCalAttempt,InitTolerance*100);
}//if(IsCalib)




































// --- WRITE HISTOGRAMS TO FILE ---
//______________________________________________________________________________________________________//
TFile* fout = new TFile(sfileout,"RECREATE");

int minstats  = 100;//write histos only if entries>minstats;

//ADC arrays
TDirectory *dirs[MaxArrays];
for(int iArr=0;iArr<nArrADC;iArr++){
//create dir
 dirs[iArr] = (TDirectory*)fout->mkdir(ArrADC[iArr]->GetName());dirs[iArr]->cd();
 for(int ih=0;ih<ArrADC[iArr]->GetEntriesFast();ih++){
  if(((TH1F*)ArrADC[iArr]->At(ih))->GetEntries()>minstats) ArrADC[iArr]->At(ih)->Write(); 
 }
}

//See Cross talk
fout->cd();
TObjArray* CROSSTALK = new TObjArray();
THStack *THScrosstalk[nBUS][nSDD];
for(int iBUS=0;iBUS<nBUS;iBUS++){
 for(int iSDD=0;iSDD<nSDD;iSDD++){
  if(hADC[iBUS][iSDD]->GetEntries()>0){
   THScrosstalk[iBUS][iSDD] = new THStack(Form("CrossTalk_%i_%i",iBUS,iSDD),"");
   THScrosstalk[iBUS][iSDD]->Add(hADC[iBUS][iSDD],"nostack");
   THScrosstalk[iBUS][iSDD]->Add(hADCgood[iBUS][iSDD],"nostack");
   CROSSTALK->Add(THScrosstalk[iBUS][iSDD]);
  }
 }
}
CROSSTALK->Write("CROSSTALK",TObject::kSingleKey);

//KT array
 fout->cd();
 ArrKT->Write("KaonTrigger",TObject::kSingleKey);
if(IsCalib){
 //PEAK FINDER ARRAY:
 TObjArray* PeakFinderArray = new TObjArray();
 for(int iBUS=0;iBUS<nBUS;iBUS++){
  gPeakFinderG[iBUS]->SetMarkerStyle(21);
  gPeakFinderG0[iBUS]->SetMarkerStyle(21);
  PeakFinderArray->Add(gPeakFinderG[iBUS]);
  PeakFinderArray->Add(gPeakFinderG0[iBUS]);
 }
 PeakFinderArray->Write("PeakFinder_G_G0",TObject::kSingleKey);
 //PRECAL ARRAYS
 if(!QuickCalib){
  //histos
  hPRECALArray->Write("PRECAL_histos",TObject::kSingleKey);
  //linfit:
  PRECALlinfitArray->Write("PRECAL_LinearFit",TObject::kSingleKey);
  //G,G0
  TObjArray* PRECALArray = new TObjArray();
  for(int iBUS=0;iBUS<nBUS;iBUS++){
   gPRECALG[iBUS]->SetMarkerStyle(21);
   gPRECALG0[iBUS]->SetMarkerStyle(21);
   gIsGoodCal[iBUS]->SetMarkerStyle(25);
   PRECALArray->Add(gPRECALG[iBUS]);
   PRECALArray->Add(gPRECALG0[iBUS]);
   PRECALArray->Add(gIsGoodCal[iBUS]);
  }
  PRECALArray->Write("PRECAL_G_G0",TObject::kSingleKey);
 }
}

//write ENERGY histos
if(!IsCalib){ 
 //non-triggered
 TObjArray* ArrhE = new TObjArray();
 TObjArray* ArrhEgood = new TObjArray();
 for(int iBUS=0;iBUS<nBUS;iBUS++){
  for(int iSDD=0;iSDD<nSDD;iSDD++){
   if(hE[iBUS][iSDD]->GetEntries()>0) ArrhE->Add(hE[iBUS][iSDD]);  
   if(hEgood[iBUS][iSDD]->GetEntries()>0) ArrhEgood->Add(hEgood[iBUS][iSDD]);  
  }
 }
 ArrhE->Write("ENERGY",TObject::kSingleKey);
 ArrhEgood->Write("ENERGY_GOOD",TObject::kSingleKey);

 TObjArray* ENERGY_CROSSTALK = new TObjArray(); // crosstalk for energy plots
 THStack *THSenergy[nBUS+1][nSDD+1]; //+1 because the last one is the sum
 for(int iBUS=0;iBUS<nBUS;iBUS++){
  for(int iSDD=0;iSDD<nSDD;iSDD++){
   if(hE[iBUS][iSDD]->GetEntries()>0){
    THSenergy[iBUS][iSDD] = new THStack(Form("Energy_CrossTalk_%i_%i",iBUS,iSDD),"");
    THSenergy[iBUS][iSDD]->Add(hE[iBUS][iSDD],"nostack");
    hEgood[iBUS][iSDD]->SetLineColor(2);
    THSenergy[iBUS][iSDD]->Add(hEgood[iBUS][iSDD],"nostack");
    ENERGY_CROSSTALK->Add(THSenergy[iBUS][iSDD]);
   }
  }
 }
 THSenergy[nBUS][nSDD] = new THStack("Energy_CrossTalk_sum","");
 THSenergy[nBUS][nSDD]->Add(hEsum);
 hEgoodsum->SetLineColor(2);
 THSenergy[nBUS][nSDD]->Add(hEgoodsum);
 ENERGY_CROSSTALK->Add(THSenergy[nBUS][nSDD]);
 ENERGY_CROSSTALK->Write("ENERGY_CROSSTALK",TObject::kSingleKey);

 hEsum->Write();
 hEgoodsum->Write();

 // TRIGGERED
 TObjArray* ArrhE_trig = new TObjArray();
 TObjArray* ArrhEgood_trig = new TObjArray();
 for(int iBUS=0;iBUS<nBUS;iBUS++){
  for(int iSDD=0;iSDD<nSDD;iSDD++){
   if(hE_trig[iBUS][iSDD]->GetEntries()>0) ArrhE_trig->Add(hE_trig[iBUS][iSDD]);  
   if(hEgood_trig[iBUS][iSDD]->GetEntries()>0) ArrhEgood_trig->Add(hEgood_trig[iBUS][iSDD]);
  }
 }
 ArrhE_trig->Write("ENERGY_trig",TObject::kSingleKey);
 ArrhEgood_trig->Write("ENERGY_GOOD_trig",TObject::kSingleKey);
 
 TObjArray* ENERGY_CROSSTALK_trig = new TObjArray(); // crosstalk for energy plots
 THStack *THSenergy_trig[nBUS+1][nSDD+1]; //+1 because the last one is the sum
 for(int iBUS=0;iBUS<nBUS;iBUS++){
  for(int iSDD=0;iSDD<nSDD;iSDD++){
   if(hE_trig[iBUS][iSDD]->GetEntries()>0){
    THSenergy_trig[iBUS][iSDD] = new THStack(Form("Energy_CrossTalk_trig_%i_%i",iBUS,iSDD),"");
    THSenergy_trig[iBUS][iSDD]->Add(hE_trig[iBUS][iSDD],"nostack");
    hEgood_trig[iBUS][iSDD]->SetLineColor(2);
    THSenergy_trig[iBUS][iSDD]->Add(hEgood_trig[iBUS][iSDD],"nostack");
    ENERGY_CROSSTALK_trig->Add(THSenergy_trig[iBUS][iSDD]);
   }
  }
 }
 THSenergy_trig[nBUS][nSDD] = new THStack("Energy_CrossTalk_sum_trig","");
 THSenergy_trig[nBUS][nSDD]->Add(hEsum_trig);
 hEgoodsum_trig->SetLineColor(2);
 THSenergy_trig[nBUS][nSDD]->Add(hEgoodsum_trig);
 ENERGY_CROSSTALK_trig->Add(THSenergy_trig[nBUS][nSDD]);
 ENERGY_CROSSTALK_trig->Write("ENERGY_CROSSTALK_trig",TObject::kSingleKey);
 
 hEsum_trig->Write();
 hEgoodsum_trig->Write();
 hEgoodsum_bkg->Write();
 hpercm2->Write();
 hpercm2MK->Write();
 hpercm2pb->Write();

TDirectory *Numbers = (TDirectory*)fout->mkdir("Numbers");Numbers->cd();
 gSDDon->SetMarkerStyle(20);gSDDon->Write();
 gTotHeYield->SetMarkerStyle(20);gTotHeYield->Write();
 gHeYieldperSDD->SetMarkerStyle(20);gHeYieldperSDD->Write();
 gTDCkaonsInt->SetMarkerStyle(20);gTDCkaonsInt->Write();
 gDriftPeakInt->SetMarkerStyle(20);gDriftPeakInt->Write();
 gHeYieldpercm2->SetMarkerStyle(20);gHeYieldpercm2->Write();
 gHeYieldpercm2MK->SetMarkerStyle(20);gHeYieldpercm2MK->Write();
 gHeYieldpercm2pb->SetMarkerStyle(20);gHeYieldpercm2pb->Write();

 fout->cd();
 TDirectory *ZONES = (TDirectory*)fout->mkdir("ZONES");ZONES->cd();
 for(int i=0;i<nzones;i++) {
  lINCL[i]->Write();
  lTRIG[i]->Write();
  lSIG[i]->Write();
  gINCL[i]->Write();
  gTRIG[i]->Write();
  gSIG[i]->Write();
  gINCLbad[i]->Write();
  gTRIGbad[i]->Write();
  gSIGbad[i]->Write();
 }


fout->cd();

}//!IsCalib


fout->Close();//close output file
}

//________________________
void SDD::FUNCTION_NUMBERS(){

 //define signal region:
 float Emin = 4000.;   // from 4-12 keV
 float Emax = 12000.;  // from 4-12 keV

 //define integrals of INCL, trig spectrum and signal region for all SDD's:
 nall = hEgoodsum->Integral( hEgoodsum->FindBin(Emin), hEgoodsum->FindBin(Emax));
 ntrig = hEgoodsum_trig->Integral( hEgoodsum_trig->FindBin(Emin), hEgoodsum_trig->FindBin(Emax));
 nbkg = hEgoodsum_bkg->Integral( hEgoodsum_bkg->FindBin(Emin), hEgoodsum_bkg->FindBin(Emax));

 float EminkeV=Emin/1000.;
 float EmaxkeV=Emax/1000.;
 printf("\n INCLUSIVE hits (in %.0f-%.0fkeV) = %.0f\n",EminkeV,EmaxkeV,nall);
 printf(" TRIGGER hits (%.0f-%.0fkeV) = %.0f\n",EminkeV,EmaxkeV,ntrig);
 printf(" BKG window hits (%.0f-%.0fkeV) = %.0f\n\n",EminkeV,EmaxkeV,nbkg);

 nSDDon=0;
 nSDDGon=0;
 float MAX = 5.* (1./((int)NGoodCal))  ; // max signal/sdd  x5 times what expected from #calibsdd
 float MIN = (1./((int)NGoodCal)) /5. ; // min signal/sdd  /5 times what expected from #calibsdd
 cout<<endl<<" MIN AND MAX for SDDon CRITERIUM : MIN="<<MIN<<"  MAX="<<MAX<<endl;
 float minONentries = 10000.; // OLD! consider on with 10k entries in INCL spectrum
 for(int iBUS=0;iBUS<nBUS;iBUS++){
  for(int iSDD=0;iSDD<nSDD;iSDD++){
   IsGoodSDD[iBUS][iSDD] = false;
   int binmin = hEgood[iBUS][iSDD]->FindBin(Emin);
   int binmax = hEgood[iBUS][iSDD]->FindBin(Emax);
   INCLint[iBUS][iSDD] = hEgood[iBUS][iSDD]->Integral(binmin,binmax);
   TRIGint[iBUS][iSDD] = hEgood_trig[iBUS][iSDD]->Integral(binmin,binmax);
     //CALCULATE NUMBER OF ON SDDs:
         // OLD: just look at #hits in non trig spectrum
         //if(INCLint[iBUS][iSDD]>minONentries) nSDDon++; 
         //if(GoodCal[iBUS][iSDD]&&INCLint[iBUS][iSDD]>minONentries) nSDDGon++;
     //NEW: #hits in incl. (and trig. spectrum?) between 5 and /10 times what expected from numer of calib sdds:
//cout<<iBUS<<" "<<iSDD<<endl;
//cout<<"inc "<<INCLint[iBUS][iSDD]<<" INCLint[iBUS][iSDD]/nall "<<INCLint[iBUS][iSDD]/nall<< endl;
//cout<<"trig "<<TRIGint[iBUS][iSDD]<<" TRIGint[iBUS][iSDD]/ntrig "<<TRIGint[iBUS][iSDD]/ntrig<< endl;
     if( INCLint[iBUS][iSDD]/nall > MIN && INCLint[iBUS][iSDD]/nall < MAX
      //&&TRIGint[iBUS][iSDD]/ntrig > MIN && TRIGint[iBUS][iSDD]/ntrig < MAX
      ) {
      nSDDon++; 
      if(GoodCal[iBUS][iSDD]) {
       IsGoodSDD[iBUS][iSDD] = true;
       nSDDGon++; 
      }
     }
  }
 } 

 // (quick) FIT K4He Lalpha
 float xmin = 6100.;
 float xmax = 7500.;
 TF1* fheb = new TF1("fheb","gaus(0)+pol0(3)",xmin,xmax);
 float xmean = 6470.;
 fheb->SetParameter(0,hEgoodsum_trig->GetBinContent(hEgoodsum_trig->FindBin(xmean)));
 fheb->SetParameter(1,xmean);
 float xsigma = 110.;
 fheb->SetParameter(2,xsigma);
 float pol0 = hEgoodsum_trig->GetBinContent(hEgoodsum_trig->FindBin(xmin));
 fheb->SetParameter(3,pol0);
 hEgoodsum_trig->Fit(fheb,"R","",xmin,xmax);
 TF1* fhe = new TF1("fhe","gaus",xmin,xmax);
 fhe->SetParameter(0,fheb->GetParameter(0));
 fhe->SetParameter(1,fheb->GetParameter(1));
 fhe->SetParameter(2,fheb->GetParameter(2));
 xmean = fheb->GetParameter(1);
 xsigma = fheb->GetParameter(2);
 pol0 = fheb->GetParameter(3);
 float nsigmas = 3;
 float xintmin = xmean-nsigmas*xsigma;
 float xintmax = xmean+nsigmas*xsigma;
 int ibinmin = hEgoodsum_trig->FindBin(xintmin);
 int ibinmax = hEgoodsum_trig->FindBin(xintmax);
 //OLD (takes signal as the integral of the gaussian):
 // float TotHeYield = fhe->Integral(xintmin,xintmax)/hEgoodsum_trig->GetBinWidth(1);
 //NEW, takes signal as the Integral of the histo minus the integral of the bkg
  TotHeYield = hEgoodsum_trig->Integral(ibinmin,ibinmax);
  bkgint= (ibinmax-ibinmin+1)*pol0;
  TotHeYield = TotHeYield-bkgint;
  //here save also total signal region without bkg subutr (nsig) and tha same for each sdd:
  nsig = hEgoodsum_trig->Integral(ibinmin,ibinmax); 
  for(int iBUS=0;iBUS<nBUS;iBUS++){
   for(int iSDD=0;iSDD<nSDD;iSDD++){
    SIGint[iBUS][iSDD] = hEgood_trig[iBUS][iSDD]->Integral(ibinmin,ibinmax);
   }
  }
 HeYieldperSDD = TotHeYield/((float)nSDDGon);
 SDDcm2 = 0.64;
 HeYieldpercm2 = TotHeYield/(((float)nSDDGon)*SDDcm2);
 gTotHeYield = new TGraph(); gTotHeYield->SetName("gTotHeYield");
 gTotHeYield->SetPoint(0,1,TotHeYield);
 gHeYieldperSDD = new TGraph(); gHeYieldperSDD->SetName("gHeYieldperSDD");
 gHeYieldperSDD->SetPoint(0,1,HeYieldperSDD);
 //create also histo per cm2:
 hpercm2 = (TH1F*) hEgoodsum_trig->Clone("hpercm2");
 hpercm2->GetYaxis()->SetTitle("Xrays/cm2");
 hpercm2->Scale(1./(((float)nSDDGon)*SDDcm2));
 gHeYieldpercm2 = new TGraph(); gHeYieldpercm2->SetName("gHeYieldpercm2");
 gHeYieldpercm2->SetPoint(0,1,HeYieldpercm2);
 //create also histo per cm2 per 10^6 Kaons(from TDC spectrum)
 MK = Kint/1e6;
 hpercm2MK = (TH1F*) hpercm2->Clone("hpercm2MK");
 hpercm2MK->GetYaxis()->SetTitle("Xrays/cm2/MK");
 hpercm2MK->Scale(1./MK);
 HeYieldpercm2MK = HeYieldpercm2/MK;
 gHeYieldpercm2MK = new TGraph(); gHeYieldpercm2MK->SetName("gHeYieldpercm2MK");
 gHeYieldpercm2MK->SetPoint(0,1,HeYieldpercm2MK);
 //create also histo per cm2 per pb-1(from luminosity input in run.C)
 hpercm2pb = (TH1F*) hpercm2->Clone("hpercm2pb");
 hpercm2pb->GetYaxis()->SetTitle("Xrays/cm2/pb-1");
 hpercm2pb->Scale(1./Luminositypb);
 HeYieldpercm2pb = HeYieldpercm2/Luminositypb;
 gHeYieldpercm2pb = new TGraph(); gHeYieldpercm2pb->SetName("gHeYieldpercm2pb");
 gHeYieldpercm2pb->SetPoint(0,1,HeYieldpercm2pb);

 //==> Graphs for INCLUSIVE, triggered spectrum, estimation of signal in function of the position of the SDD:
 for(int i=0;i<nzones;i++) {
  gINCL[i] = new TGraphErrors();gINCL[i]->SetName(Form("gINCL%i",i));
  gINCL[i]->SetMarkerStyle(21);
  gTRIG[i] = (TGraphErrors*)gINCL[i]->Clone(Form("gTRIG%i",i));
  gTRIG[i]->SetLineColor(4);
  gTRIG[i]->SetMarkerColor(4);
  gSIG[i] = (TGraphErrors*)gINCL[i]->Clone(Form("gSIG%i",i));
  gSIG[i]->SetLineColor(2);
  gSIG[i]->SetMarkerColor(2);
  gINCLbad[i] = (TGraphErrors*)gINCL[i]->Clone(Form("gINCLbad%i",i));
  gINCLbad[i]->SetMarkerStyle(25);
  gTRIGbad[i] = (TGraphErrors*)gTRIG[i]->Clone(Form("gTRIGbad%i",i));
  gTRIGbad[i]->SetMarkerStyle(25);
  gSIGbad[i] = (TGraphErrors*)gSIG[i]->Clone(Form("gSIGbad%i",i));
  gSIGbad[i]->SetMarkerStyle(25);
  lINCL[i] = new TGraph();lINCL[i]->SetName(Form("lINCL%i",i));
  lTRIG[i] = new TGraph();lTRIG[i]->SetName(Form("lTRIG%i",i));
  lSIG[i] = new TGraph();lSIG[i]->SetName(Form("lSIG%i",i));

 }
//PERCENT: Signal of SDD/ total signal:
float sumINCL[nzones] = {};
float nsddINCL[nzones] = {};
float sumTRIG[nzones] = {};
float sumSIG[nzones] = {};
 for(int iBUS=0;iBUS<nBUS;iBUS++){
  for(int iSDD=0;iSDD<nSDD;iSDD++){
   int iwhere = where(iBUS,iSDD);
   int iSDDlocal = iBUS*100+iSDD;
   if(IsGoodSDD[iBUS][iSDD]){
//    if(INCLint[iBUS][iSDD]>=100.) {
     gINCL[iwhere]->SetPoint( gINCL[iwhere]->GetN(), iSDDlocal , INCLint[iBUS][iSDD]/nall);
     sumINCL[iwhere] += INCLint[iBUS][iSDD];
     nsddINCL[iwhere]++;
     gTRIG[iwhere]->SetPoint( gTRIG[iwhere]->GetN(), iSDDlocal , TRIGint[iBUS][iSDD]/ntrig);
     sumTRIG[iwhere] += TRIGint[iBUS][iSDD];
     gSIG[iwhere]->SetPoint( gSIG[iwhere]->GetN(), iSDDlocal , SIGint[iBUS][iSDD]/nsig);
     sumSIG[iwhere] += SIGint[iBUS][iSDD];
//    }
  }else{
     gINCLbad[iwhere]->SetPoint( gINCLbad[iwhere]->GetN(), iSDDlocal , INCLint[iBUS][iSDD]/nall);
     gTRIGbad[iwhere]->SetPoint( gTRIGbad[iwhere]->GetN(), iSDDlocal , TRIGint[iBUS][iSDD]/ntrig);
     gSIGbad[iwhere]->SetPoint( gSIGbad[iwhere]->GetN(), iSDDlocal , SIGint[iBUS][iSDD]/nsig);
   }
  }
 } 
//EXPECTED PERCENT: Signal of ZONE/ nSDD in ZONE / total signal:
//  This expected signal is a graph with two points to cover from 0 to XXXX (max #SDD per zone)
 float lmin = 80;
 float lmax = 680;

 for(int i=0;i<nzones;i++){
  //if(i>2) lmin = 80;
  //if(i>2) lmax = 380;
  sumINCL[i] = sumINCL[i] / nsddINCL[i] / nall;
  lINCL[i]->SetPoint(0,lmin,sumINCL[i]);
  lINCL[i]->SetPoint(1,lmax,sumINCL[i]);
  lINCL[i]->SetMarkerStyle(29);
  lINCL[i]->SetLineWidth(2);
  sumTRIG[i] = sumTRIG[i] / nsddINCL[i] / ntrig;
  lTRIG[i]->SetPoint(0,lmin,sumTRIG[i]);
  lTRIG[i]->SetPoint(1,lmax,sumTRIG[i]);
  lTRIG[i]->SetLineColor(4);
  lTRIG[i]->SetMarkerColor(4);
  lTRIG[i]->SetMarkerStyle(29);
  lTRIG[i]->SetLineWidth(2);
  sumSIG[i] = sumSIG[i] / nsddINCL[i] / nsig;
  lSIG[i]->SetPoint(0,lmin,sumSIG[i]);
  lSIG[i]->SetPoint(1,lmax,sumSIG[i]);
  lSIG[i]->SetLineColor(2);
  lSIG[i]->SetMarkerColor(2);
  lSIG[i]->SetMarkerStyle(29);
  lSIG[i]->SetLineWidth(2);
 }

TCanvas* c2 = new TCanvas("c2");
c2->Divide(2,3);
 for(int i=0;i<nzones;i++){ 

lINCL[i]->GetYaxis()->SetRangeUser(0.,0.01);

//draw left boost side, right antiboost side
if(i<3){ c2->cd(i*2+1);}else{ c2->cd((i-3)*2+2);  };

lINCL[i]->Draw("APL");

lTRIG[i]->Draw("PLsame");
lSIG[i]->Draw("PLsame");

gINCL[i]->Draw("Psame");
gTRIG[i]->Draw("Psame");
gSIG[i]->Draw("Psame");

gINCLbad[i]->Draw("Psame");
gTRIGbad[i]->Draw("Psame");
gSIGbad[i]->Draw("Psame");
}


 
}
