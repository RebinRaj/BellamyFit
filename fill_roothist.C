//Programme to fill the root histogram
//Reads a root file to fill the histogram
//Author: Karaparambil Rajan Rebin       #Date: 23 Jun 2023

#include <cstdlib>
#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "fill_roothist.h"

using namespace std;

int fill_roothist(TH1D* &Hist)
{
  //28
  int ch = 28; //Channel number of the data to be filled

  TFile* fileIn = TFile::Open("led_c2L_2020-11-16_18_52.root");

  UShort_t adc[64];
  Bool_t trigger_mask[64];
  ULong64_t timestamp_int;

  TTree* mytree;
  mytree = (TTree*)fileIn->Get("TT_ROB_data");

  mytree->SetBranchAddress("charge", adc);
  mytree->SetBranchAddress("trigger_mask", trigger_mask);
  mytree->SetBranchAddress("timeStamp_int",&timestamp_int);

  Long64_t entries = mytree->GetEntries();

  ULong64_t time0;
  ULong64_t timeDiff;

  for(Long64_t j=0; j<entries; j++)
    {
      mytree->GetEntry(j);
      
      if(j==0){time0 = timestamp_int;}
      timeDiff = timestamp_int - time0;
      
      Hist->Fill(adc[ch]);

    }

  double scale = timeDiff*1e-9; //seconds
  double scale_pdf = 1/scale;
  //  Hist->Scale(scale_pdf);

  cout<<"Signal rate (Hz): "<<Hist->Integral()<<endl;
  
  fileIn->Close();
  delete fileIn;
  
  return 0;
}
