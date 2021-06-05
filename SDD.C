#define SDD_cxx
#include "SDD.h"
#include <TRandom3.h>
#include <TH2.h>
#include <TF1.h>
#include <TGraphErrors.h>
#include <TCutG.h>
#include <TFile.h>
#include <TSpectrum.h>
#include <TLorentzVector.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <deque>
#include <vector>

void SDD::Loop(Int_t InjectionFlag){


// --- Define histograms and stuff --- //
//______________________________________________________________________________________________________//

const int nSDD = 65; //last SDD #64
bool writeSDD[nSDD] = {};
bool goodcalSDD[nSDD] = {};
int WriteIfHits = 999;//write SDD (and calib) if at least 1k hits 
int nbinsadc = 8000;
int minadc = 0.;
int maxadc = 8000.;

//plain adc spectra
TH1F* hADC[nSDD];
TH1F* hADC_trig[nSDD];

//event number diff:
TH1F* hEvDiff[nSDD];

//time diff:
TH1F* hTimeDiff[nSDD];

//set all defined histograms:
for(int iSDD=0;iSDD<nSDD;iSDD++){
  hADC[iSDD] = new TH1F(Form("ADC%i",iSDD),"",nbinsadc,minadc,maxadc);
  hADC_trig[iSDD] = new TH1F(Form("ADC%i_trig",iSDD),"",nbinsadc,minadc,maxadc);
  hADC[iSDD]->GetXaxis()->SetTitle("ADC");
  hADC_trig[iSDD]->GetXaxis()->SetTitle("ADC");
  hEvDiff[iSDD] = new TH1F(Form("EvDiff%i",iSDD),"",4,-1.5,2.5);
  hTimeDiff[iSDD] = new TH1F(Form("TimeDiff%i",iSDD),"",10000,0.,66000);
}

//2D histograms
//-------------
int rebinfactor = 20;
nbinsadc = nbinsadc/rebinfactor;//rebin for 2D histos

//ADC
TH2F* hADCpre[nSDD];
//TH2F* hADCpre_[nSDD][nSDD];
TH2F* hADCpre_[nSDD][nSDD+2];//+2 to add two histos: InSfera, NotInSfera
TH2F* hADCpreBADtime[nSDD];
TH2F* hADCpreGOODtime[nSDD];
TH2F* hADCpreGOODevdiff[nSDD];
TH2F* hADCpreBADevdiff[nSDD];
TH2F* hADCpre_GOODev_GOODtime[nSDD];
TH1F* hADCpre_GOODev_GOODtime1D[nSDD];
TH1F* hADCgood[nSDD];//define a simple histo "good": crosstak checked only within sfera <--THIS IS THE FINAL OUTPUT
TH2F* hADCpre_GOODev_BADtime[nSDD];
TH2F* hADCpre_BADev_GOODtime[nSDD];
TH2F* hADCpre_BADev_BADtime[nSDD];
TH2F* hADCgoodCurrent[nSDD]; 
TH2F* hADCgoodCurrent2[nSDD]; 
TH2F* hADCgoodRate[nSDD]; 
TH2F* hADCgoodRate2[nSDD]; 
TH1F* hADCgoodZeroCurrent[nSDD]; 
TH1F* hADCgoodWithCurrent[nSDD]; 

for(int iSDD=0;iSDD<nSDD;iSDD++){
 hADCpre[iSDD] = new TH2F(Form("ADCpre%i",iSDD),"",nbinsadc,minadc,maxadc,nbinsadc,minadc,maxadc);
 for(int jSDD=0;jSDD<nSDD;jSDD++){
  hADCpre_[iSDD][jSDD] = new TH2F(Form("ADC%i_pre%i",iSDD,jSDD),"",nbinsadc,minadc,maxadc,nbinsadc,minadc,maxadc);
 }
 hADCpreGOODtime[iSDD] = new TH2F(Form("ADC%i_GOODtime",iSDD),"",nbinsadc,minadc,maxadc,nbinsadc,minadc,maxadc);
 hADCpreBADtime[iSDD] = new TH2F(Form("ADC%i_BADtime",iSDD),"",nbinsadc,minadc,maxadc,nbinsadc,minadc,maxadc);
 hADCpreGOODevdiff[iSDD] = new TH2F(Form("ADC%i_GOODev",iSDD),"",nbinsadc,minadc,maxadc,nbinsadc,minadc,maxadc);
 hADCpreBADevdiff[iSDD] = new TH2F(Form("ADC%i_BADev",iSDD),"",nbinsadc,minadc,maxadc,nbinsadc,minadc,maxadc);
 hADCpre_GOODev_GOODtime[iSDD] = new TH2F(Form("ADC%i_GOODev_GOODtime",iSDD),"",nbinsadc,minadc,maxadc,nbinsadc,minadc,maxadc);
 hADCpre_GOODev_GOODtime1D[iSDD] = new TH1F(Form("ADC%i_GOODev_GOODtime1D",iSDD),"",nbinsadc*rebinfactor,minadc,maxadc);
 hADCgood[iSDD] = new TH1F(Form("ADC%igood",iSDD),"",nbinsadc*rebinfactor,minadc,maxadc);
 hADCgood[iSDD]->GetXaxis()->SetTitle("ADC");
 hADCpre_GOODev_BADtime[iSDD] = new TH2F(Form("ADC%i_GOODev_BADtime",iSDD),"",nbinsadc,minadc,maxadc,nbinsadc,minadc,maxadc);
 hADCpre_BADev_GOODtime[iSDD] = new TH2F(Form("ADC%i_BADev_GOODtime",iSDD),"",nbinsadc,minadc,maxadc,nbinsadc,minadc,maxadc);
 hADCpre_BADev_BADtime[iSDD] = new TH2F(Form("ADC%i_BADev_BADtime",iSDD),"",nbinsadc,minadc,maxadc,nbinsadc,minadc,maxadc);
 //current dependent
 hADCgoodCurrent[iSDD] = new TH2F(Form("ADC%igoodCurrent",iSDD),"",nbinsadc,minadc,maxadc,15,0,15);
 hADCgoodCurrent2[iSDD] = new TH2F(Form("ADC%igoodCurrent2",iSDD),"",nbinsadc,minadc,maxadc,150,0.,1500.);
 hADCgoodZeroCurrent[iSDD] = new TH1F(Form("ADC%igoodZeroCurrent",iSDD),"",nbinsadc*rebinfactor,minadc,maxadc);
 hADCgoodWithCurrent[iSDD] = new TH1F(Form("ADC%igoodWithCurrent",iSDD),"",nbinsadc*rebinfactor,minadc,maxadc);
 //rate dependent:
 hADCgoodRate[iSDD] = new TH2F(Form("ADC%igoodRate",iSDD),"",nbinsadc,minadc,maxadc,10,0,10);
 hADCgoodRate2[iSDD] = new TH2F(Form("ADC%igoodRate2",iSDD),"",nbinsadc,minadc,maxadc,100,0.,100.);
}

//Sferas histos:
//-------------
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
int whichSfera[nSDD] = {-1};//THIS IS A BUG!
for(int iSfera=0;iSfera<nSferas;iSfera++){
 for(int iSDD=firstSDD[iSfera];iSDD<=lastSDD[iSfera];iSDD++){
  whichSfera[iSDD] = iSfera;
 }
}
for(int iSDD=0;iSDD<nSDD;iSDD++){
 hADCpre_[iSDD][nSDD] = new TH2F(Form("ADC%i_preInSfera",iSDD),"",nbinsadc,minadc,maxadc,nbinsadc,minadc,maxadc);
 hADCpre_[iSDD][nSDD+1] = new TH2F(Form("ADC%i_preNotInSfera",iSDD),"",nbinsadc,minadc,maxadc,nbinsadc,minadc,maxadc);
}

//define other variables
int preSDD,preADC,postSDD,postADC,theSDD,theADC,timediff,evdiff,t1,t2,iepbin,ratebin,iep,CPbin,oldCPbin;
int oldbuffertime,olddate;
olddate = 0;
oldCPbin = 0;
float CP;
bool ISGOODtime,ISGOODevdiff;
int rmin,rmax,totmin;
float PI = 3.14159265359;

//vectors and deques for rate and current calculation
deque<int> rate_nhits;
deque<int> rate_date;
const int CPnbins = 50;
//vector<int> CPtime[CPnbins];//old method, WRONG
int CPtime[CPnbins] = {};
TH1F* hCPtime = new TH1F("hCPtime","",CPnbins,0.,(float)CPnbins);
TH1F* hBuffertime = new TH1F("hBuffertime","",100,0.,100.);
uint rate_depth = 10; //use last 10 buffers to calculate rate
float rate = 0.;
TH2F* rate_c = new TH2F("rate_c","",100,0,2000.,100,0.,200);
bool activeSDD[nSDD] = {};
int NactiveSDD = 0;

//kaon trigger plots:
TH2F* hktwide = new TH2F("hktwide","",100,0,8000,100,0,8000);
TH2F* hkt = new TH2F("hkt","",1500,3500,5000,1500,3500,5000);
TH1F* hktrot = new TH1F("hktrot","",1000,5500,6500);
hktwide->GetXaxis()->SetTitle("TDC1+TDC2");
hktwide->GetYaxis()->SetTitle("TDC3+TDC4");
hkt->GetXaxis()->SetTitle("TDC1+TDC2");
hkt->GetYaxis()->SetTitle("TDC3+TDC4");
hktrot->GetXaxis()->SetTitle("rotated TDC");

TH1F* hdrift = new TH1F("hdrift","",2000,-33000,-31000);
TH2F* hEdrift = new TH2F("hEdrift","",200,0,20000,200,-33000,-31000);
hdrift->GetXaxis()->SetTitle("Drift time (1 channel = 8.3e-3 #mus)");
hEdrift->GetXaxis()->SetTitle("E (eV)");
hEdrift->GetYaxis()->SetTitle("Drift time (1 channel = 8.3e-3 #mus)");


//Energy plots:
float xminE = 0.;
float xmaxE = 20000.;
int nbinsE = 4000;
TH1F* hE[nSDD]; // Calibrated energy spectrum
TH1F* hE_trig[nSDD]; // Calibrated energy spectrum
TH2F* hEdrift_trig[nSDD]; // Calibrated energy spectrum
bool SUMMED[nSDD] = {};
float xminCP = 0.;
float xmaxCP = 500e3;
TH1F* hEsum = new TH1F("hEsum","",nbinsE,xminE,xmaxE);
TH1F* hEsum_trig = new TH1F("hEsum_trig","",nbinsE,xminE,xmaxE);
TH2F* hEsumCP = new TH2F("hEsumCP","",nbinsE/10,xminE,xmaxE,CPnbins,xminCP,xmaxCP);// CP is product of currents
TH2F* hEsumCP_trig = new TH2F("hEsumCP_trig","",nbinsE/10,xminE,xmaxE,CPnbins,xminCP,xmaxCP);// CP is product of currents
hEsum->GetXaxis()->SetTitle("E (eV)");
hEsum_trig->GetXaxis()->SetTitle("E (eV)");
hEsumCP->GetXaxis()->SetTitle("E (eV)");
hEsumCP_trig->GetXaxis()->SetTitle("E (eV)");
hEsumCP->GetYaxis()->SetTitle("ie*ip (mA^{2})");
hEsumCP_trig->GetYaxis()->SetTitle("ie*ip (mA^{2})");
for(int iSDD = 0;iSDD<nSDD;iSDD++){
 hE[iSDD] = new TH1F(Form("hE%i",iSDD),"",nbinsE,xminE,xmaxE);
 hE_trig[iSDD] = new TH1F(Form("hE%i_trig",iSDD),"",nbinsE,xminE,xmaxE);
 hEdrift_trig[iSDD] = new TH2F(Form("hEdrift%i_trig",iSDD),"",200,0,20000,200,-33000,-31000);
 hE[iSDD]->GetXaxis()->SetTitle("E (eV)");
 hE_trig[iSDD]->GetXaxis()->SetTitle("E (eV)");
 hEdrift_trig[iSDD]->GetXaxis()->SetTitle("E (eV)");
 hEdrift_trig[iSDD]->GetYaxis()->SetTitle("Drift time (1 channel = 8.3e-3 #mus)");
}
TH1F* hEsum_trig_tdc = (TH1F*)hEsum_trig->Clone("hEsum_trig_tdc");hEsum_trig_tdc->SetName("hEsum_trig_tdc");
TH1F* hEsum_trig_tdc_drift = (TH1F*)hEsum_trig->Clone("hEsum_trig_tdc_drift");hEsum_trig_tdc_drift->SetName("hEsum_trig_tdc_drift");
TH1F* hEsum_trig_mip = (TH1F*)hEsum_trig->Clone("hEsum_trig_mip");hEsum_trig_mip->SetName("hEsum_trig_mip");
TH1F* hEsum_trig_mip_nodrift = (TH1F*)hEsum_trig->Clone("hEsum_trig_mip_nodrift");hEsum_trig_mip_nodrift->SetName("hEsum_trig_mip_nodrift");


//get calibration if available:
float UseG0[nSDD] = {};
float UseG[nSDD] = {};
if(CalibArray->GetEntries()>0){ //DO WE HAVE A CALIBRATION LOADED?
 cout<<" Using Calibration with "<<CalibArray->GetEntries()<<" SDD's calibrated"<<endl;
 TF1* fcal;
 char* fnamechar;
 TString sfname;
 for(int j=0;j<CalibArray->GetEntries();j++){  // loop over calibrated SDDs:
  fcal = (TF1*) CalibArray->At(j); //take function from array:
  sfname = (TString) fcal->GetName();
  int iSDD=99999;
  if(sfname.Contains("PreCal")){//calib is from Pre-Cal with format "fPreCalXX"
   TString sfnumber(sfname(7,8));
   iSDD=sfnumber.Atoi();
  }else if(sfname.Contains("PeakFinder")){//calib is from PeakFinder with format "fPeakFinderXX"
   TString sfnumber(sfname(11,12));
   iSDD=sfnumber.Atoi();
  }
  UseG0[iSDD] =fcal->GetParameter(0);
  UseG[iSDD] =fcal->GetParameter(1);
  fcal->Delete();
 }
}
//calib loaded



// --- MAIN BUFFERS LOOP --- //
//______________________________________________________________________________________________________//

if (fChain == 0) return;
Long64_t nentries = fChain->GetEntriesFast();
Long64_t nbytes = 0, nb = 0;
cout<<" Nentries "<<nentries<<endl;
for (Long64_t jentry=0; jentry<nentries;jentry++) {
//for (Long64_t jentry=0; jentry<100;jentry++) {
 Long64_t ientry = LoadTree(jentry);
 if (ientry < 0) break;
 nb = fChain->GetEntry(jentry);   nbytes += nb;
 // if (Cut(ientry) < 0) continue;

 if(nhits<1) {
  cout<<"SKIP ENTRY "<<jentry<<" WITH nhits="<<nhits<<endl;
  continue;
 }


 //calculate rate
 //--------------
 //cout<<"-----------------------"<<endl;
 //cout<<" EVENT "<<jentry<<"  nhits "<<nhits<<" date "<<date<<endl;
 NactiveSDD=0;
 for(int iSDD=0;iSDD<nSDD;iSDD++){
  if(activeSDD[iSDD]) NactiveSDD++; //NactiveSDD is given by the previous buffer! :/
  activeSDD[iSDD] = false;
 }
 rate = 0.;
 rate_nhits.push_back(nhits);
 rate_date.push_back(date);
 //cout<<" buffer size after filling "<<rate_nhits.size()<<endl;
 if(rate_nhits.size()==rate_depth){
  for(uint ibus=0;ibus<rate_nhits.size();ibus++){
   rate += rate_nhits.at(ibus);
   //cout<<"    "<<ibus<<" adding "<<rate_nhits.at(ibus)<<"  sum="<<rate<<endl;
  }
  //cout<<" first time "<<rate_date.at(0)<<endl;
  //cout<<" last time "<<rate_date.at(rate_nhits.size()-1)<<endl;
  //cout<<" total seconds "<<rate_date.at(rate_nhits.size()-1) - rate_date.at(0)<<endl;
  rate = rate / (rate_date.at(rate_nhits.size()-1) - rate_date.at(0)) / NactiveSDD ;
  rate_nhits.pop_front();//remove first component
  rate_date.pop_front();//remove first component
 }
 //cout<<" final buffer size "<<rate_nhits.size()<<endl;
 //cout<<"rate="<<rate<<endl;
 ratebin = (int)rate/10;
 if(rate>=100) ratebin = 10;

 //get current:
 //-------------
  iep = ie+ip;
  iepbin = (int)iep/100;
  if(iep>=1500) iepbin = 15;
  CP = ie*ip;//current product
  CPbin = CP/10000; // 50 bins up to 500e3
  if(CP>500e3)CPbin=50;
  //if(!(CPtime[CPbin].size()>0&&CPtime[CPbin].at(CPtime[CPbin].size()-1)==date)) CPtime[CPbin].push_back(date);//old method, WRONG
  if(jentry>0){
   oldbuffertime = date-olddate;
   hBuffertime->Fill(oldbuffertime);
   //CPtime[oldCPbin] += oldbuffertime;
   //now add injection rejection flag:
   if(InjectionFlag==2) {
    CPtime[oldCPbin] += oldbuffertime;
   } else {
    if(InjectionFlag==1&&dum==1) CPtime[oldCPbin] += oldbuffertime;
    if(InjectionFlag==0&&dum==0) CPtime[oldCPbin] += oldbuffertime;
   }
  }
  //cout<<" CPbin "<<CPbin<<" oldCPbin "<<oldCPbin<<" oldbuffertime "<<oldbuffertime<<" CPtime[oldCPbin] "<<CPtime[oldCPbin]<<endl;
  oldCPbin = CPbin;//save CPbin for the next buffer
  olddate = date;//save the date ;)
  if(jentry==nentries-1){ //last buffer
   CPtime[oldCPbin] += 10;//put a generic time of 10s for the last buffer
  }

  //rate vs current plot
  if(rate>0.) rate_c->Fill(iep,rate);

 //Analyze events with injection?? 
 //===============================
 //InjectionFlag == 2   => Analyze all events
 //InjectionFlag == 1   => Analyze only INJECTION EVENTS
 //InjectionFlag == 0   => Analyze only NOT-INJECTION EVENTS
 //  OLD: in raw data the flag is dum, dum == 0 means no injection, dum ==1 means injection
 //  OLD: if(InjectionFlag==1&&dum!=1) continue;
 //  OLD: if(InjectionFlag==0&&dum!=0) continue;
 //NEW from intergration of kaon trigger:
 if(InjectionFlag==1&&dum>=0) continue;
 if(InjectionFlag==0&&dum<0) continue;

 //kaon trigger plots:
 hktwide->Fill(kt[0]+kt[1],kt[1]+kt[2]);
 hkt->Fill(kt[0]+kt[1],kt[1]+kt[2]);
 float ktrot = cos(PI/4)*(kt[0]+kt[1])+sin(PI/4)*(kt[2]+kt[3]);//x axis of 90deg rotation
 hktrot->Fill(ktrot);


 // --- HITS LOOP --- //
 //-------------------//
 for (int ihit = 0;ihit<nhits;ihit++){

  //get adc and sdd:
  theADC=adc[ihit];
  theSDD=sdd[ihit];
  activeSDD[theSDD]=true;


  // non-triggered SPECTRA:
  //-----------------------
  bool histoit=false;
  if(trigg[ihit]!=1){

   //get pre hits:
   //-------------
   preSDD = -1; if(ihit>0) preSDD=sdd[ihit-1];
   preADC = -1; if(ihit>0) preADC=adc[ihit-1];
   //preSDD = -1; if(ihit<nhits-1) preSDD=sdd[ihit+1]; // QUICK DIRTY TRY WITH POST
   //preADC = -1; if(ihit<nhits-1) preADC=adc[ihit+1]; // QUICK DIRTY TRY WITH POST
   //preSDD = -1; if(ihit>1) preSDD=sdd[ihit-2];//just to check previous previous hit
   //preADC = -1; if(ihit>1) preADC=adc[ihit-2];//just to check previous previous hit

   //get post hits:
   postSDD = -1; if(ihit<nhits-1) postSDD=sdd[ihit+1];
   postADC = -1; if(ihit<nhits-1) postADC=adc[ihit+1];
   //postSDD = -1; if(ihit<nhits-2) postSDD=sdd[ihit+2];//just to check post post hit
   //postADC = -1; if(ihit<nhits-2) postADC=adc[ihit+2];//just to check post post hit

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

    //Decide if IS GOOD based in time/event_number:
    //---------------------------------------------
    ISGOODevdiff = true;
    if(evdiff==0) ISGOODevdiff=false; 
    ISGOODtime = true;
    if(timediff>0.&&timediff<620.) ISGOODtime = false;//std 5microseconds
    //if(timediff>0.&&timediff<4000.) ISGOODtime = false;//test ~20microseconds

    //ADC plots
    //---------
    hADC[theSDD]->Fill(theADC);
    hADCpre[theSDD]->Fill(theADC,preADC);
    hADCpre_[theSDD][preSDD]->Fill(theADC,preADC);
    if(ISGOODtime) hADCpreGOODtime[theSDD]->Fill(theADC,preADC);
    if(!ISGOODtime) hADCpreBADtime[theSDD]->Fill(theADC,preADC);
    if(ISGOODevdiff) hADCpreGOODevdiff[theSDD]->Fill(theADC,preADC);
    if(!ISGOODevdiff) hADCpreBADevdiff[theSDD]->Fill(theADC,preADC);
    if(ISGOODtime&&ISGOODevdiff) hADCpre_GOODev_GOODtime[theSDD]->Fill(theADC,preADC);
    if(ISGOODtime&&ISGOODevdiff) hADCpre_GOODev_GOODtime1D[theSDD]->Fill(theADC);
    if(!ISGOODtime&&ISGOODevdiff) hADCpre_GOODev_BADtime[theSDD]->Fill(theADC,preADC);
    if(ISGOODtime&&!ISGOODevdiff) hADCpre_BADev_GOODtime[theSDD]->Fill(theADC,preADC);
    if(!ISGOODtime&&!ISGOODevdiff) hADCpre_BADev_BADtime[theSDD]->Fill(theADC,preADC);

    //histogram it?
    if(whichSfera[theSDD]>-1){
     if(whichSfera[theSDD]==whichSfera[preSDD]){//same sfera, check if it is crosstalk:
      if(ISGOODtime&&ISGOODevdiff) histoit = true;
     }else{//different sfera, just histo it:
      histoit=true;
     }
    }

    //ADC-current plots:
    //------------------
    if(histoit){
     hADCgood[theSDD]->Fill(theADC);//simple good histo
     //if(ISGOODtime&&ISGOODevdiff)hADCgood[theSDD]->Fill(theADC);//simple good histo
     hADCgoodCurrent[theSDD]->Fill(theADC,iepbin);//simple good histo
     hADCgoodCurrent2[theSDD]->Fill(theADC,iep);//simple good histo
     hADCgoodRate[theSDD]->Fill(theADC,ratebin);//simple good histo
     hADCgoodRate2[theSDD]->Fill(theADC,rate);//simple good histo
     if(iep<=0)hADCgoodZeroCurrent[theSDD]->Fill(theADC);//simple good histo
     if(iep>0)hADCgoodWithCurrent[theSDD]->Fill(theADC);//simple good histo
    }
  
    //Sfera plots:
    if(whichSfera[theSDD]>-1){
     if(whichSfera[theSDD]==whichSfera[preSDD]){
      hADCpre_[theSDD][nSDD]->Fill(theADC,preADC);//same sfera
     }else{
      hADCpre_[theSDD][nSDD+1]->Fill(theADC,preADC);//different sfera
     }
    }
   
    //evdiff plots
    hEvDiff[theSDD]->Fill(evdiff);
 
    //timediff plots
    hTimeDiff[theSDD]->Fill( timediff );

   }//if(preADC>0) -> only non-triggred hits with prehit
 

  //TRIGGERED hits ADC 
  //------------------
  }else{ //  if(trigg[ihit]!=1)
   hADC_trig[theSDD]->Fill(theADC);
   hdrift->Fill(drift[ihit]);
  }//end triggered hits


  //ENERGY PLOTS
  //------------
  if(UseG0[theSDD]!=0&&UseG0[theSDD]!=0){//we have calib for this sdd
   float theE = UseG0[theSDD]+UseG[theSDD]*theADC;
   float minG = 2.5;//range of accepted gain for the SDD to be summed
   float maxG = 4.;
   if(trigg[ihit]!=1){//non-triggered:
    if(histoit){
     hE[theSDD]->Fill(theE);
     if(UseG[theSDD]>minG&&UseG[theSDD]<maxG){
      goodcalSDD[theSDD]=1;
      hEsum->Fill(theE);
      hEsumCP->Fill(theE,CP);
     }
    }
   }else{ // TRIGGERED:
    hE_trig[theSDD]->Fill(theE);
    hEdrift_trig[theSDD]->Fill(theE,drift[ihit]);
    if(UseG[theSDD]>minG&&UseG[theSDD]<maxG){
     hEsum_trig->Fill(theE);
     if((ktrot>5880&&ktrot<5950)||(ktrot>6100&&ktrot<6170)){
      hEsum_trig_tdc->Fill(theE);
      if((drift[ihit]>-32600&&drift[ihit]<-32400)){
       hEsum_trig_tdc_drift->Fill(theE);
      }
     }
    else{
     hEsum_trig_mip->Fill(theE);
     if((drift[ihit]>-32400)){
      hEsum_trig_mip_nodrift->Fill(theE);
     }
    }
     hEsumCP_trig->Fill(theE,CP);
     hEdrift->Fill(theE,drift[ihit]);
    }
   }
  }



 }//end ihit loop
}//end entries loop

