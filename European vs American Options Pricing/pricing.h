#ifndef QUESTION_1_PRICING_H
#define QUESTION_1_PRICING_H

#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

double Type_Of_Derivative();

class Option {

public:

    double S0, Mu, Sigma, r, T, M, K;
    int N;

    double R;
    double U, D;

    double rn_P;
    int x, y; // array sizes

    int Put_or_Call; // Put or call option computation

    // Initializing arrays to store stock-related information
    vector <vector<double>> Array_UD;
    vector <vector<double>> Array_RN;
    vector <vector<double>> Array_Stock;
    vector <vector<double>> Array_P;

    double Option_Inputs();
    double Option_Parameters();
    double Risk_Neutral();

    void No_MC_UD_Array();
    void RN_Array();
    void P_Array();
    void Stock_Array();

    // Obtain strike price, K
    double Derivative_Inputs();

};

class BinPrice : public virtual Option {

public:

    // European Options

    vector <vector<double>> Array_Eur_Payoff;
    double Eur_Bin_Price;

    // Payoff Calculation
    void EurPayoff();
    double EurBinOptPrice();

    // Boundary Payoff Calculation
    void EurBoundaryPayoff();

    // American Options

    vector <vector<double>> Array_Am_Payoff;
    double Am_Bin_Price;

    // Payoff Calculation
    void AmPayoff();
    double AmBinOptPrice();

};

class BSPrice : public virtual Option {

public:

    double d1, d2;
    double BS_Price;

    double BS_d1();
    double BS_d2();

    // Put and Call Pricing

    double BSPut();
    double BSCall();
    double BSPricer();

    double NormDist(double norm_x);

    // Delta Hedging
    double xcall, ycall;
    double xput, yput;

    double BSDeltaHedging();

};

// Calculate option price (Need to change to pointers)

class DerivativesPricing : public BinPrice, public BSPrice {

};

#endif //QUESTION_1_PRICING_H