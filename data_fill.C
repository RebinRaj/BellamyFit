//Programme to fill the data vector
//Calls a subroutine that fills a root histogram
//Author: Karaparambil Rajan Rebin        #Date: 23 Jun 2023

#include <cstdlib>
#include <iostream>

#include "TH1D.h"
#include "data_fill.h"
#include "fill_roothist.h"

using namespace std;


int data_fill(std::vector<double> &Data_vec, std::vector<double> &If_adc, double Bin_info[])
{

  //Fill the root hitogram in accordance with root file structure
  int Bins = (Bin_info[2] - Bin_info[1])/Bin_info[0];
  TH1D * hist = new TH1D("","",Bins, Bin_info[1], Bin_info[2]);
  fill_roothist(hist);

  int bins = hist->GetNbinsX();

  for(int b=1; b<=bins; b++)
    {
      double val = hist->GetBinContent(b);
      double adc = hist->GetBinCenter(b);

      Data_vec.push_back(val);
      If_adc.push_back(adc);
      
    }

  return 0;

}
