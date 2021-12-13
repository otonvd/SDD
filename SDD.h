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
   SDD(char *fn, char *fncalib);
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
   

//additional functions go here:
//-----------------------------
private:
bool ThereIsPost(UShort_t thesdd, Short_t theadc, int postsdd,int postadc,int rmin=99999, int rmax=-99999, int totmin =-99999);
bool ThereIsPre(UShort_t thesdd, Short_t theadc, int presdd,int preadc,int rmin=99999, int rmax=-99999, int totmin=-99999 );
bool IsInBus(int thesdd, int thebus);
bool IsGood(int theadc, int preadc);
bool IsSignal(int theadc);


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
 TObjArray* CalibArray;

 static const int nBUS = 6;//
 static const int nSDD = 65; //last SDD #64

};

#endif

#ifdef SDD_cxx
//SDD::SDD(TTree *tree) : fChain(0) {
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
//   if (tree == 0) {
//      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("20200117_1240_0117_1414_sf1sf3_Dafne+xray_bottom_25kv_100ua.root");
//      if (!f || !f->IsOpen()) {
SDD::SDD(char *fn, char *fncalib){
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
  if(sfilein.Contains("xray")){
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
  string fout_post;
  fout_post  = fn;
  string fout = "foutSDD_";
  fout += fout_post;
  sfileout = (TString) fout.c_str();
  cout<<" Output File "<<sfileout.Data()<<endl;
  TFile *f = new TFile(sfilein);
  f->GetObject("ft10",tree);
  Init(tree);
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
#endif // #ifdef SDD_cxx
