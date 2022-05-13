
//globals
const static int maxfiles = 99;
TFile* file[maxfiles];
float lum[maxfiles];
int whichData[maxfiles];
float kaons[maxfiles];
float dt[maxfiles];
float dtbkg[maxfiles];
float nSDD[maxfiles];
float Area[maxfiles];
float deg[maxfiles];
float Signal[maxfiles];
float Bkg[maxfiles];
//results:
float K_perpb[maxfiles];      // Kaons /cm2/pb-1
float TrigHits_percm2K[maxfiles];// Drift time peak int /cm2/K
float DriftSB[maxfiles];      // Drift time peak S/B
float Signal_percm2pb[maxfiles]; // KHe Kalpha /cm2/pb-1
float Signal_percm2K[maxfiles];  // KHe Kalpha /cm2/K
float SB[maxfiles];           // KHe Kalpha S/B


// MAIN
void results(){
 float GetKaons(int ii);
 float GetDriftTime(int ii);
 float GetDriftBkg(int ii);
 float GetNSDD(int ii);
 void FillSignal(int ii);

 int nfiles = 0;
 TGraphErrors* gK = new TGraphErrors();   
 gK->SetMarkerStyle(20);
 gK->GetXaxis()->SetTitle("degrader");
 gK->GetYaxis()->SetTitle("Kaons /pb-1");

 gK->GetXaxis()->SetTitleSize(0.05);
 gK->GetXaxis()->SetLabelSize(0.05);
 gK->GetYaxis()->SetTitleSize(0.06);
 gK->GetYaxis()->SetTitleOffset(0.80);
 gK->GetYaxis()->SetLabelSize(0.05);

 TGraphErrors* gD = new TGraphErrors();
 gD->SetMarkerStyle(20);
 gD->GetXaxis()->SetTitle("degrader");
 gD->GetYaxis()->SetTitle("SDD trigger hits /cm2/Kaon");

 gD->GetXaxis()->SetTitleSize(0.05);
 gD->GetXaxis()->SetLabelSize(0.05);
 gD->GetYaxis()->SetTitleSize(0.06);
 gD->GetYaxis()->SetTitleOffset(0.80);
 gD->GetYaxis()->SetLabelSize(0.05);


 TGraphErrors* gDSB = new TGraphErrors();
 gDSB->SetMarkerStyle(20);
 gDSB->GetXaxis()->SetTitle("degrader");
 gDSB->GetYaxis()->SetTitle("Drift Time S/B");

 gDSB->GetXaxis()->SetTitleSize(0.05);
 gDSB->GetXaxis()->SetLabelSize(0.05);
 gDSB->GetYaxis()->SetTitleSize(0.06);
 gDSB->GetYaxis()->SetTitleOffset(0.80);
 gDSB->GetYaxis()->SetLabelSize(0.05);

 TGraphErrors* gSpb = new TGraphErrors();
 gSpb->SetMarkerStyle(20);
 gSpb->GetXaxis()->SetTitle("degrader");
 gSpb->GetYaxis()->SetTitle("KHe Kalpha signal /cm2/pb-1");

 gSpb->GetXaxis()->SetTitleSize(0.05);
 gSpb->GetXaxis()->SetLabelSize(0.05);
 gSpb->GetYaxis()->SetTitleSize(0.06);
 gSpb->GetYaxis()->SetTitleOffset(0.80);
 gSpb->GetYaxis()->SetLabelSize(0.05);

 TGraphErrors* gSpb_deg = (TGraphErrors*)gSpb->Clone("gSpb_deg");
 gSpb_deg->GetXaxis()->SetTitle("Degrader thickness (um)");

 gSpb_deg->GetXaxis()->SetTitleSize(0.05);
 gSpb_deg->GetXaxis()->SetLabelSize(0.05);
 gSpb_deg->GetYaxis()->SetTitleSize(0.06);
 gSpb_deg->GetYaxis()->SetTitleOffset(0.80);
 gSpb_deg->GetYaxis()->SetLabelSize(0.05);

 TGraphErrors* gSK = new TGraphErrors();
 gSK->SetMarkerStyle(20);
 gSK->GetXaxis()->SetTitle("degrader");
 gSK->GetYaxis()->SetTitle("KHe Kalpha signal /cm2/Kaon");

 gSK->GetXaxis()->SetTitleSize(0.05);
 gSK->GetXaxis()->SetLabelSize(0.05);
 gSK->GetYaxis()->SetTitleSize(0.06);
 gSK->GetYaxis()->SetTitleOffset(0.80);
 gSK->GetYaxis()->SetLabelSize(0.05);

 TGraphErrors* gSK_deg = (TGraphErrors*)gSK->Clone("gSK_deg");
 gSK_deg->GetXaxis()->SetTitle("Degrader thickness (um)");

 gSK_deg->GetXaxis()->SetTitleSize(0.05);
 gSK_deg->GetXaxis()->SetLabelSize(0.05);
 gSK_deg->GetYaxis()->SetTitleSize(0.06);
 gSK_deg->GetYaxis()->SetTitleOffset(0.80);
 gSK_deg->GetYaxis()->SetLabelSize(0.05);

 TGraphErrors* gSB = new TGraphErrors();
 gSB->SetMarkerStyle(20);
 gSB->GetXaxis()->SetTitle("degrader");
 gSB->GetYaxis()->SetTitle("KHe Kalpha S/B");

 gSB->GetXaxis()->SetTitleSize(0.05);
 gSB->GetXaxis()->SetLabelSize(0.05);
 gSB->GetYaxis()->SetTitleSize(0.06);
 gSB->GetYaxis()->SetTitleOffset(0.80);
 gSB->GetYaxis()->SetLabelSize(0.05);

 TGraphErrors* gSB_deg = (TGraphErrors*)gSB->Clone("gSB_deg");
 gSB_deg->GetXaxis()->SetTitle("Degrader thickness (um)");

 gSB_deg->GetXaxis()->SetTitleSize(0.05);
 gSB_deg->GetXaxis()->SetLabelSize(0.05);
 gSB_deg->GetYaxis()->SetTitleSize(0.06);
 gSB_deg->GetYaxis()->SetTitleOffset(0.80);
 gSB_deg->GetYaxis()->SetLabelSize(0.05);



 // DEFINE HERE DATA FILES
 //------------------------
 file[nfiles] = TFile::Open("foutSDD_S2He_1-2.root");
 whichData[nfiles] = 1;
 lum[nfiles] = 5.716;
 deg[nfiles] = 475;
 nfiles++;

 file[nfiles] = TFile::Open("foutSDD_S2He1_1-2.root");
 whichData[nfiles] = 2;
 lum[nfiles] = 4.32;
 deg[nfiles] = 350;
 nfiles++;

 file[nfiles] = TFile::Open("foutSDD_S2He2_1-2.root");
 whichData[nfiles] = 2;
 lum[nfiles] = 5.553;
 deg[nfiles] = 600;
 nfiles++;

 file[nfiles] = TFile::Open("foutSDD_S2He3_1-2.root");
 whichData[nfiles] = 2;
 lum[nfiles] = 6.503;
 deg[nfiles] = 475+10;//add file to be visible in graphic
 nfiles++;
 //----------------------------------------


 for(int i=0;i<nfiles;i++){
  kaons[i] = GetKaons(i);
  dt[i] = GetDriftTime(i);
  dtbkg[i] = GetDriftBkg(i);
  nSDD[i] = GetNSDD(i);
  Area[i] = nSDD[i]*0.64;
  FillSignal(i);

  //calc results:
  K_perpb[i] = kaons[i]/lum[i];
  gK->SetPoint(i,i+1,K_perpb[i]);
  gK->SetPointError(i,0., sqrt(kaons[i])/lum[i]);

  TrigHits_percm2K[i] = dt[i]/Area[i]/kaons[i];
  gD->SetPoint(i,i+1,TrigHits_percm2K[i]);
  gD->SetPointError(i,0.,sqrt(dt[i])/Area[i]/kaons[i] );

  DriftSB[i] = dt[i]/dtbkg[i];
  gDSB->SetPoint(i,i+1,DriftSB[i]);
  float rele = sqrt( pow( sqrt(dt[i])/dt[i]  ,2)  + pow( sqrt(dtbkg[i])/dtbkg[i] ,2) );
  gDSB->SetPointError(i,0.,rele*DriftSB[i]);

  Signal_percm2pb[i] = Signal[i]/Area[i]/lum[i];
  gSpb->SetPoint(i,i+1,Signal_percm2pb[i]);
  gSpb->SetPointError(i,0.,sqrt(Signal[i])/Area[i]/lum[i]);
  gSpb_deg->SetPoint(i,deg[i],Signal_percm2pb[i]);
  gSpb_deg->SetPointError(i,0.,sqrt(Signal[i])/Area[i]/lum[i]);


  Signal_percm2K[i] = Signal[i]/Area[i]/kaons[i];
  gSK->SetPoint(i,i+1,Signal_percm2K[i]);
  gSK->SetPointError(i,0., sqrt(Signal[i])/Area[i]/kaons[i]);
  gSK_deg->SetPoint(i,deg[i],Signal_percm2K[i]);
  gSK_deg->SetPointError(i,0., sqrt(Signal[i])/Area[i]/kaons[i]);

  SB[i] = Signal[i]/Bkg[i];
  gSB->SetPoint(i,i+1,SB[i]);
  rele = sqrt( pow( sqrt(Signal[i])/Signal[i]  ,2)  + pow( sqrt(Bkg[i])/Bkg[i] ,2) );
  gSB->SetPointError(i,0.,rele*SB[i]);
  gSB_deg->SetPoint(i,deg[i],SB[i]);
  gSB_deg->SetPointError(i,0.,rele*SB[i]);

  
 }

 TCanvas* c1 = new TCanvas();
 c1->Divide(3,3);
 c1->cd(1);
 gK->Draw("AP");
 c1->cd(4);
 gD->Draw("AP");
 c1->cd(7);
 gDSB->Draw("AP");

 c1->cd(2);
 gSpb->Draw("AP");
 c1->cd(5);
 gSK->Draw("AP");
 c1->cd(8);
 gSB->Draw("AP");

 c1->cd(3);
 gSpb_deg->Draw("AP");
 c1->cd(6);
 gSK_deg->Draw("AP");
 c1->cd(9);
 gSB_deg->Draw("AP");

}



