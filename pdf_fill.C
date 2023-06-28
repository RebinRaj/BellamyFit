//Programe calculates the theoritical Bellamy function given input parameters
//Fills the values using a root TF1 and TSpline functionalities.
//Author: Karaparambil Rajan Rebin          #Date: 23 Jun 2023

#include <iostream>
#include<math.h>
#include "TF1.h"

/***************************** Function prototype *************************/
double funcBellamy(double Val, double *par);
/**************************************************************************/

int pdf_fill(double *PAR, std::vector<double> &Pdf_vec, std::vector<double> &If_adc)
{

  for(int b=0; b<If_adc.size(); b++)
    {
      double value = If_adc[b]; 
      double pdf_val = funcBellamy(value, PAR);
      Pdf_vec.push_back(pdf_val);
    }

  return 0;
}


double funcBellamy(double Val, double *par)
{
  double x   = Val;
  double N_0 = par[0];   //Scaling
  double Q_0 = par[1];   //Gain pedestal
  double Q_1 = par[2];   //Gain signal
  double s_0 = par[3];   //Sigma pedestal
  double s_1 = par[4];   //Sigma signal
  double w = par[5];     //Background probability
  double alpha = par[6]; //Background coefficient
  double mu = par[7];    //Mean light
  double Q_sh = w/alpha; //Background shift
 
  //Heaviside function
  double theta;
  if(x>=Q_0){
    theta=1.;
  }
  else{
    theta=0.;
  }

  double beta1_pd = -1*pow((x-Q_0),2)/ ( 2*pow(s_0,2) );
  double beta2_pd = -1*alpha*(x-Q_0);
  double Pedestal = ( (1-w)*exp(beta1_pd)/(s_0*sqrt(2*M_PI)) + w*theta*alpha*exp(beta2_pd) )*exp(-1.*mu);
  double Bellamy =0.;
  double beta_sig;
  double n=0.;
  for(int i=1; i<150; ++i)
    {
      n = 1.*i;
      beta_sig = -1*pow( (x- Q_0 - Q_sh - n*Q_1),2 )/( 2*n*pow(s_1,2) );
      Bellamy += ( TMath::Poisson(n,mu) * exp(beta_sig) / (s_1*sqrt(2*M_PI*n)) );
    }
  Bellamy = N_0*(Bellamy+Pedestal);
  
  return Bellamy;

}
