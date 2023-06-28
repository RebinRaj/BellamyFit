//Programe to calculate the chisquare
//Author: Karaparambil Rajan Rebin      #Date: 23 Jun 2023

#include <cstdlib>
#include <iostream>
#include<math.h>
#include <vector>

using namespace std;

int chisq(std::vector<double> &Data, std::vector<double> &PDF, double &t_chq, double &bins, int limits[])
{
  double chi =0;
  
  bins =0;
  for(int b=0; b<Data.size(); b++)
    {
      int val = b+1;
      if(Data[b] != 0 && val>limits[0] && val<limits[1])
        {
          bins++;
          
          double Ntr = Data[b] - PDF[b];
          chi += pow(Ntr, 2)/Data[b];
          
        }
    }

  t_chq = chi;

  return 0;
}
