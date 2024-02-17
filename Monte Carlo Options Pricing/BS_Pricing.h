#ifndef COURSEWORK_4_BS_PRICING_H
#define COURSEWORK_4_BS_PRICING_H

#include <iostream>
#include <cmath>
#include <string>
#include <vector>

#include "Option_variables.h"
#include "Math_Lib.h"

class Vanilla_BS_Pricing : virtual public Option_Variables {

public:

    double d_plus, d_minus;

    double BS_Call_Price, BS_Put_Price;

    // Generate D with period (T-t)
    void Calculate_D(double Sn, double start_t);

    //Overloading function: Obtains S(t)
    double St(double St, double dt, double norm_x, double norm_y);

    // Price calls and puts with period (T-t)
    void Vanilla_BS_Call_Option(double Sn, double start_t);
    void Vanilla_BS_Put_Option(double Sn, double start_t);

    // Payoff Types
    double Payoff(double St);

    // Price Monte Carlo Options
    vector <double> Array_St;
    vector <double> Array_St_2;
    vector <double> Array_Payoff;
    vector <double> Array_Payoff_2;

    // Calculating stock array for vanilla options
    void MC_BS_St_Array(double change_in_t);

    // Calculating array for payoffs
    void MC_BS_Payoff_Array(double initial_payoff);

    // Calculating array for asian option average payoffs
    void MC_BS_Asian_Average_Array(double Option_buy_date, double Option_expiration_date);

    // Calculating expectation of array
    double Expectation_Payoff();

    // calculate discounted payoff
    double Discount(double from_t, double to_T, double EV_at_T);

};

class Grid_BS_Pricing : virtual public Option_Variables {

public:

    int imin, imax;
    double smin, smax;
    int i0;

    double size_i;

    // Obtain Inputs
    int Obtain_Grid_Inputs();

    // Payoffs
    double Payoff(double St);

    // Arrays

    vector <double> Array_St;
    vector <double> Array_Payoff;
    vector <double> Array_Payoff_2;

    // Closed form pricing model
    void St_Array();
    void Payoff_Array();

    // Pricing variables

    double S(int stock_step_i);
    double a(int stock_step_i);
    double b(int stock_step_i);
    double c(int stock_step_i);

    void Payoff_Calc();

    double temp_payoff;

};

#endif //COURSEWORK_4_BS_PRICING_H
