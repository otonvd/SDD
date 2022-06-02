//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Jan 25 14:44:15 2021 by ROOT version 6.20/08
// from TTree ft10/SIDDHARTA-2 TTree
// found on file: 20200117_1240_0117_1414_sf1sf3_Dafne+xray_bottom_25kv_100ua.root
//////////////////////////////////////////////////////////

#ifndef SDD_h
#define SDD_h

#include <iostream>

#include <TROOT.h>
#include <TF1.h>
#include <TH2.h>
#include <TChain.h>
#include <TFile.h>
#include <TLine.h>
#include <TGraphErrors.h>






// Header file for the classes stored in the TTree if any.

class SDD {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   static const int MAXHITS = 99999;
   Int_t           buf;
   Short_t         kt[4];
   Int_t           nhits;
   Int_t           bus[MAXHITS];   //[nhits]
   UShort_t        evnr[MAXHITS];   //[nhits]
   UShort_t        ht[MAXHITS];   //[nhits]
   UShort_t        trigg[MAXHITS];   //[nhits]
   UShort_t        sdd[MAXHITS];   //[nhits]
   Short_t         adc[MAXHITS];   //[nhits]
   Short_t         drift[MAXHITS];   //[nhits]
   Int_t           date;
   Short_t         ie;
   Short_t         ip;
   Short_t         dum;

   // List of branches
   TBranch        *b_buf;   //!
   TBranch        *b_kt;   //!
   TBranch        *b_nhits;   //!
   TBranch        *b_bus;   //!
   TBranch        *b_evnr;   //!
   TBranch        *b_ht;   //!
   TBranch        *b_trigg;   //!
   TBranch        *b_sdd;   //!
   TBranch        *b_adc;   //!
   TBranch        *b_drift;   //!
   TBranch        *b_date;   //!
   TBranch        *b_ie;   //!
   TBranch        *b_ip;   //!
   TBranch        *b_dum;   //!

   //SDD(TTree *tree=0);
   //SDD(char *fn, char *fncalib, int fWhichData, float fLuminositypb);
   SDD(TChain* inputChain, const char *fn,const char *fncalib, int fWhichData, float fLuminositypb);

   virtual ~SDD();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
//   virtual void     GetCalib(TString *sfcalib);
   virtual void     Loop(Int_t InjectionFlag = 2);
   virtual void     SetQuick();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
virtual void FUNCTION_NUMBERS();
   

//additional functions go here:
//-----------------------------
private:
bool ThereIsPost(UShort_t thesdd, Short_t theadc, int postsdd,int postadc,int rmin=99999, int rmax=-99999, int totmin =-99999);
bool ThereIsPre(UShort_t thesdd, Short_t theadc, int presdd,int preadc,int rmin=99999, int rmax=-99999, int totmin=-99999 );
int where(int thebus, int thesdd); //position boost/antiboost, low/mid/up

//additional variables go here:
//-----------------------------
 static const int nbusesMAX = 10;
 int firstSDD[nbusesMAX];
 int lastSDD[nbusesMAX];
 TF1* fpre1;
 TF1* fpre2;
 TString sfilein;
 TString sfilecalib;
 TString sfileout;
 bool QuickCalib = false;
 bool IsCalib = false;
 int WhichData = 1; // 0=siddhartino, 1=S2He
 float Luminositypb = 99999.; // absurd intialization
 TObjArray* CalibArray;

 static const int nBUS = 7;//
 static const int nSDD = 65; //last SDD #64

 //DEFINITION OF HISTOGRAMS and other
 TH1F* hEsum;
 TH1F* hEsum_trig;
 TH1F* hEgoodsum;
 TH1F* hEgoodsum_trig;
 TH1F* hEgoodsum_bkg;
 TH1F* hE[nBUS][nSDD]; // Calibrated energy spectrum
 TH1F* hEgood[nBUS][nSDD]; // Calibrated energy spectrum
 TH1F* hE_trig[nBUS][nSDD]; // Calibrated energy spectrum
 TH1F* hEgood_trig[nBUS][nSDD]; // Calibrated energy spectrum

