#ifndef COURSEWORK_4_BINOMIAL_PRICING_H
#define COURSEWORK_4_BINOMIAL_PRICING_H

#include <iostream>
#include <cmath>
#include <string>
#include <vector>

#include "Option_Variables.h"

using namespace std;

class Vanilla_Binomial_Pricing : virtual public Option_Variables {

public:

    double R, U, D;
    double RN_Prob;

    double Bin_Call_Price, Bin_Put_Price;

    // Input Validity Checking
    int Check_Input_Validity();

    // Model Alterations
    void Obtain_Bin_Inputs();

    // Standard calculation procedures (EUR)
    void Risk_Neutral_Probability();

    // Create vectors
    vector <string> Array_UD;
    vector <double> Array_Probabilities;
    vector <double> Array_Sn;
    vector <double> Array_Payoff;
    vector <double> Am_Array_Payoff;

    void Calc_Array_UD();
    void Calc_Array_Probabilities(int time_step_nn);
    void Calc_Array_Sn(int time_step_n);

    // Am and Eur Option Put Call Payoffs

    double Vanilla_Call_Payoff(double Sn);
    double Vanilla_Put_Payoff(double Sn);
    double Expected_Payoff();

    void Calc_Array_Eur_Payoff();
    void Calc_Array_Am_Payoff();

    // discounting function
    double discounting(double Expected_Payoff, double disc_period);
    double temp_Put_Call_Price;

    // EUR Option Pricing
    void Eur_Option_Pricing();

    // Standard calculation procedures (AM)
    double Am_Option_Pricing(double Put_Call_Price);

    // Standard calculation procedures (Asian)


    // Standard calculation procedures (Barrier)

};

#endif //COURSEWORK_4_BINOMIAL_PRICING_H
