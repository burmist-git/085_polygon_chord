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

struct segment_str {
  Double_t x0;
  Double_t y0;
  Double_t x1;
  Double_t y1;
  Double_t dt;
  TVector2 r0;
  TVector2 r1;
  TVector2 dr;
  Double_t l;
  TVector2 v;
  /////intersection/////
  Double_t t;
  Double_t s;
  Double_t status;
  Double_t x_int;
  Double_t y_int;
  TVector2 r_int;
  //////////////////////
  TVector2 r_mu;
  TVector2 v_mu;
  //////////////////////
  segment_str(){
    x0 = 0.0;
    y0 = 0.0;
    x1 = 0.0;
    y1 = 0.0;
    dt = 0.0;
    t = -999.0;
    s = -999.0;
    status = -1;
    x_int = -999.0;
    y_int = -999.0;
  };
  void init(){
    dt = 1.0;
    r0.SetX(x0);
    r0.SetY(y0);
    r1.SetX(x1);
    r1.SetY(y1);
    dr = r1 - r0;
    l = dr.Mod();
    v = dr / dt;
  };  
  void intersection(TVector2 rmu, TVector2 vmu){
    r_mu = rmu;
    v_mu = vmu;  
    //
    // a1    * x + b1   * y = c1
    // a2    * x + b2   * y = c2
    // vi_x * t + vmu_x * (-s) = rmu_x - ri_x
    // vi_y * t + vmu_y * (-s) = rmu_y - ri_y
    //
    // D = a1 * b2 - a2 * b1
    // x = ( c1 * b2 - c2 * b1 ) / D 
    // y = ( a1 * c2 - a2 * c1 ) / D
    //
    // c1 = rmu_x - ri_x
    // c2 = rmu_y - ri_y
    // D  = vi_x * vmu_y - vi_y * vmu_x
    //  t = (   c1 * vmu_y -   c2 * vmu_x ) / D 
    // -s = ( vi_x * c2    - vi_y * c1    ) / D
    //
    Double_t tolerance = 1.0e-20;
    Double_t c1 = rmu.X() - r0.X();
    Double_t c2 = rmu.Y() - r0.Y();
    Double_t D  = v.X() * vmu.Y() - v.Y() * vmu.X();
    //
    if(TMath::Abs(D) < tolerance){
      t = -999.0;
      status = -1;
      x_int = -999.0;
      y_int = -999.0;
      r_int.SetX(x_int);
      r_int.SetY(y_int);
    }
    else{
      //
      t = (c1 * vmu.Y() - c2 * vmu.X()) / D; 
      s = -(v.X() * c2 - v.Y() * c1) / D;
      if(s>0){
      if(t>= 0 && t<1.0)
	status = 0;
      else
	status = 1;
      }
      else{
	status = 1;
      }
      //
      r_int = r0 + v * t;
      x_int = r_int.X();
      y_int = r_int.Y();
    }
  };
  void print_info(Bool_t ifHead = false){
    if(ifHead){
      cout<<setw(15)<<"x0"
	  <<setw(15)<<"y0"
	  <<setw(15)<<"x1"
	  <<setw(15)<<"y1"
	  <<setw(15)<<"dt"
	  <<setw(15)<<"l"
	  <<setw(15)<<"status"
	  <<setw(15)<<"t"
	  <<setw(15)<<"s"
	  <<setw(15)<<"x_int"
	  <<setw(15)<<"y_int"
	  <<endl;
    }
    else{
      cout<<setw(15)<<x0
	  <<setw(15)<<y0
	  <<setw(15)<<x1
	  <<setw(15)<<y1
	  <<setw(15)<<dt
	  <<setw(15)<<l
	  <<setw(15)<<status
	  <<setw(15)<<t
	  <<setw(15)<<s
	  <<setw(15)<<x_int
	  <<setw(15)<<y_int
	  <<endl;
    }
  };
};

TGraph *read_data(TString fname, TString name_title, vector<segment_str> &segment_v);
Int_t lst();
Int_t test();
Int_t lst_reco_test();

Int_t main(){
  return lst();
  //return test();
  //return lst_reco_test();
}