 bool GoodCal[nBUS][nSDD] = {};

 // DEFINITION OF GRAPHS AND OTHER FOR FUNCTION-LIKE CODE:
 float nall,ntrig,nbkg,nsig;
 int nSDDon, nSDDGon, NGoodCal;
 float HeYieldperSDD,SDDcm2,HeYieldpercm2,TotHeYield,bkgint,Kint;
 TGraph* gTotHeYield;
 TGraph* gHeYieldperSDD;
 TH1F* hpercm2;
 TGraph* gHeYieldpercm2;
 float MK, HeYieldpercm2MK, HeYieldpercm2pb;
 TH1F* hpercm2MK;
 TGraph* gHeYieldpercm2MK;
 TH1F* hpercm2pb;
 TGraph* gHeYieldpercm2pb;
 bool IsGoodSDD[nBUS][nSDD];
 float INCLint[nBUS][nSDD] = {};
 float TRIGint[nBUS][nSDD] = {};
 float SIGint[nBUS][nSDD] = {};
 static const int nzones = 6; 
 TGraphErrors* gINCL[nzones];  // 0,1,2= low,mid,up BOOST, 3,4,5=low,mid,up ANTIBOOST 
 TGraphErrors* gINCLbad[nzones];  // for bad sdds
 TGraphErrors* gTRIG[nzones];  // 0,1,2= low,mid,up BOOST, 3,4,5=low,mid,up ANTIBOOST 
 TGraphErrors* gTRIGbad[nzones];  // for bad sdds
 TGraphErrors* gSIG[nzones];  // 0,1,2= low,mid,up BOOST, 3,4,5=low,mid,up ANTIBOOST
 TGraphErrors* gSIGbad[nzones];  // for bad sdds
 TGraph* lINCL[nzones]; 
 TGraph* lTRIG[nzones]; 
 TGraph* lSIG[nzones]; 


};

#endif

#ifdef SDD_cxx
//SDD::SDD(TTree *tree) : fChain(0) {
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
//   if (tree == 0) {
//      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("20200117_1240_0117_1414_sf1sf3_Dafne+xray_bottom_25kv_100ua.root");
//      if (!f || !f->IsOpen()) {
//SDD::SDD(char *fn, char *fncalib, int fWhichData, float fLuminositypb ){
SDD::SDD(TChain* inputChain, const char *fn,const char *fncalib, int fWhichData, float fLuminositypb){

  TTree *tree = 0;
//  sfilein = (TString) fn;
//  sfilecalib = (TString) fncalib;
  string fin_pre = "../";
  string fin = fn;
  fin = fin_pre + fin;
  sfilein = (TString) fin.c_str();
  string fincalib = fncalib;
  //fincalib = fin_pre + fincalib;
  sfilecalib = (TString) fincalib.c_str();
  cout<<" Input File "<<sfilein.Data()<<endl;
  if(sfilein.Contains("xray")||sfilein.Contains("CALIB")||sfilein.Contains("calib")){
   IsCalib = true;
   cout<<endl<<" Contains xray -> CALIBRATION WILL BE PERFORMED "<<endl<<endl;
   CalibArray = new TObjArray();
  }else{
   // GET CALIBRATION
   //=================
   cout<<" Calib File "<<sfilecalib.Data()<<endl;
   TFile *fcalib = new TFile(sfilecalib);
   if(fcalib->GetListOfKeys()->Contains("PRECAL_G_G0")){
    cout<<" Pre-Calibration found "<<endl;
    CalibArray = (TObjArray*) fcalib->Get("PRECAL_G_G0")->Clone("CalibArray");
// }else if(fcalib->GetListOfKeys()->Contains("PeakFinder_G_G0")){
//  cout<<" Peak-Finder calibration found "<<endl;
//  CalibArray = (TObjArray*) fcalib->Get("PeakFinder_G_G0")->Clone("CalibArray");
   }
  }

  //Define output file:
  string fout_post;
  fout_post  = fn;
  string fout = "foutSDD_";
  fout += fout_post;
  string fout_postpost = ".root";
  fout += fout_postpost;
  sfileout = (TString) fout.c_str();
  cout<<" Output File "<<sfileout.Data()<<endl;

  //get tree
  //TFile *f = new TFile(sfilein);//old
  //f->GetObject("ft10",tree);//old
  tree = inputChain;
  Init(tree);

  //at the end set WhichData and Luminosity too:
  WhichData = fWhichData;
  if(fLuminositypb>0.&&fLuminositypb<99999.) Luminositypb = fLuminositypb;
}

