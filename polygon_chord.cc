//root
#include "TROOT.h"
#include "TString.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TRandom3.h"
#include "TGraph.h"
#include "TStyle.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TVector2.h"

//C, C++
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct fourier_str {
  Double_t a0;
  vector<Double_t> an;
  vector<Double_t> bn;
  fourier_str(){
    a0 = 0.0;
  };
  void print_info(){
    cout<<"a0 = "<<setw(10)<<a0<<endl;
    cout<<"N  = "<<setw(10)<<an.size()<<endl;
    for(unsigned int i = 0; i< an.size(); i++){
      cout<<"an = "<<setw(20)<<an.at(i)
	  <<"  bn = "<<setw(20)<<bn.at(i)<<endl;
    }
  };
};

TGraph* meander(TString name_title = "gr_meander", Int_t np = 10, Double_t ampl = 1.0);
TGraph* box(TString name_title = "gr_box", Int_t np = 200, Double_t side = 300.0);
TGraph* hexagon(TString name_title = "gr_hexagon", Int_t np = 200, Double_t side = 151.0);
void get_fourier_series_spectrum(TGraph *gr, fourier_str &fourier_spectrum, Int_t N = 10);
Double_t fourier_spectrum_numerical_integral(TGraph *gr, Int_t fourier_n = 0, Int_t key = 0);
TGraph *reco_from_fourier_spectrum(TString name_title, Double_t x_min, Double_t x_max, Int_t np, fourier_str &fourier_spectrum, unsigned int N, Double_t threshold = 0.0);
TGraph *get_xy_graph_from_phiR_graph(TGraph *gr);
TGraph *get_phiR_graph_from_xy_graph(TGraph *gr);
TGraph *read_data(TString fname, TString name_title, Int_t np, bool if_true_lst_mirrors = false);
TGraph *read_data_read_xy(TString fname, TString name_title);
TGraph *interpolate_segment_graph(TGraph *gr, TString name_title, Int_t np = 100);
void get_fourier_spectrum_graph(TGraph *gr_a, TGraph *gr_b, TGraph *gr_ampl, fourier_str &fourier_spectrum);
Int_t lst();
Int_t camera();
Int_t mirror_hexagon();

Int_t main(){
  return lst();
  //return camera();
  //return mirror_hexagon();
}