Int_t lst(){
  //
  vector<segment_str> segment_v;
  //
  TGraph *gr_lst = read_data("lst_mirror_outline_sorted.dat", "gr_lst", segment_v);
  TGraph *gr_intersection = new TGraph();
  gr_intersection->SetNameTitle("gr_intersection","gr_intersection");
  ////////////////////////////////////////////
  TVector2 rmu01(300.0, 300.0);
  TVector2 vmu01(1.0, 0.0);
  for(unsigned int i = 0; i< segment_v.size(); i++)
    segment_v.at(i).intersection(rmu01, vmu01);
  //
  segment_v.at(0).print_info(true);
  for(unsigned int i = 0; i< segment_v.size(); i++){
    if(segment_v.at(i).status == 0){
      gr_intersection->SetPoint(gr_intersection->GetN(),segment_v.at(i).r_mu.X(),segment_v.at(i).r_mu.Y());
      gr_intersection->SetPoint(gr_intersection->GetN(),segment_v.at(i).x_int,segment_v.at(i).y_int);
      gr_intersection->SetPoint(gr_intersection->GetN(),segment_v.at(i).r_mu.X(),segment_v.at(i).r_mu.Y());
    }
    segment_v.at(i).print_info();
  }

  ////////////////////////////////////////////
  TVector2 rmu02(300.0, 300.0);
  TVector2 vmu02(1.0, 1.0);
  for(unsigned int i = 0; i< segment_v.size(); i++)
    segment_v.at(i).intersection(rmu02, vmu02);
  for(unsigned int i = 0; i< segment_v.size(); i++){
    if(segment_v.at(i).status == 0){
      gr_intersection->SetPoint(gr_intersection->GetN(),segment_v.at(i).r_mu.X(),segment_v.at(i).r_mu.Y());
      gr_intersection->SetPoint(gr_intersection->GetN(),segment_v.at(i).x_int,segment_v.at(i).y_int);
      gr_intersection->SetPoint(gr_intersection->GetN(),segment_v.at(i).r_mu.X(),segment_v.at(i).r_mu.Y());
    }
  }
  ////////////////////////////////////////////
  TVector2 rmu03(300.0, 300.0);
  TVector2 vmu03(0.0, 1.0);
  for(unsigned int i = 0; i< segment_v.size(); i++)
    segment_v.at(i).intersection(rmu03, vmu03);
  for(unsigned int i = 0; i< segment_v.size(); i++){
    if(segment_v.at(i).status == 0){
      gr_intersection->SetPoint(gr_intersection->GetN(),segment_v.at(i).r_mu.X(),segment_v.at(i).r_mu.Y());
      gr_intersection->SetPoint(gr_intersection->GetN(),segment_v.at(i).x_int,segment_v.at(i).y_int);
      gr_intersection->SetPoint(gr_intersection->GetN(),segment_v.at(i).r_mu.X(),segment_v.at(i).r_mu.Y());
    }
  }
  ////////////////////////////////////////////
  TVector2 rmu04(300.0, 300.0);
  TVector2 vmu04(-1.0, 1.0);
  for(unsigned int i = 0; i< segment_v.size(); i++)
    segment_v.at(i).intersection(rmu04, vmu04);
  for(unsigned int i = 0; i< segment_v.size(); i++){
    if(segment_v.at(i).status == 0){
      gr_intersection->SetPoint(gr_intersection->GetN(),segment_v.at(i).r_mu.X(),segment_v.at(i).r_mu.Y());
      gr_intersection->SetPoint(gr_intersection->GetN(),segment_v.at(i).x_int,segment_v.at(i).y_int);
      gr_intersection->SetPoint(gr_intersection->GetN(),segment_v.at(i).r_mu.X(),segment_v.at(i).r_mu.Y());
    }
  }
  ////////////////////////////////////////////
  TVector2 rmu05(300.0, 300.0);
  TVector2 vmu05(-1.0, 0.0);
  for(unsigned int i = 0; i< segment_v.size(); i++)
    segment_v.at(i).intersection(rmu05, vmu05);
  for(unsigned int i = 0; i< segment_v.size(); i++){
    if(segment_v.at(i).status == 0){
      gr_intersection->SetPoint(gr_intersection->GetN(),segment_v.at(i).r_mu.X(),segment_v.at(i).r_mu.Y());
      gr_intersection->SetPoint(gr_intersection->GetN(),segment_v.at(i).x_int,segment_v.at(i).y_int);
      gr_intersection->SetPoint(gr_intersection->GetN(),segment_v.at(i).r_mu.X(),segment_v.at(i).r_mu.Y());
    }
  }
  ////////////////////////////////////////////
  TVector2 rmu06(300.0, 300.0);
  TVector2 vmu06(-1.0, -1.0);
  for(unsigned int i = 0; i< segment_v.size(); i++)
    segment_v.at(i).intersection(rmu06, vmu06);
  for(unsigned int i = 0; i< segment_v.size(); i++){
    if(segment_v.at(i).status == 0){
      gr_intersection->SetPoint(gr_intersection->GetN(),segment_v.at(i).r_mu.X(),segment_v.at(i).r_mu.Y());
      gr_intersection->SetPoint(gr_intersection->GetN(),segment_v.at(i).x_int,segment_v.at(i).y_int);
      gr_intersection->SetPoint(gr_intersection->GetN(),segment_v.at(i).r_mu.X(),segment_v.at(i).r_mu.Y());
    }
  }
  ////////////////////////////////////////////
  TVector2 rmu07(300.0, 300.0);
  TVector2 vmu07(0.0, -1.0);
  for(unsigned int i = 0; i< segment_v.size(); i++)
    segment_v.at(i).intersection(rmu07, vmu07);
  for(unsigned int i = 0; i< segment_v.size(); i++){
    if(segment_v.at(i).status == 0){
      gr_intersection->SetPoint(gr_intersection->GetN(),segment_v.at(i).r_mu.X(),segment_v.at(i).r_mu.Y());
      gr_intersection->SetPoint(gr_intersection->GetN(),segment_v.at(i).x_int,segment_v.at(i).y_int);
      gr_intersection->SetPoint(gr_intersection->GetN(),segment_v.at(i).r_mu.X(),segment_v.at(i).r_mu.Y());
    }
  }
  ////////////////////////////////////////////
  TVector2 rmu08(300.0, 300.0);
  TVector2 vmu08(1.0, -1.0);
  for(unsigned int i = 0; i< segment_v.size(); i++)
    segment_v.at(i).intersection(rmu08, vmu08);
  for(unsigned int i = 0; i< segment_v.size(); i++){
    if(segment_v.at(i).status == 0){
      gr_intersection->SetPoint(gr_intersection->GetN(),segment_v.at(i).r_mu.X(),segment_v.at(i).r_mu.Y());
      gr_intersection->SetPoint(gr_intersection->GetN(),segment_v.at(i).x_int,segment_v.at(i).y_int);
      gr_intersection->SetPoint(gr_intersection->GetN(),segment_v.at(i).r_mu.X(),segment_v.at(i).r_mu.Y());
    }
  }


  //
  //----------------------
  TFile* rootFile = new TFile("polygon_chord.root", "RECREATE", " Histograms", 1);
  rootFile->cd();
  if (rootFile->IsZombie()){
    cout<<"  ERROR ---> file "<<"polygon_chord.root"<<" is zombi"<<endl;
    assert(0);
  }
  else
    cout<<"  Output Histos file ---> "<<"polygon_chord.root"<<endl;
  //----------------------
  //
  gr_lst->Write();
  gr_intersection->Write();
  //
  rootFile->Close();
  //----------------------
  return 0;
}