//_____________________
float GetKaons(int ii){
 TObjArray* kt = (TObjArray*)file[ii]->Get("KaonTrigger")->Clone("kt");
 TH1F* hk = (TH1F*)kt->FindObject("hktrot")->Clone("hk");
 float tdcmin1,tdcmax1,tdcmin2,tdcmax2;
 int bmin1,bmax1,bmin2,bmax2;
 int WhichData=whichData[ii];
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
 }
 bmin1=hk->FindBin(tdcmin1);
 bmax1=hk->FindBin(tdcmax1);
 bmin2=hk->FindBin(tdcmin2);
 bmax2=hk->FindBin(tdcmax2);
 return hk->Integral(bmin1,bmax1)+hk->Integral(bmin2,bmax2);
}
//_____________________
float GetDriftTime(int ii){
 TObjArray* kt = (TObjArray*)file[ii]->Get("KaonTrigger")->Clone("kt");
 TH1F* hd = (TH1F*)kt->FindObject("hDriftPeak")->Clone("hd");
 TH1F* hdbkg = (TH1F*)kt->FindObject("hDriftBkg")->Clone("hdbkg");
 float tot = hd->Integral(1, hd->GetNbinsX());
 float bkg = hdbkg->Integral(1, hdbkg->GetNbinsX());
 return tot-bkg;
}
//_____________________
float GetDriftBkg(int ii){
 TObjArray* kt = (TObjArray*)file[ii]->Get("KaonTrigger")->Clone("kt");
 TH1F* hdbkg = (TH1F*)kt->FindObject("hDriftBkg")->Clone("hdbkg");
 return hdbkg->Integral(1, hdbkg->GetNbinsX());
}
//_____________________
float GetNSDD(int ii){
 TDirectoryFile* Numbers = (TDirectoryFile*)file[ii]->GetDirectory("Numbers");
 TGraph* gDPI = (TGraph*)Numbers->Get("gDriftPeakInt")->Clone("gDPI"); // Use DriftPeakInt to normalize nSDD !!!
 TGraph* gSDD = (TGraph*)Numbers->Get("gSDDon")->Clone("gSDD"); 
 float nsdd = 0.;
 int np = gSDD->GetN();
 float Norm = 0.;
 double x,DPI,y;
 for(int ip = 0;ip<np;ip++){
  gDPI->GetPoint(ip,x,y);
  Norm += y;
 }
 for(int ip = 0;ip<np;ip++){
  gDPI->GetPoint(ip,x,DPI);
  gSDD->GetPoint(ip,x,y);
  nsdd += (y*DPI)/Norm;
 }
 return nsdd;
}
//_____________________
void FillSignal(int ii){
 TH1F* hS = (TH1F*)file[ii]->Get("hEgoodsum_trig")->Clone("hS");
 // (quick) FIT K4He Lalpha
 float xmin = 6100.;
 float xmax = 7500.;
 TF1* fheb = new TF1("fheb","gaus(0)+pol0(3)",xmin,xmax);
 float xmean = 6470.;
 fheb->SetParameter(0,hS->GetBinContent(hS->FindBin(xmean)));
 fheb->SetParameter(1,xmean);
 float xsigma = 110.;
 fheb->SetParameter(2,xsigma);
 float pol0 = hS->GetBinContent(hS->FindBin(xmin));
 fheb->SetParameter(3,pol0);
 hS->Fit(fheb,"R","",xmin,xmax);
 xmean = fheb->GetParameter(1);
 xsigma = fheb->GetParameter(2);
 pol0 = fheb->GetParameter(3);
 float nsigmas = 3;
 float xintmin = xmean-nsigmas*xsigma;
 float xintmax = xmean+nsigmas*xsigma;
 int ibinmin = hS->FindBin(xintmin);
 int ibinmax = hS->FindBin(xintmax);
 //NEW, takes signal as the Integral of the histo minus the integral of the bkg
 Signal[ii] = hS->Integral(ibinmin,ibinmax);
 Bkg[ii] = (ibinmax-ibinmin+1)*pol0;
 Signal[ii] = Signal[ii]-Bkg[ii];
 return;
}
