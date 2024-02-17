#include <iostream>
#include <cmath>
#include <string>
#include <ctime>
#include <chrono>
#include "pricing.h"

using namespace std;

int main() {

    // Obtain inputs

    DerivativesPricing Option1;

    if(Option1.Option_Inputs() == 1) return 1;
    if(Option1.Derivative_Inputs() == 1) return 1;

    /*
    Option1.Mu = 0.1;
    Option1.Sigma = 0.3;
    Option1.S0 = 100;
    Option1.r = 0.05;
    Option1.T = 0.0833333333333333333;
    Option1.Put_or_Call = 0;
    Option1.N = 3;
    Option1.K = 50;
    Option1.Upper_Bound = 50;
    Option1.Lower_Bound = 30;
     */

    Option1.Option_Parameters();

    cout << "R is: " << Option1.R << endl;
    cout << "U is: " << Option1.U << endl;
    cout << "D is: " << Option1.D << endl;

    // Calculate risk neutral probabilities

    Option1.Risk_Neutral();

    // Creating (N+1) by N^2 array with appropriate partitions to identify UD for each node
    Option1.No_MC_UD_Array();

    // Deriving RN array from UD Array
    Option1.RN_Array();

    // Creating a probability tree
    Option1.P_Array();

    // Creating array of stock prices for each node
    Option1.Stock_Array();

    // Boundary European Call Price Calculation

    Option1.EurPayoff();
    Option1.EurBinOptPrice();

    cout << "The price of the European boundary option under the Binomial model is: " << Option1.Eur_Bin_Price << endl;

    // American Call Price Calculation

    Option1.AmPayoff();
    Option1.AmBinOptPrice();

    cout << "The price of the American boundary option under the Binomial model is: " << Option1.Am_Bin_Price << endl;

    // Graceful Exit
    return 0;
}