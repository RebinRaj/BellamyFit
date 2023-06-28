//Programe to plot and compare the converged spectrum
//Author: Karaparambil Rajan Rebin     #Date: 23 Jun 2023

#include <cstdlib>
#include <iostream>
#include<math.h>
#include <vector>

#include "TH1D.h"
#include "TCanvas.h"
#include "pdf_fill.h"
#include "TPaveStats.h"
#include "TStyle.h"
#include "TLatex.h"
#include "TRint.h"

using namespace std;

int plot_spectrum(std::vector<double> &Data, std::vector<double> &ADC, double par[],double par_err[], double chi_info[], double Bin_info[], int limits[], bool interactive)
{
  std::vector<double> PDF;
  PDF.clear();
  pdf_fill(par, PDF, ADC);  

  //Histogram
  int Bins = (Bin_info[2] - Bin_info[1])/Bin_info[0];
  TH1D * hist_data = new TH1D("","",Bins, Bin_info[1], Bin_info[2]);
  TH1D * hist_pdf = new TH1D("","",Bins, Bin_info[1], Bin_info[2]);

  int bins = hist_data->GetNbinsX();

  if(bins = Data.size())
    {
      cout<<"Plotting the fit spectrum"<<endl;
      for(int b=0; b<Data.size(); b++)
        {
          hist_data->SetBinContent(b+1, Data[b]);

          int val = b+1;
          if(val>limits[0] && val<limits[1]){
            hist_pdf->SetBinContent(b+1, PDF[b]);
          }
          else{
            hist_pdf->SetBinContent(b+1,0);
          }
          
        }
    }
  
  //Plot
  
  int argc; char **argv;
  TRint app("app", &argc, argv);
  
  TCanvas *c1 = new TCanvas("c1","c1");
  gStyle->SetOptStat();
  gPad->SetGrid();
  gPad-> SetLogy();
  hist_data->SetTitle("; ADC; Count");
  hist_data->GetXaxis()->CenterTitle();
  hist_data->GetXaxis()->SetTitleSize(0.045);
  hist_data->GetYaxis()->CenterTitle();
  hist_data->GetYaxis()->SetTitleSize(0.045);
  hist_data->SetLineColor(1);
  hist_data->SetLineWidth(2);
  hist_data->SetLineStyle(1);
  hist_data->Draw();

  hist_pdf->SetLineColor(2);
  hist_pdf->Draw("sames");

  c1->Update();

  
  TPaveStats *ps = (TPaveStats*)c1->GetPrimitive("stats");
  if(interactive)
    ps->SetName("fit-stats");
  TList *listOfLines = ps->GetListOfLines();

  TText *tconst = ps->GetLineWith("Entries");
  listOfLines->Remove(tconst);

  TText *tconst2 = ps->GetLineWith("Mean");
  listOfLines->Remove(tconst2);

  TText *tconst3 = ps->GetLineWith("Std");
  listOfLines->Remove(tconst3);

  double chbin = chi_info[0]/chi_info[1];
  
  TLatex *myt[9];
  myt[0] = new TLatex(0,0,Form("#chi^{2}/NDF = %1.2f",chbin));
  myt[1] = new TLatex(0,0,Form("N_{0} = %1.2f #pm %1.4f",par[0], par_err[0]));
  myt[2] = new TLatex(0,0,Form("Q_{0} = %1.2f #pm %1.4f",par[1], par_err[1]));
  myt[3] = new TLatex(0,0,Form("Q_{1} = %2.2f #pm %1.4f",par[2], par_err[2]));
  myt[4] = new TLatex(0,0,Form("#sigma_{0} = %1.2f #pm %1.4f",par[3], par_err[3]));
  myt[5] = new TLatex(0,0,Form("#sigma_{1} = %1.2f #pm %1.4f",par[4], par_err[4]));
  myt[6] = new TLatex(0,0,Form("w = %1.2f #pm %1.4f",par[5], par_err[5]));
  myt[7] = new TLatex(0,0,Form("#alpha = %1.2f #pm %1.4f",par[6], par_err[6]));
  myt[8] = new TLatex(0,0,Form("#mu = %1.2f #pm %1.4f",par[7], par_err[7]));

  listOfLines->Add(myt[0]);
  myt[0]->SetTextFont(42);
  myt[0]->SetTextSize(0.03);

  listOfLines->Add(myt[1]);
  myt[1]->SetTextFont(42);
  myt[1]->SetTextSize(0.03);

  listOfLines->Add(myt[2]);
  myt[2]->SetTextFont(42);
  myt[2]->SetTextSize(0.03);

  listOfLines->Add(myt[3]);
  myt[3]->SetTextFont(42);
  myt[3]->SetTextSize(0.03);

  listOfLines->Add(myt[4]);
  myt[4]->SetTextFont(42);
  myt[4]->SetTextSize(0.03);

  listOfLines->Add(myt[5]);
  myt[5]->SetTextFont(42);
  myt[5]->SetTextSize(0.03);

  listOfLines->Add(myt[6]);
  myt[6]->SetTextFont(42);
  myt[6]->SetTextSize(0.03);

  listOfLines->Add(myt[7]);
  myt[7]->SetTextFont(42);
  myt[7]->SetTextSize(0.03);
  
  listOfLines->Add(myt[8]);
  myt[8]->SetTextFont(42);
  myt[8]->SetTextSize(0.03);

    
  
  hist_data->SetStats(0);
  hist_pdf->SetStats(0);
  c1->Modified();
  c1->Update();

  if(interactive){
    app.Run();}
  else{
    c1->Print("spectrum.pdf"); 
  }
  return 0;
}