//CPtime plot:
for(int i=0;i<CPnbins;i++){
 hCPtime->SetBinContent(i+1,CPtime[i]);
}

//write/calib sdd?
for(int iSDD=0;iSDD<nSDD;iSDD++){
 if(hADCgood[iSDD]->GetEntries()>=WriteIfHits) writeSDD[iSDD]=true; //write if hADCgood has >10k entries
}





























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
const int nPFPeaks = 3;//number of peaks for peak finder
float xminPeakFinder = 1500;// Region of peak finding
float xmaxPeakFinder = 3800;
int sigmaPeakFinder = 10; //sigma for peak Finder, in adc channels
float InitThresholdPeakFinder = 0.05; //initial thresholdPar for peak finder, std in TSpectrum is 0.05
float InitTolerance = 0.05; // 5% -> Tolerance to check that peak assumption is correct
Double_t *xpeaks;
Double_t *ypeaks;
float xadc[nPFPeaks] ={-1.};
float yadc[nPFPeaks] ={-1.};
float PFPeakE[nPFPeaks] ={-1.};
TString* PFPeakName[nPFPeaks];
TF1* fPeakFinder[nSDD];
TH1F* hPreCal[nSDD];
TGraphErrors* gPreCal[nSDD];
TGraphErrors* gPreCalLin[nSDD];
TGraph* gPreCalG = new TGraph();gPreCalG->SetName("gPreCalG");
TGraph* gPreCalG0 = new TGraph();gPreCalG0->SetName("gPreCalG0");
int ipoint = 0;
TF1* fPreCal[nSDD];
TSpectrum *spectrum = new TSpectrum(nPFPeaks);
TObjArray* fPeakFinderArray = new TObjArray();
TObjArray* fPreCalArray = new TObjArray();
if(IsCalib){
 // Assumption for the PeakFinder peaks:  TiA - CuA - CuB
 //-------------------------------------------------------
 PFPeakE[0] = eTi_KA;
 PFPeakName[0] = new TString("Ti_KA");
 PFPeakE[1] = eCu_KA;
 PFPeakName[1] = new TString("Cu_KA");
 PFPeakE[2] = eCu_KB;
 PFPeakName[2] = new TString("Cu_KB");


//PreCal configuration:
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
 for(int i=0;i<nPFPeaks;i++){AddPeakE[nPeaks]=PFPeakE[i];AddPeakName[nPeaks]=PFPeakName[i];nPeaks++;}
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

 //set PreCal fit function: gaussians + bkg
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
 cout<<" PreCal fit Function is : "<<fstr<<endl;





//sdd loop
//--------
for(int iSDD=0;iSDD<nSDD;iSDD++){
//for(int iSDD=0;iSDD<12;iSDD++){//for testing
 if(writeSDD[iSDD]){
  TH1F* thehisto; 
  thehisto = (TH1F*) hADCgood[iSDD]->Clone("thehisto");thehisto->SetName("thehisto");



  //  --- Peak Finder Calibration --- //
  //----------------------------------//
  cout<<endl<<endl<<"-----PEAK FINDER for SDD "<<iSDD<<" with entries "<<thehisto->GetEntries()<<endl;
  
  //restrict to PeakFinder area:
  thehisto->GetXaxis()->SetRangeUser(xminPeakFinder,xmaxPeakFinder);

  //peak finder config:
  Int_t nfound = 0;
  float thresholdPeakFinder = InitThresholdPeakFinder;
  while(nfound<nPFPeaks){
   nfound = spectrum->Search(thehisto,sigmaPeakFinder,"",thresholdPeakFinder);
   printf("Found %d candidate peaks to fit\n",nfound);
   thresholdPeakFinder = thresholdPeakFinder*.1;//TSpectrum std=0.05. Change it til it finds the peaks
  }

  xpeaks = spectrum->GetPositionX();//array with X-positions of the centroids found by TSpectrum 
  ypeaks = spectrum->GetPositionY();//array with X-positions of the centroids found by TSpectrum 

  //reorder in adc counts and check if compatible with maximum peaks assumption
  themin = 999999.;
  imin = 0;
  for(int i =0;i<nfound;i++){
   //find the smallest, write it in xadc and remove it:
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
  //find also the highest height:
  float highestheight = 0.;
  for(int i =0;i<nfound;i++){
   if(yadc[i]>highestheight) highestheight = yadc[i];
  }

  //print them, now ordered:
  for(int i =0;i<nfound;i++){cout<<" found peak ADC="<<xadc[i]<<" height "<<yadc[i]<<endl;}
  cout<<endl;

  //check if the peaks found are compatible with the assumption:
  //------------------------------------------------------------
  float eDist10 = PFPeakE[1]-PFPeakE[0];
  float eDist21 = PFPeakE[2]-PFPeakE[1];
  float Erelation = eDist21/eDist10;
  float xDist10 = xadc[1]-xadc[0];
  float xDist21 = xadc[2]-xadc[1];
  float ADCrelation = eDist21/eDist10;
  cout<<" Check assumption: Energy relation "<<Erelation<<" vs ADC relation "<<ADCrelation<<endl;


  //Define tolerance parameter
  float tol = InitTolerance; // 5%
  if(fabs(1.-(Erelation/ADCrelation))>tol){
   for(int i=0;i<10;i++){
    cout<<endl<<" tolERROR "<<endl<<endl<<" tolERROR"<<endl;
   }
  }

  // Peak Finder calibration Offset G0PF and Slope GPF
  float Dadc = xadc[0]-xadc[1];
  float De = PFPeakE[0]-PFPeakE[1];
  float GPF = De/Dadc;
  float G0PF = -1.*xadc[0]*GPF+PFPeakE[0];
  cout<<"PeakFinder cal, offset G0PF "<<G0PF<<" slope GPF "<<GPF<<endl;

  // peak finder histograms and function:
  fPeakFinder[iSDD] = new TF1(Form("fPeakFinder%i",iSDD),"pol1",0.,8000.);
  fPeakFinder[iSDD]->SetParameter(0,G0PF);
  fPeakFinder[iSDD]->SetParameter(1,GPF);
  fPeakFinderArray->Add(fPeakFinder[iSDD]);


  
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
    fPre->SetParameter(iInt[i], yadc[0]*sigmaADCguess );//initial value: height of 1st PF peak * sigma
    //cout<<"fPre->SetParameter("<<iInt[i]<<","<<yadc[0]*sigmaADCguess<<");"<<endl;
    fPre->SetParameter(iMean[i],  (PeakE[i]-G0PF)/GPF   );
    //cout<<"fPre->SetParameter("<<iMean[i]<<","<<(PeakE[i]-G0PF)/GPF<<");"<<endl;
    fPre->SetParameter(iSigma[i],  sigmaADCguess   );
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
   thehisto->Fit(fPre,"","0R",xminPre[0],xmaxPre[nPeaks-1]);
 
   //put back original errors:
   for(int ibin=1;ibin<=thehisto->GetNbinsX();ibin++){
    thehisto->SetBinError(ibin,hADCgood[iSDD]->GetBinError(ibin));
   }

   //Fit straight line to the peaks to get PreCal calibration function
   gPreCal[iSDD] = new TGraphErrors();
   gPreCal[iSDD]->SetName(Form("gPreCal%i",iSDD));
   for(int i=0;i<nPeaks;i++){
    gPreCal[iSDD]->SetPoint(i, PeakE[i], fPre->GetParameter(iMean[i]) ); 
    gPreCal[iSDD]->SetPointError(i, 0., fPre->GetParError(iMean[i])); 
   }
   TF1* fline = new TF1("fline","pol1");
   fline->SetParameter(0, -1.*G0PF/GPF);//inital pars from peak finder
   fline->SetParameter(1, 1./GPF);//inital pars from peak finder
   gPreCal[iSDD]->Fit(fline,"0");
   float G0Pre = -1.*fline->GetParameter(0)/fline->GetParameter(1);
   float GPre = 1./fline->GetParameter(1);
   //PreCal histograms and function:
   hPreCal[iSDD] = (TH1F*) thehisto->Clone(Form("hPreCal%i",iSDD));
   hPreCal[iSDD]->SetName(Form("hPreCal%i",iSDD));
   hPreCal[iSDD]->GetYaxis()->SetRangeUser(.0001,highestheight*1.2);
   fPreCal[iSDD] = new TF1(Form("fPreCal%i",iSDD),"pol1",0.,8000.);
   fPreCal[iSDD]->SetParameter(0, G0Pre  );
   fPreCal[iSDD]->SetParameter(1, GPre );
   fPreCalArray->Add(fPreCal[iSDD]);
   //linearity plot:
   gPreCalLin[iSDD] = new TGraphErrors();
   gPreCalLin[iSDD]->SetName(Form("gPreCalLin%i",iSDD));
   for(int i=0;i<nPeaks;i++){
    gPreCalLin[iSDD]->SetPoint(i,PeakE[i],fPre->GetParameter(iMean[i])-fline->Eval(PeakE[i]));
    gPreCalLin[iSDD]->SetPointError(i,0.,fPre->GetParError(iMean[i]));
   }
   //G and G0 plots:
   gPreCalG->SetPoint(ipoint,iSDD,GPre);
   gPreCalG0->SetPoint(ipoint,iSDD,G0Pre);
   ipoint++;

  //clean the mess
   fline->Delete();
  }//!QuickCalib



  thehisto->Delete();

 }//if write
}//sdd loop
}//if(IsCalib)





