Int_t test(){
  //
  segment_str segment;
  //
  segment.x0 = 1000;
  segment.y0 = 100;
  segment.x1 = 1000;
  segment.y1 = -100;
  segment.init();
  TVector2 rmu(0.0, 0.0);
  TVector2 vmu(1.0, 0.0);
  segment.intersection(rmu, vmu);
  segment.print_info(true);
  segment.print_info();
  //----------------------
  return 0;
}

TGraph *read_data(TString fname, TString name_title, vector<segment_str> &segment_v){
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
  for(Int_t i = 0 ; i < (gr->GetN() - 1); i++){
    segment_str str_tmp;
    gr->GetPoint( i, str_tmp.x0, str_tmp.y0);
    gr->GetPoint( i+1, str_tmp.x1, str_tmp.y1);
    str_tmp.init();
    segment_v.push_back(str_tmp);    
  }
  //  
  return gr;
}

Int_t lst_reco_test(){
  //
  vector<segment_str> segment_v;
  TGraph *gr_lst = read_data("lst_mirror_outline_sorted.dat", "gr_lst", segment_v);
  TGraph *gr_lst_reco = new TGraph();
  gr_lst_reco->SetNameTitle("gr_lst_reco","gr_lst_reco");
  //
  TVector2 rmu(200.0, 200.0);
  TVector2 vmu(1.0, 0.0);
  for(unsigned int i = 0; i< segment_v.size(); i++){
    segment_v.at(i).intersection(rmu, vmu);
    if(i<100){
      gr_lst_reco->SetPoint(gr_lst_reco->GetN(), segment_v.at(i).x0, segment_v.at(i).y0);
      gr_lst_reco->SetPoint(gr_lst_reco->GetN(), segment_v.at(i).x1, segment_v.at(i).y1);
    }
  }
  //
  //----------------------
  TFile* rootFile = new TFile("lst_reco_test.root", "RECREATE", " Histograms", 1);
  rootFile->cd();
  if (rootFile->IsZombie()){
    cout<<"  ERROR ---> file "<<"lst_reco_test.root"<<" is zombi"<<endl;
    assert(0);
  }
  else
    cout<<"  Output Histos file ---> "<<"lst_reco_test.root"<<endl;
  //----------------------
  //
  gr_lst->Write();
  gr_lst_reco->Write();
  //
  rootFile->Close();
  //----------------------
  return 0;
}
