//C, C++
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

#include <time.h>

using namespace std;

Int_t plots(){
  //
  TString fileN;
  //
  fileN = "./polygon_chord.root";
  //
  TFile *f1 = new TFile(fileN.Data());

  TGraph *gr_lst = (TGraph*)f1->Get("gr_lst");
  TGraph *gr_intersection = (TGraph*)f1->Get("gr_intersection");
  
  gr_lst->SetLineColor(kBlack);
  gr_lst->SetLineWidth(2);
  gr_lst->SetMarkerColor(kRed + 2);
  gr_lst->SetMarkerStyle(20);
  //
  gr_intersection->SetLineColor(kMagenta + 2);
  gr_intersection->SetLineWidth(3.0);
  gr_intersection->SetMarkerColor(kMagenta + 2);
  gr_intersection->SetMarkerStyle(21);
  /*
  //
  gr_ampl->SetLineColor(kBlack);
  gr_ampl->SetLineWidth(3.0);
  gr_ampl->SetMarkerColor(kBlack);
  gr_ampl->SetMarkerStyle(18);
  */
  //
  //
  
  //
  //
  TMultiGraph *mg = new TMultiGraph();
  mg->Add(gr_lst);
  mg->Add(gr_intersection);
  //
  gStyle->SetPalette(1);
  gStyle->SetFrameBorderMode(0);
  gROOT->ForceStyle();
  gStyle->SetStatColor(kWhite);
  //gStyle->SetOptStat(kFALSE);
  //
  TCanvas *c1 = new TCanvas("c1",fileN.Data(),10,10,1000,1000);
  mg->Draw("apl");  
  //mg->GetYaxis()->SetTitle("Term value");
  //mg->GetXaxis()->SetTitle("Term ID");
  //nameOutGifPdf += ".gif";
  //nameOutGifPdf += ".pdf";
  //cout<<"nameOutGifPdf "<<nameOutGifPdf<<endl;
  //c1->SaveAs(nameOutGifPdf);
  TLegend *leg = new TLegend(0.6,0.6,0.9,0.9,"","brNDC");
  leg->AddEntry(gr_lst, "lst", "apl");
  //leg->AddEntry(gr_b, "b", "apl");
  //leg->AddEntry(gr_ampl, "Amplitude", "apl");
  //leg->Draw();
  //
  //
  return 0;  
}