// --- WRITE HISTOGRAMS TO FILE ---
//______________________________________________________________________________________________________//
TFile* fout = new TFile(sfileout,"RECREATE");


for(int iSDD=0;iSDD<nSDD;iSDD++){
 if(writeSDD[iSDD]){
  fout->mkdir(Form("SDD%i",iSDD));
  fout->cd(Form("SDD%i",iSDD));
  hADC[iSDD]->GetXaxis()->SetRangeUser(500.,maxadc);
  hADC[iSDD]->Write();
  hADCpre[iSDD]->GetXaxis()->SetRangeUser(500.,maxadc);
  hADCpre[iSDD]->Write();
  //for(int jSDD=0;jSDD<nSDD;jSDD++){
  for(int jSDD=0;jSDD<nSDD+2;jSDD++){
    hADCpre_[iSDD][jSDD]->GetXaxis()->SetRangeUser(500.,maxadc);
    hADCpre_[iSDD][jSDD]->GetYaxis()->SetRangeUser(500.,maxadc);
    hADCpre_[iSDD][jSDD]->Write();
  }
  hADCpreBADtime[iSDD]->GetXaxis()->SetRangeUser(500.,maxadc);
  hADCpreBADtime[iSDD]->GetYaxis()->SetRangeUser(500.,maxadc);
  hADCpreBADtime[iSDD]->Write();
  hADCpreGOODtime[iSDD]->GetXaxis()->SetRangeUser(500.,maxadc);
  hADCpreGOODtime[iSDD]->GetYaxis()->SetRangeUser(500.,maxadc);
  hADCpreGOODtime[iSDD]->Write();
  hADCpreGOODevdiff[iSDD]->GetXaxis()->SetRangeUser(500.,maxadc);
  hADCpreGOODevdiff[iSDD]->GetYaxis()->SetRangeUser(500.,maxadc);
  hADCpreGOODevdiff[iSDD]->Write();
  hADCpreBADevdiff[iSDD]->GetXaxis()->SetRangeUser(500.,maxadc);
  hADCpreBADevdiff[iSDD]->GetYaxis()->SetRangeUser(500.,maxadc);
  hADCpreBADevdiff[iSDD]->Write();
  hADCpre_GOODev_GOODtime[iSDD]->GetXaxis()->SetRangeUser(500.,maxadc);
  hADCpre_GOODev_GOODtime[iSDD]->GetYaxis()->SetRangeUser(500.,maxadc);
  hADCpre_GOODev_GOODtime[iSDD]->Write();
  hADCpre_GOODev_GOODtime1D[iSDD]->Write();
  hADCpre_GOODev_BADtime[iSDD]->GetXaxis()->SetRangeUser(500.,maxadc);
  hADCpre_GOODev_BADtime[iSDD]->GetYaxis()->SetRangeUser(500.,maxadc);
  hADCpre_GOODev_BADtime[iSDD]->Write();
  hADCpre_BADev_GOODtime[iSDD]->GetXaxis()->SetRangeUser(500.,maxadc);
  hADCpre_BADev_GOODtime[iSDD]->GetYaxis()->SetRangeUser(500.,maxadc);
  hADCpre_BADev_GOODtime[iSDD]->Write();
  hADCpre_BADev_BADtime[iSDD]->GetXaxis()->SetRangeUser(500.,maxadc);
  hADCpre_BADev_BADtime[iSDD]->GetYaxis()->SetRangeUser(500.,maxadc);
  hADCpre_BADev_BADtime[iSDD]->Write();
  hTimeDiff[iSDD]->Write();
  hEvDiff[iSDD]->Write();
  hADCgood[iSDD]->Write();
  hADCgoodCurrent[iSDD]->Write();
  hADCgoodCurrent2[iSDD]->Write();
  hADCgoodRate[iSDD]->Write();
  hADCgoodRate2[iSDD]->Write();
  hADCgoodZeroCurrent[iSDD]->Write();
  hADCgoodWithCurrent[iSDD]->Write();
 }//writesdd
}//sdd
//fg->Write();

