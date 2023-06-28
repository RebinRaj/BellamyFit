//Programe using TMinuit minimisation for bellamy fit
//User defined bellamy function and FCN
//Author: Karaparambil Rajan Rebin        #Date: 23 Jun 2023
//'For the fun in coding & love in physics'

#include <cstdlib>
#include <iostream>
#include <time.h>
#include "TMath.h"
#include "TROOT.h"
#include "TRint.h"
#include "TMinuit.h"

#include "data_fill.h"
#include "chisq.h"
#include "pdf_fill.h"
#include "plot_spectrum.h"

using namespace std;

std::vector<double> data_vec;
std::vector<double> if_adc;
std::vector<double> pdf_vec;

//Binning values
double bin_width = 1.0; //1 adc
double left_edge = -0.5;
double right_edge = 200.5;
double bin_info[3] = {bin_width, left_edge, right_edge};
int limits[2] = {28, 150};
double FitBin;
int calls = 0;
bool plot = true;
bool plot_interactive = true;


int main()
{

  //Fill the PMT adc data
  data_fill(data_vec, if_adc, bin_info);

  //initialize TMinuit
  TMinuit *gMinuit = new TMinuit(8);    //Give number of parameters in the fit
  gMinuit->SetFCN(chq_fcn);

  double arglist[10];
  int ierflg =0;
  arglist[0] =1;
  gMinuit->mnexcm("SET ERRordef", arglist ,1,ierflg);

  //----------------------------------------------------------
  //-----------------Set up parameters-------------------------
  //Par1 N_0: scaling        [2000
  //Par2 Q_0: Gain pedestal  [30
  //Par3 Q_1: Gain signal    [10
  //Par4 s_0: Sigma pedestal [0.5
  //Par5 s_1: Sigma signal   [6
  //Par6 w: Bkg probability  [0.1
  //Par7 a: Bkg coeff        [0.4
  //Par8 mu: Mean light      [1.4
  //-----------------------------------------------------------
  //double vstart[8] = {2300, 28.1769, 10, 0.02, 5, 0.1, 0.4, 1.5};
  //double step[8] = {5, 0.01, 0.1, 0.001, 0.2, 0.01, 0.05, 0.1};
  //double L_lim[8] = {1000, 26.5, 8, 0.001, 3, 0.01, 0.1, 1};
  //double U_lim[8] = {3000, 28.5, 14, 1, 8, 0.6, 0.8, 3};
  
  double vstart[8] = {1000000, 28.1769, 10, 0.02, 5, 0.1, 0.4, 1.5};
  double step[8] = {5, 0.01, 0.1, 0.001, 0.2, 0.01, 0.05, 0.1};
  double L_lim[8] = {100000, 26.5, 8, 0.001, 3, 0.01, 0.1, 0.2};
  double U_lim[8] = {2000000, 28.5, 14, 1, 8, 0.6, 0.8, 3};
    
  string par_names[8] = {"N_{0}","Q_{0}", "Q_{1}", "#sigma_{0}", "#sigma_{1}", "w", "#alpha", "#mu"};

  for(int p=0; p<8; p++)
    {
      gMinuit->mnparm(p, par_names[p], vstart[p] , step[p], L_lim[p], U_lim[p], ierflg);
    }

  //Setting strategy
  arglist[0] = 2;
  gMinuit->mnexcm("SET STRATEGY",arglist,1,ierflg);

  //Strat minimization
  arglist[0] = 5000;
  arglist[1] = 1;
  gMinuit->mnexcm("MIGRAD", arglist ,2,ierflg);

  bool converged = true;
  //Check for convergence
  if(ierflg == 4)
    {
      converged = false;
    }

  if(converged)
    {
      //gMinuit->mnexcm("JUMp", arglist ,1,ierflg);
      //gMinuit->mnexcm("IMProve", arglist ,1,ierflg);
    }
  if(!converged)
    {
      cout << "Not Converged : Starting Minos" << endl;
      gMinuit->mnexcm("MINOS", arglist ,1,ierflg);
    }

  //Print results
  double chqmin,edm,errdef;
  int nvpar,nparx,co_stat, number;
  gMinuit->mnstat(chqmin,edm,errdef,nvpar,nparx,co_stat);
  double parval[8];
  double parerr[8];
  for(int p=0; p<8; p++)
    gMinuit->GetParameter(p,parval[p],parerr[p]);

  cout<<"###***** OUTPUT *****###"<<endl;
  cout<<"chq : "<<chqmin<<endl;
  for(int p=0; p<8; p++){
    cout<<par_names[p]<<"\t"<<parval[p]<<"\t err : "<<parerr[p]<<endl;}
  cout<<"###*******************###"<<endl;

  double chi_info[2];
  chi_info[0] = chqmin; //Minumum chi-square
  chi_info[1] = FitBin - 8; //NDF  
  
 if(plot){
   plot_spectrum(data_vec, if_adc, parval, parerr, chi_info, bin_info, limits, plot_interactive);
   }
}

void chq_fcn(int &npar, double *gin, double &func, double *par, int iflag)
{
  calls++;
  pdf_vec.clear();
  //Construct pdf with bellamy function
  pdf_fill(par, pdf_vec, if_adc);

  //Calculate the chi-square value
  double T_chq;
  double bns;
  chisq(data_vec, pdf_vec, T_chq, bns, limits);
  FitBin = bns;
  
  cout<<"Minimization call: "<<calls<<endl;
  cout<<"Total bins: "<<bns<<"\t"<<"Chi-sq: "<<T_chq<<endl;
  cout<<""<<endl;
  
  func = (T_chq);

}