Int_t mirror_hexagon(){
  //
  TGraph *gr_hexagon = hexagon("gr_hexagon", 200, 151.0);
  //
  fourier_str fourier_spectrum;
  get_fourier_series_spectrum(gr_hexagon, fourier_spectrum, 30);
  cout<<"get_fourier_series_spectrum"<<endl;
  //fourier_spectrum.print_info();
  TGraph *gr_a = new TGraph();
  TGraph *gr_b = new TGraph();
  TGraph *gr_ampl = new TGraph();
  gr_a->SetNameTitle("gr_a", "gr_a");
  gr_b->SetNameTitle("gr_b", "gr_b");
  gr_ampl->SetNameTitle("gr_ampl", "gr_ampl");
  //
  get_fourier_spectrum_graph(gr_a, gr_b, gr_ampl, fourier_spectrum);
  //
  //
  TGraph *gr_reco_0coef = reco_from_fourier_spectrum("gr_reco_0coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 10000, fourier_spectrum, 0);
  TGraph *gr_reco_6coef = reco_from_fourier_spectrum("gr_reco_6coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 10000, fourier_spectrum, 6);
  TGraph *gr_reco_12coef = reco_from_fourier_spectrum("gr_reco_12coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 10000, fourier_spectrum, 12);
  TGraph *gr_reco_18coef = reco_from_fourier_spectrum("gr_reco_18coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 10000, fourier_spectrum, 18);
  TGraph *gr_reco_24coef = reco_from_fourier_spectrum("gr_reco_24coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 10000, fourier_spectrum, 24);
  TGraph *gr_reco_30coef = reco_from_fourier_spectrum("gr_reco_30coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 10000, fourier_spectrum, 30);
  //----------------------
  TFile* rootFile = new TFile("fourier_series_hexagon.root", "RECREATE", " Histograms", 1);
  rootFile->cd();
  if (rootFile->IsZombie()){
    cout<<"  ERROR ---> file "<<"fourier_series_box.root"<<" is zombi"<<endl;
    assert(0);
  }
  else
    cout<<"  Output Histos file ---> "<<"fourier_series_box.root"<<endl;
  //----------------------
  //
  //
  gr_hexagon->Write();
  get_xy_graph_from_phiR_graph(gr_hexagon)->Write();
  //
  gr_a->Write();
  gr_b->Write();
  gr_ampl->Write();
  gr_reco_0coef->Write();
  gr_reco_6coef->Write();
  gr_reco_12coef->Write();
  gr_reco_18coef->Write();
  gr_reco_24coef->Write();
  gr_reco_30coef->Write();
  get_xy_graph_from_phiR_graph(gr_reco_0coef)->Write();
  get_xy_graph_from_phiR_graph(gr_reco_6coef)->Write();
  get_xy_graph_from_phiR_graph(gr_reco_12coef)->Write();
  get_xy_graph_from_phiR_graph(gr_reco_18coef)->Write();
  get_xy_graph_from_phiR_graph(gr_reco_24coef)->Write();
  get_xy_graph_from_phiR_graph(gr_reco_30coef)->Write();
  //
  rootFile->Close();
  //----------------------
  return 0;
}

Int_t camera(){
  //
  TGraph *gr_box = box("gr_box", 200, 300.0);
  TGraph *gr_hexagon = hexagon("gr_hexagon", 200, 151.0);
  //
  fourier_str fourier_spectrum;
  get_fourier_series_spectrum(gr_box, fourier_spectrum, 30);
  cout<<"get_fourier_series_spectrum"<<endl;
  //fourier_spectrum.print_info();
  TGraph *gr_a = new TGraph();
  TGraph *gr_b = new TGraph();
  TGraph *gr_ampl = new TGraph();
  gr_a->SetNameTitle("gr_a", "gr_a");
  gr_b->SetNameTitle("gr_b", "gr_b");
  gr_ampl->SetNameTitle("gr_ampl", "gr_ampl");
  //
  get_fourier_spectrum_graph(gr_a, gr_b, gr_ampl, fourier_spectrum);
  //
  //
  TGraph *gr_reco_0coef = reco_from_fourier_spectrum("gr_reco_0coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 10000, fourier_spectrum, 0);
  TGraph *gr_reco_4coef = reco_from_fourier_spectrum("gr_reco_4coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 10000, fourier_spectrum, 4);
  TGraph *gr_reco_8coef = reco_from_fourier_spectrum("gr_reco_8coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 10000, fourier_spectrum, 8);
  TGraph *gr_reco_12coef = reco_from_fourier_spectrum("gr_reco_12coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 10000, fourier_spectrum, 12);
  TGraph *gr_reco_16coef = reco_from_fourier_spectrum("gr_reco_16coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 10000, fourier_spectrum, 16);
  TGraph *gr_reco_20coef = reco_from_fourier_spectrum("gr_reco_20coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 10000, fourier_spectrum, 20);
  //----------------------
  TFile* rootFile = new TFile("fourier_series_box.root", "RECREATE", " Histograms", 1);
  rootFile->cd();
  if (rootFile->IsZombie()){
    cout<<"  ERROR ---> file "<<"fourier_series_box.root"<<" is zombi"<<endl;
    assert(0);
  }
  else
    cout<<"  Output Histos file ---> "<<"fourier_series_box.root"<<endl;
  //----------------------
  //
  //
  gr_box->Write();
  gr_hexagon->Write();
  get_xy_graph_from_phiR_graph(gr_box)->Write();
  //
  gr_a->Write();
  gr_b->Write();
  gr_ampl->Write();
  gr_reco_0coef->Write();
  gr_reco_4coef->Write();
  gr_reco_8coef->Write();
  gr_reco_12coef->Write();
  gr_reco_16coef->Write();
  gr_reco_20coef->Write();
  get_xy_graph_from_phiR_graph(gr_reco_0coef)->Write();
  get_xy_graph_from_phiR_graph(gr_reco_4coef)->Write();
  get_xy_graph_from_phiR_graph(gr_reco_8coef)->Write();
  get_xy_graph_from_phiR_graph(gr_reco_12coef)->Write();
  get_xy_graph_from_phiR_graph(gr_reco_16coef)->Write();
  get_xy_graph_from_phiR_graph(gr_reco_20coef)->Write();
  //
  rootFile->Close();
  //----------------------
  return 0;
}

Int_t lst(){

  TGraph *gr_meander = meander("gr_meander", 200, 1.0);

  //TGraph *gr_LST = read_data("LST.csv", "gr_LST", 1);
  TGraph *gr_LST_read_xy = read_data_read_xy("lst_mirror_outline_sorted.dat", "gr_LST_read_xy");
  TGraph *gr_LST_read_xy_interpolate = interpolate_segment_graph(gr_LST_read_xy, "gr_LST_read_xy_interpolate", 100);

  TGraph *gr_LST_read_phir_interpolate = get_phiR_graph_from_xy_graph(gr_LST_read_xy_interpolate);

  TGraph *gr_LST = read_data("lst_mirror_outline_sorted.dat", "gr_LST", 1, true);

  
  fourier_str fourier_spectrum;
  //get_fourier_series_spectrum(gr_LST, fourier_spectrum, 1000);
  get_fourier_series_spectrum(gr_LST_read_phir_interpolate, fourier_spectrum, 100);
  cout<<"get_fourier_series_spectrum"<<endl;
  //fourier_spectrum.print_info();
  //
  TGraph *gr_a = new TGraph();
  TGraph *gr_b = new TGraph();
  TGraph *gr_ampl = new TGraph();
  gr_a->SetNameTitle("gr_a", "gr_a");
  gr_b->SetNameTitle("gr_b", "gr_b");
  gr_ampl->SetNameTitle("gr_ampl", "gr_ampl");
  //
  get_fourier_spectrum_graph(gr_a, gr_b, gr_ampl, fourier_spectrum);
  //
  //
  //
  TGraph *gr_reco_meander_0coef = reco_from_fourier_spectrum("gr_reco_meander_0coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 100000, fourier_spectrum, 0);
  TGraph *gr_reco_meander_1coef = reco_from_fourier_spectrum("gr_reco_meander_1coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 100000, fourier_spectrum, 1);
  TGraph *gr_reco_meander_2coef = reco_from_fourier_spectrum("gr_reco_meander_2coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 100000, fourier_spectrum, 2);
  TGraph *gr_reco_meander_3coef = reco_from_fourier_spectrum("gr_reco_meander_3coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 100000, fourier_spectrum, 3);
  TGraph *gr_reco_meander_4coef = reco_from_fourier_spectrum("gr_reco_meander_4coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 100000, fourier_spectrum, 4);
  TGraph *gr_reco_meander_5coef = reco_from_fourier_spectrum("gr_reco_meander_5coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 100000, fourier_spectrum, 5);
  TGraph *gr_reco_meander_6coef = reco_from_fourier_spectrum("gr_reco_meander_6coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 100000, fourier_spectrum, 6);
  TGraph *gr_reco_meander_7coef = reco_from_fourier_spectrum("gr_reco_meander_7coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 100000, fourier_spectrum, 7);
  TGraph *gr_reco_meander_8coef = reco_from_fourier_spectrum("gr_reco_meander_8coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 100000, fourier_spectrum, 8);
  TGraph *gr_reco_meander_9coef = reco_from_fourier_spectrum("gr_reco_meander_9coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 100000, fourier_spectrum, 9);
  cout<<"gr_reco_meander_9coef"<<endl;
  TGraph *gr_reco_meander_10coef = reco_from_fourier_spectrum("gr_reco_meander_10coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 100000, fourier_spectrum, 10);
  TGraph *gr_reco_meander_15coef = reco_from_fourier_spectrum("gr_reco_meander_15coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 100000, fourier_spectrum, 15);
  TGraph *gr_reco_meander_20coef = reco_from_fourier_spectrum("gr_reco_meander_20coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 100000, fourier_spectrum, 20);
  TGraph *gr_reco_meander_30coef = reco_from_fourier_spectrum("gr_reco_meander_30coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 100000, fourier_spectrum, 30);
  TGraph *gr_reco_meander_40coef = reco_from_fourier_spectrum("gr_reco_meander_40coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 100000, fourier_spectrum, 40);
  TGraph *gr_reco_meander_50coef = reco_from_fourier_spectrum("gr_reco_meander_50coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 100000, fourier_spectrum, 50);
  TGraph *gr_reco_meander_60coef = reco_from_fourier_spectrum("gr_reco_meander_60coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 100000, fourier_spectrum, 60);
  TGraph *gr_reco_meander_70coef = reco_from_fourier_spectrum("gr_reco_meander_70coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 100000, fourier_spectrum, 70);
  TGraph *gr_reco_meander_80coef = reco_from_fourier_spectrum("gr_reco_meander_80coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 100000, fourier_spectrum, 80);
  TGraph *gr_reco_meander_90coef = reco_from_fourier_spectrum("gr_reco_meander_90coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 100000, fourier_spectrum, 90);
  TGraph *gr_reco_meander_100coef = reco_from_fourier_spectrum("gr_reco_meander_100coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 100000, fourier_spectrum, 100);
  TGraph *gr_reco_meander_100coef_cut = reco_from_fourier_spectrum("gr_reco_meander_100coef_cut", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 100000, fourier_spectrum, 100, 6.0);
  //cout<<"gr_reco_meander_100coef"<<endl;
  //TGraph *gr_reco_meander_200coef = reco_from_fourier_spectrum("gr_reco_meander_200coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 100000, fourier_spectrum, 200);
  //TGraph *gr_reco_meander_300coef = reco_from_fourier_spectrum("gr_reco_meander_300coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 100000, fourier_spectrum, 300);
  //TGraph *gr_reco_meander_400coef = reco_from_fourier_spectrum("gr_reco_meander_400coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 100000, fourier_spectrum, 400);
  //TGraph *gr_reco_meander_500coef = reco_from_fourier_spectrum("gr_reco_meander_500coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 100000, fourier_spectrum, 500);
  //TGraph *gr_reco_meander_600coef = reco_from_fourier_spectrum("gr_reco_meander_600coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 100000, fourier_spectrum, 600);
  //TGraph *gr_reco_meander_700coef = reco_from_fourier_spectrum("gr_reco_meander_700coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 100000, fourier_spectrum, 700);
  //TGraph *gr_reco_meander_800coef = reco_from_fourier_spectrum("gr_reco_meander_800coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 100000, fourier_spectrum, 800);
  //TGraph *gr_reco_meander_900coef = reco_from_fourier_spectrum("gr_reco_meander_900coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 100000, fourier_spectrum, 900);
  //TGraph *gr_reco_meander_1000coef = reco_from_fourier_spectrum("gr_reco_meander_1000coef", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 100000, fourier_spectrum, 1000);
  //cout<<"gr_reco_meander_1000coef"<<endl;
  TGraph *gr_reco_meander_all = reco_from_fourier_spectrum("gr_reco_meander_all", -0.0 * TMath::Pi(), 2.0 * TMath::Pi(), 100000, fourier_spectrum, fourier_spectrum.an.size());
  //  
  //
  //----------------------
  TFile* rootFile = new TFile("fourier_series_lst.root", "RECREATE", " Histograms", 1);
  rootFile->cd();
  if (rootFile->IsZombie()){
    cout<<"  ERROR ---> file "<<"fourier_series_lst.root"<<" is zombi"<<endl;
    assert(0);
  }
  else
    cout<<"  Output Histos file ---> "<<"fourier_series_lst.root"<<endl;
  //----------------------
  //
  //
  gr_meander->Write();
  //
  gr_a->Write();
  gr_b->Write();
  gr_ampl->Write();
  //
  gr_reco_meander_0coef->Write();
  gr_reco_meander_1coef->Write();
  gr_reco_meander_2coef->Write();
  gr_reco_meander_3coef->Write();
  gr_reco_meander_4coef->Write();
  gr_reco_meander_5coef->Write();
  gr_reco_meander_6coef->Write();
  gr_reco_meander_7coef->Write();
  gr_reco_meander_8coef->Write();
  gr_reco_meander_9coef->Write();
  gr_reco_meander_10coef->Write();
  gr_reco_meander_15coef->Write();
  gr_reco_meander_20coef->Write();
  gr_reco_meander_30coef->Write();
  gr_reco_meander_40coef->Write();
  gr_reco_meander_50coef->Write();
  gr_reco_meander_60coef->Write();
  gr_reco_meander_70coef->Write();
  gr_reco_meander_80coef->Write();
  gr_reco_meander_90coef->Write();
  gr_reco_meander_100coef->Write();
  gr_reco_meander_100coef_cut->Write();
  //gr_reco_meander_200coef->Write();
  //gr_reco_meander_300coef->Write();
  //gr_reco_meander_400coef->Write();
  //gr_reco_meander_500coef->Write();
  //gr_reco_meander_600coef->Write();
  //gr_reco_meander_700coef->Write();
  //gr_reco_meander_800coef->Write();
  //gr_reco_meander_900coef->Write();
  //gr_reco_meander_1000coef->Write();
  gr_reco_meander_all->Write();
  cout<<"gr_reco_meander_all->Write()"<<endl;
  //
  gr_LST->Write();
  gr_LST_read_xy->Write();
  gr_LST_read_xy_interpolate->Write();
  gr_LST_read_phir_interpolate->Write();
  //
  //get_xy_graph_from_phiR_graph(gr_meander)->Write();
  get_xy_graph_from_phiR_graph(gr_meander)->Write();
  get_xy_graph_from_phiR_graph(gr_reco_meander_0coef)->Write();
  get_xy_graph_from_phiR_graph(gr_reco_meander_1coef)->Write();
  get_xy_graph_from_phiR_graph(gr_reco_meander_2coef)->Write();
  get_xy_graph_from_phiR_graph(gr_reco_meander_3coef)->Write();
  get_xy_graph_from_phiR_graph(gr_reco_meander_4coef)->Write();
  get_xy_graph_from_phiR_graph(gr_reco_meander_5coef)->Write();
  get_xy_graph_from_phiR_graph(gr_reco_meander_6coef)->Write();
  get_xy_graph_from_phiR_graph(gr_reco_meander_7coef)->Write();
  get_xy_graph_from_phiR_graph(gr_reco_meander_8coef)->Write();
  get_xy_graph_from_phiR_graph(gr_reco_meander_9coef)->Write();
  cout<<"get_xy_graph_from_phiR_graph(gr_reco_meander_9coef)->Write()"<<endl;
  get_xy_graph_from_phiR_graph(gr_reco_meander_10coef)->Write();
  get_xy_graph_from_phiR_graph(gr_reco_meander_15coef)->Write();
  get_xy_graph_from_phiR_graph(gr_reco_meander_20coef)->Write();
  get_xy_graph_from_phiR_graph(gr_reco_meander_30coef)->Write();
  get_xy_graph_from_phiR_graph(gr_reco_meander_40coef)->Write();
  get_xy_graph_from_phiR_graph(gr_reco_meander_50coef)->Write();
  get_xy_graph_from_phiR_graph(gr_reco_meander_60coef)->Write();
  get_xy_graph_from_phiR_graph(gr_reco_meander_70coef)->Write();
  get_xy_graph_from_phiR_graph(gr_reco_meander_80coef)->Write();
  get_xy_graph_from_phiR_graph(gr_reco_meander_90coef)->Write();
  cout<<"get_xy_graph_from_phiR_graph(gr_reco_meander_90coef)->Write()"<<endl;
  get_xy_graph_from_phiR_graph(gr_reco_meander_100coef)->Write();
  get_xy_graph_from_phiR_graph(gr_reco_meander_100coef_cut)->Write();
  //get_xy_graph_from_phiR_graph(gr_reco_meander_200coef)->Write();
  //get_xy_graph_from_phiR_graph(gr_reco_meander_300coef)->Write();
  //get_xy_graph_from_phiR_graph(gr_reco_meander_400coef)->Write();
  //get_xy_graph_from_phiR_graph(gr_reco_meander_500coef)->Write();
  //get_xy_graph_from_phiR_graph(gr_reco_meander_600coef)->Write();
  //get_xy_graph_from_phiR_graph(gr_reco_meander_700coef)->Write();
  //get_xy_graph_from_phiR_graph(gr_reco_meander_800coef)->Write();
  //get_xy_graph_from_phiR_graph(gr_reco_meander_900coef)->Write();
  //get_xy_graph_from_phiR_graph(gr_reco_meander_1000coef)->Write();
  //cout<<"get_xy_graph_from_phiR_graph(gr_reco_meander_1000coef)->Write()"<<endl;
  //
  get_xy_graph_from_phiR_graph(gr_reco_meander_all)->Write();
  //
  get_xy_graph_from_phiR_graph(gr_LST)->Write();
  //
  rootFile->Close();
  //----------------------
  return 0;
}

TGraph* meander(TString name_title, Int_t np, Double_t ampl){
  TGraph *gr = new TGraph();
  gr->SetNameTitle(name_title, name_title);
  Double_t x_min = 0.0;
  Double_t x_max = 2.0*TMath::Pi();
  Double_t x, y;
  //Double_t aa = - 1.0 / TMath::Pi() / TMath::Pi();
  //Double_t bb =   2.0 / TMath::Pi();
  for(Int_t i = 0 ; i< np; i++){
    x = x_min + (x_max - x_min) / (np - 1) * i;
    //y = x;
    //y = aa*x*x + bb*x;
    //y = TMath::Sqrt(x);
    //y = TMath::Gaus( x, TMath::Pi(), TMath::Pi()/5.0);
    //y = x * x;
    //
    if(x<=TMath::Pi())
      y = ampl / 2.0;
    else
      y = -ampl / 2.0;
    //
    gr->SetPoint(i,x,y);
  }
  return gr;
}

TGraph *read_data(TString fname, TString name_title, Int_t np, bool if_true_lst_mirrors){
  TGraph *grtmp = new TGraph();
  TGraph *gr = new TGraph();
  gr->SetNameTitle(name_title, name_title);
  string mot;
  ifstream fFile(fname);
  Double_t x, y;
  TVector2 v;
  Int_t point_counter = 0;
  if(!if_true_lst_mirrors){
    if(fFile.is_open()){
      fFile>>mot>>mot;
      while(fFile>>x>>y){
	v.SetX(x);
	v.SetY(y);
	grtmp->SetPoint(point_counter, v.Phi(),v.Mod());
	cout<<"v.Phi() "<<v.Phi()<<endl;
	point_counter++;
      }
    }
  }
  else{
    Int_t f_mirID;
    Double_t f_xc, f_yc, f_x_apex, f_y_apex, f_r, f_phi;
    if(fFile.is_open()){
      //198           -462       -1066.08         -386.5       -1022.49         1093.1
      fFile>>mot>>mot>>mot>>mot>>mot>>mot>>mot;
      while(fFile>>f_mirID>>f_xc>>f_yc>>f_x_apex>>f_y_apex>>f_r>>f_phi){
	grtmp->SetPoint(point_counter, f_phi,f_r);
	point_counter++;
      }
    }  
  }
  fFile.close();
  //
  Double_t x_min = 0.0;
  Double_t x_max = 2.0*TMath::Pi();
  Int_t npoints = grtmp->GetN()*np;
  for(Int_t i = 0 ; i < npoints; i++){
    x = x_min + (x_max - x_min) / (npoints - 1) * i;
    gr->SetPoint(i,x,grtmp->Eval(x));
  }
  //  
  return gr;
}

void get_fourier_series_spectrum(TGraph *gr, fourier_str &fourier_spectrum, Int_t N){
  fourier_spectrum.a0 = fourier_spectrum_numerical_integral(gr);
  for(Int_t i = 1 ;i<=N;i++){
    fourier_spectrum.an.push_back(fourier_spectrum_numerical_integral(gr, i, 0));
    fourier_spectrum.bn.push_back(fourier_spectrum_numerical_integral(gr, i, 1));
  }
}

Double_t fourier_spectrum_numerical_integral(TGraph *gr, Int_t fourier_n, Int_t key){
  Int_t npoints = 10000;
  Double_t x_min = 0.0;
  Double_t x_max = 2.0*TMath::Pi();
  Double_t x, y;
  Double_t integral_val = 0;
  Double_t dx = (x_max - x_min) / (npoints - 1);
  if(fourier_n == 0 || key == 0){
    for(Int_t i = 0;i<npoints;i++){
      x = x_min + (x_max - x_min) / (npoints - 1) * i;
      y = gr->Eval(x);
      integral_val += y * TMath::Cos(fourier_n * x);
    }
    if(fourier_n == 0)
      return integral_val / 2.0 / TMath::Pi() * dx;
    return integral_val / TMath::Pi() * dx;
  }
  for(Int_t i = 0;i<npoints;i++){
    x = x_min + (x_max - x_min) / (npoints - 1) * i;
    y = gr->Eval(x);
    integral_val += y * TMath::Sin(fourier_n * x);
  }
  return integral_val / TMath::Pi() * dx;
}

TGraph *reco_from_fourier_spectrum(TString name_title, Double_t x_min, Double_t x_max, Int_t np, fourier_str &fourier_spectrum, unsigned int N, Double_t threshold){
  TGraph *gr = new TGraph();
  gr->SetNameTitle(name_title, name_title);
  Double_t x, y;
  Double_t ampl;
  Int_t n_Terms = 0 ;
  for(Int_t i = 0 ; i< np; i++){
    x = x_min + (x_max - x_min) / (np - 1) * i;
    y = fourier_spectrum.a0;
    for(unsigned int j = 0;j<N;j++){
      ampl = TMath::Sqrt(fourier_spectrum.an.at(j) * fourier_spectrum.an.at(j) +
			 fourier_spectrum.bn.at(j) * fourier_spectrum.bn.at(j));
      if(ampl>threshold){
	y += fourier_spectrum.an.at(j) * TMath::Cos( (j + 1) * x);
	y += fourier_spectrum.bn.at(j) * TMath::Sin( (j + 1) * x);
	if(i == 0)
	  n_Terms++;
      }
    }
    gr->SetPoint(i,x,y);
  }
  //
  cout<<setw(30)<<name_title<<setw(20)<<N<<setw(20)<<threshold<<setw(20)<<n_Terms<<endl;
  //
  return gr;  
}

void get_fourier_spectrum_graph(TGraph *gr_a, TGraph *gr_b, TGraph *gr_ampl, fourier_str &fourier_spectrum){
  gr_a->SetPoint(gr_a->GetN(),gr_a->GetN(),fourier_spectrum.a0);
  gr_ampl->SetPoint(gr_ampl->GetN(),gr_ampl->GetN(),fourier_spectrum.a0);
  for(unsigned int j = 0;j<fourier_spectrum.an.size();j++){
    gr_a->SetPoint(gr_a->GetN(),gr_a->GetN(), fourier_spectrum.an.at(j));
    gr_b->SetPoint(gr_b->GetN(),gr_b->GetN() + 1, fourier_spectrum.bn.at(j));
    gr_ampl->SetPoint(gr_ampl->GetN(),gr_ampl->GetN(),
		      TMath::Sqrt(fourier_spectrum.an.at(j) * fourier_spectrum.an.at(j) + fourier_spectrum.bn.at(j) * fourier_spectrum.bn.at(j)) );
  }
}

TGraph *get_xy_graph_from_phiR_graph(TGraph *gr){
  TGraph *grxy = new TGraph();
  TString name_title = gr->GetName();
  name_title += "_xy";
  grxy->SetNameTitle(name_title, name_title);
  Double_t phi, r;
  TVector2 v;
  for(Int_t i = 0 ; i < gr->GetN(); i++){
    gr->GetPoint( i, phi, r);
    r = TMath::Abs(r);
    v.SetMagPhi(r,phi);
    grxy->SetPoint(i,v.X(),v.Y());
  }
  return grxy;
}

TGraph *read_data_read_xy(TString fname, TString name_title){
  TGraph *gr = new TGraph();
  gr->SetNameTitle(name_title, name_title);
  string mot;
  ifstream fFile(fname);
  Int_t point_counter = 0;
  Int_t f_mirID;
  Double_t f_xc, f_yc, f_x_apex, f_y_apex, f_r, f_phi;
  if(fFile.is_open()){
    //198           -462       -1066.08         -386.5       -1022.49         1093.1
    fFile>>mot>>mot>>mot>>mot>>mot>>mot>>mot;
    while(fFile>>f_mirID>>f_xc>>f_yc>>f_x_apex>>f_y_apex>>f_r>>f_phi){
      cout<<setw(15)<<f_mirID
	<<setw(15)<<f_xc
	<<setw(15)<<f_yc
	<<setw(15)<<f_x_apex
	<<setw(15)<<f_y_apex
	<<setw(15)<<f_r
	<<setw(15)<<f_phi
	<<setw(15)<<endl;
       gr->SetPoint(point_counter, f_x_apex, f_y_apex);
       point_counter++;
    }
  }
  fFile.close();
  //  
  return gr;
}

TGraph *interpolate_segment_graph(TGraph *gr, TString name_title, Int_t np){
  TGraph *grinter = new TGraph();
  grinter->SetNameTitle(name_title, name_title);
  Double_t x1, y1;
  Double_t x2, y2;
  TVector2 v1;
  TVector2 v2;
  TVector2 dv;
  TVector2 ddv;
  TVector2 v;
  Double_t phi, mag, magmax;
  for(int i = 1;i<gr->GetN();i++){
    gr->GetPoint(i-1,x1,y1);
    gr->GetPoint(i,x2,y2);
    v1.SetX(x1);
    v1.SetY(y1);
    v2.SetX(x2);
    v2.SetY(y2);
    dv = v2 - v1;
    phi = dv.Phi();
    magmax = dv.Mod();
    for(Int_t j = 0;j<np;j++){
      mag = magmax / (np -1) * j;
      if(mag == 0.0){
	grinter->SetPoint(grinter->GetN(),v1.X(),v1.Y());
      }
      else{
	ddv.SetMagPhi(mag, phi);
	v = v1 + ddv;
	grinter->SetPoint(grinter->GetN(),v.X(),v.Y());
      }
    }
  }
  return grinter;
}

TGraph *get_phiR_graph_from_xy_graph(TGraph *gr){
  TGraph *grphir = new TGraph();
  TString name_title = gr->GetName();
  name_title += "_phir";
  grphir->SetNameTitle(name_title, name_title);
  Double_t x, y;
  Double_t phi, r;
  TVector2 v;
  Double_t minPhi = 100;
  Int_t minPhi_i;
  Double_t phi_threshold = -0.1;
  bool found = false;
  for(Int_t j = 0 ; j < gr->GetN(); j++){
    for(Int_t i = 0 ; i < gr->GetN(); i++){
      gr->GetPoint( i, x, y);
      v.SetX(x);
      v.SetY(y);
      //
      r = v.Mod();
      phi = v.Phi();
      if(phi<0)
	phi = 2.0*TMath::Pi() + phi;
      //
      if(phi>phi_threshold){
	if(minPhi>phi){
	  //cout<<"phi = "<<phi<<endl;
	  minPhi = phi;
	  minPhi_i = i;
	  found = true;
	}
      }
    }
    if(found){
      //cout<<"j = "<<j<<endl;
      gr->GetPoint( minPhi_i, x, y);
      v.SetX(x);
      v.SetY(y);
      //
      r = v.Mod();
      phi = v.Phi();
      if(phi<0)
	phi = 2.0*TMath::Pi() + phi;
      //
      //cout<<" grphir phi = "<<phi<<endl;
      grphir->SetPoint(grphir->GetN(),phi,r);
      //
      found = false;
      phi_threshold = phi;
      minPhi = 100;
    }
  }
  return grphir;
}

TGraph* box(TString name_title, Int_t np, Double_t side){
  TGraph *gr = new TGraph();
  gr->SetNameTitle(name_title);
  Double_t x, y;
  Double_t xmin, xmax, ymin, ymax;
  TVector2 v;
  Double_t phi, r;
  //H. T.
  xmin = -side / 2.0;
  xmax = side / 2.0; 
  y = side / 2.0; 
  for(Int_t i = 0 ; i < np; i++){
    x = xmin + (xmax - xmin) / (np - 1) * i;
    v.SetX(x);
    v.SetY(y);
    //
    r = v.Mod();
    phi = v.Phi();
    if(phi<0)
      phi = 2.0*TMath::Pi() + phi;
    gr->SetPoint(gr->GetN(),phi,r);
  }
  //V. R.
  ymin = -side / 2.0;
  ymax = side / 2.0; 
  x = side / 2.0; 
  for(Int_t i = 0 ; i < np; i++){
    y = ymax - (ymax - ymin) / (np - 1) * i;
    v.SetX(x);
    v.SetY(y);
    //
    r = v.Mod();
    phi = v.Phi();
    if(phi<0)
      phi = 2.0*TMath::Pi() + phi;
    gr->SetPoint(gr->GetN(),phi,r);
  }
  //H. B.
  xmin = -side / 2.0;
  xmax = side / 2.0; 
  y = -side / 2.0; 
  for(Int_t i = 0 ; i < np; i++){
    x = xmax - (xmax - xmin) / (np - 1) * i;
    v.SetX(x);
    v.SetY(y);
    //
    r = v.Mod();
    phi = v.Phi();
    if(phi<0)
      phi = 2.0*TMath::Pi() + phi;
    gr->SetPoint(gr->GetN(),phi,r);
  }
  //V. L.
  ymin = -side / 2.0;
  ymax = side / 2.0; 
  x = -side / 2.0; 
  for(Int_t i = 0 ; i < np; i++){
    y = ymin + (ymax - ymin) / (np - 1) * i;
    v.SetX(x);
    v.SetY(y);
    //
    r = v.Mod();
    phi = v.Phi();
    if(phi<0)
      phi = 2.0*TMath::Pi() + phi;
    gr->SetPoint(gr->GetN(),phi,r);
  }
  return gr;
}

TGraph* hexagon(TString name_title, Int_t np, Double_t side){
  TGraph *gr = new TGraph();
  //
  const Int_t n = 7;
  Double_t flat_to_flat = side;
  //
  Double_t x, y;
  //
  Double_t alpha   = 2.0*TMath::Pi()/6.0;
  Double_t alpha_2 = alpha/2.0;
  Double_t alpha0  = alpha_2;
  Double_t l_2 = flat_to_flat/2.0;
  Double_t rr = l_2/TMath::Cos(alpha_2);
  Double_t theta = 0.0;
  for(Int_t i = 0;i<n;i++){
    theta = alpha0 + alpha*i;
    x = rr*TMath::Cos(theta);
    y = rr*TMath::Sin(theta);
    gr->SetPoint(gr->GetN(),x,y);
  }
  //
  return get_phiR_graph_from_xy_graph(interpolate_segment_graph(gr, name_title, np));
}