//go back to main directory:
fout->cd();

//Create also a TObjarray with the hADCgood histo:
TObjArray* hArray = new TObjArray();
for(int iSDD=0;iSDD<nSDD;iSDD++){
 if(writeSDD[iSDD]){
  hArray->Add(hADCgood[iSDD]);
 }
}
hArray->Write("hArray",TObject::kSingleKey);

TObjArray* hArray_trig = new TObjArray();
for(int iSDD=0;iSDD<nSDD;iSDD++){
 if(writeSDD[iSDD]){
  hArray_trig->Add(hADC_trig[iSDD]);
 }
}
hArray_trig->Write("hArray_trig",TObject::kSingleKey);


if(IsCalib){
 fPeakFinderArray->Write("fPeakFinderArray",TObject::kSingleKey);
 
 if(!QuickCalib){
 //PreCal calib:
 TObjArray* hPreCalArray = new TObjArray();
 TObjArray* gPreCalArray = new TObjArray();
 TObjArray* gPreCalLinArray = new TObjArray();
 //TObjArray* fPreCalArray = new TObjArray();
 for(int iSDD=0;iSDD<nSDD;iSDD++){
  if(writeSDD[iSDD]){
   hPreCalArray->Add(hPreCal[iSDD]);
   gPreCalArray->Add(gPreCal[iSDD]);
   gPreCalLinArray->Add(gPreCalLin[iSDD]);
   //fPreCalArray->Add(fPreCal[iSDD]);
  }
 }
 hPreCalArray->Write("hPreCalArray",TObject::kSingleKey);
 gPreCalLinArray->Write("gPreCalLinArray",TObject::kSingleKey);
 fPreCalArray->Write("fPreCalArray",TObject::kSingleKey);
 gPreCalG0->Write();
 gPreCalG->Write();

 }
}

