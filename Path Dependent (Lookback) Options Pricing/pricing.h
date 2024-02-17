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

class EurOption {

public:

    double S0;
    int N;

    double R;
    double U, D;

    double rn_P;
    bool MC; // Checks if user wants to run MC simulation
    int M; // M = number of Monte Carlo simulations to run

    int x, y; // array sizes

    double K;

    // Initializing arrays to store stock-related information
    vector <vector<double>> Array_UD;
    vector <vector<double>> Array_RN;
    vector <vector<double>> Array_Stock;
    vector <vector<double>> Array_P;

    double Option_Inputs();
    double Risk_Neutral();
    void Is_Monte_Carlo();

    void MC_UD_Array();
    void No_MC_UD_Array();
    void RN_Array();
    void P_Array();
    void Stock_Array();

    // Obtain strike price, K
    double Derivative_Inputs();

};

class AsianCall : public virtual EurOption {

public:

    // Initializing arrays to store derivative-related information
    vector <vector<double>> Array_AC_SK;
    vector <vector<double>> Array_AC_Payoff;

    // Calculate S(n,w(n)) - K (new array needed)
    void AsianCall_SK();

    // Calculate Payoff at time N for row of array (new 1 x 2^N array needed)
    void AsianCall_Payoff();

    // AC Variables
    double AC_Call_Price;
    double AC_SE;

    // Calculate Prices
    double AC_Price ();

};

class LookbackOption : public virtual EurOption {

public:

    // Initializing array to store Lookback Option payoffs

    vector <vector<double>> Array_LB_Payoff;
    double LB_Price;
    double LB_SE;

    // Payoff Calculation
    void Lookback_Payoff();
    double Lookback_Price();


};

class NormalCallOption : public virtual EurOption {

public:

    vector <vector<double>> Array_Call_Payoff;
    double Call_Price;

    // Payoff Calculation
    void NormCall_Payoff();
    double NormCall_Price();

};

// Calculate option price (Need to change to pointers)

class DerivativesPricing : public AsianCall, public LookbackOption, public NormalCallOption{

};

#endif //QUESTION_1_PRICING_H