SDD::~SDD()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t SDD::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t SDD::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void SDD::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("buf", &buf, &b_buf);
   fChain->SetBranchAddress("kt", kt, &b_kt);
   fChain->SetBranchAddress("nhits", &nhits, &b_nhits);
   fChain->SetBranchAddress("bus", bus, &b_bus);
   fChain->SetBranchAddress("evnr", evnr, &b_evnr);
   fChain->SetBranchAddress("ht", ht, &b_ht);
   fChain->SetBranchAddress("trigg", trigg, &b_trigg);
   fChain->SetBranchAddress("sdd", sdd, &b_sdd);
   fChain->SetBranchAddress("adc", adc, &b_adc);
   fChain->SetBranchAddress("drift", drift, &b_drift);
   fChain->SetBranchAddress("date", &date, &b_date);
   fChain->SetBranchAddress("ie", &ie, &b_ie);
   fChain->SetBranchAddress("ip", &ip, &b_ip);
   fChain->SetBranchAddress("dum", &dum, &b_dum);
   Notify();
}

//void SDD::GetCalib(TString *sfcalib){
//  TFile *fcalib = new TFile(sfcalib);
//}


Bool_t SDD::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}


void SDD::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t SDD::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}

void SDD::SetQuick(){
 QuickCalib = true;
 if(QuickCalib)cout<<endl<<"  QuickCalib-> Only quick calib with Peak finder"<<endl<<endl;
 return;
}



int SDD::where(int thebus, int thesdd){
 //position boost/antiboost, low/mid/up
   bool isboost = 0;
   if(thebus>3) isboost = 1;
   int lmu = 0 ;//0=low, 1=mid, 2=up
   if(thesdd==2) lmu=1;
   if(thesdd==8) lmu=1;
   if(thesdd==18) lmu=1;
   if(thesdd==24) lmu=1;
   if(thesdd==34) lmu=1;
   if(thesdd==40) lmu=1;
   if(thesdd==50) lmu=1;
   if(thesdd==56) lmu=1;
   if(thesdd==9) lmu=1;
   if(thesdd==15) lmu=1;
   if(thesdd==25) lmu=1;
   if(thesdd==31) lmu=1;
   if(thesdd==41) lmu=1;
   if(thesdd==47) lmu=1;
   if(thesdd==57) lmu=1;
   if(thesdd==63) lmu=1;
   if(thesdd==18) lmu=1;
   if(thesdd==12) lmu=2;
   if(thesdd==13) lmu=2;
   if(thesdd==28) lmu=2;
   if(thesdd==29) lmu=2;
   if(thesdd==44) lmu=2;
   if(thesdd==45) lmu=2;
   if(thesdd==60) lmu=2;
   if(thesdd==61) lmu=2;
   if(thesdd==11) lmu=2;
   if(thesdd==14) lmu=2;
   if(thesdd==27) lmu=2;
   if(thesdd==30) lmu=2;
   if(thesdd==43) lmu=2;
   if(thesdd==46) lmu=2;
   if(thesdd==59) lmu=2;
   if(thesdd==62) lmu=2;
   if(thesdd==10) lmu=2;
   if(thesdd==16) lmu=2;
   if(thesdd==26) lmu=2;
   if(thesdd==62) lmu=2;
   if(thesdd==42) lmu=2;
   if(thesdd==48) lmu=2;
   if(thesdd==58) lmu=2;
   if(thesdd==64) lmu=2;
   if(!isboost) lmu= lmu+3;// 012 lowmidup boost, 345 lowmidup antiboost
   return  lmu;
}


#endif // #ifdef SDD_cxx