//write rate related plots:
rate_c->Write();

 // DO SUM, other stuff, and WRITE ENERGY plots:
 //---------------------------------------------
 TObjArray* hEarray = new TObjArray();
 TObjArray* hEarray_trig = new TObjArray();
 TObjArray* hEarray_drift = new TObjArray();


 //sum related:
 int MinEntriesToSum = 999; // refered to non-triggered spectra
 int isummed = 0;
 bool summedSDD[nSDD]={};
 TH1F* hSDDsummed = new TH1F("hSDDsummed","",nSDD,0,nSDD);
 TH1F* htotalSDDsummed = new TH1F("htotalSDDsummed","",nSDD,0,nSDD);
 for(int iSDD=0;iSDD<nSDD;iSDD++){
  if(goodcalSDD[iSDD]){
   if(hE[iSDD]->GetEntries()>MinEntriesToSum){//SDD considered summed? decide in base of non-trigg spectr
    isummed++;
    summedSDD[iSDD]=1;
    hSDDsummed->Fill(iSDD);
   }
   hEarray->Add(hE[iSDD]);
   hEarray_trig->Add(hE_trig[iSDD]);
   hEarray_drift->Add(hEdrift_trig[iSDD]);
  }
 }//iSDD

hEarray->Write("hEarray",TObject::kSingleKey);
hEarray_trig->Write("hEarray_trig",TObject::kSingleKey);
hEarray_drift->Write("hEarray_drift",TObject::kSingleKey);


hEsum->Write();
hEsum_trig->Write();
hEsum_trig_tdc->Write();
hEsum_trig_tdc_drift->Write();
hEsum_trig_mip->Write();
hEsum_trig_mip_nodrift->Write();



htotalSDDsummed->Fill(isummed);
cout<<isummed<<" summed sdd's"<<endl;
hSDDsummed->Write();
htotalSDDsummed->Write();
hEsumCP->Write();
hEsumCP_trig->Write();
hBuffertime->Write();
hCPtime->Write();

hktwide->Write();
hkt->Write();
hktrot->Write();
hdrift->Write();
hEdrift->Write();

fout->Close();

}
