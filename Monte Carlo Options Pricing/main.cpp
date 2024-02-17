#include <iostream>
#include <cmath>
#include <string>
#include "Option_Variables.h"
#include "Binomial_Pricing.h"
#include "BS_Pricing.h"
#include "Option_Pricing.h"
#include "Math_Lib.h"

using namespace std;

int main() {

    Price_Options Option1;

    //Obtain Inputs
    //Option1.Obtain_Inputs();

    Option1.Debugger();
    Option1.Is_Monte_Carlo();

    /*
    //Obtain Binomial Model Inputs
    Option1.Obtain_Bin_Inputs();

    //Binomial check inputs
    if (Option1.Check_Input_Validity() == 1) return 1;

    //Price EUR Options
    Option1.Risk_Neutral_Probability();
    Option1.Calc_Array_UD();
    Option1.Eur_Option_Pricing();

    if (Option1.Put_or_Call == "CALL") {

        cout << "The price of the Call option under the Binomial Model is: " << Option1.Bin_Call_Price << endl;

    }

    if (Option1.Put_or_Call == "PUT") {

        cout << "The price of the Put option under the Binomial Model is: " << Option1.Bin_Put_Price << endl;

    }

    // Closed-form black scholes

    //Black Scholes Calculate D
    Option1.Calculate_D(Option1.S0, 0);

    cout << "d+ = " << Option1.d_plus << endl;
    cout << "d- = " << Option1.d_minus << endl;

    //Put or Call for Vanilla BS Model

    Option1.Vanilla_BS_Call_Option(Option1.S0, 0);
    Option1.Vanilla_BS_Put_Option(Option1.S0, 0);

    if (Option1.Put_or_Call == "CALL") {

        cout << "The price of the Call option at time 0 under the BS model is: " << Option1.BS_Call_Price << endl;

    }

    if (Option1.Put_or_Call == "PUT") {

        cout << "The price of the Put option at time 0 under the BS model is: " << Option1.BS_Put_Price << endl;

    }

    //Put or Call for MC BS Model

    // Initializing stock prices S0, at time t = 0
    Option1.Vanilla_BS_Pricing::Array_St.resize(Option1.array_size, Option1.S0);

    // Calculating stock prices at time T
    Option1.MC_BS_St_Array(Option1.T);

    // Obtain payoffs at time T
    Option1.MC_BS_Payoff_Array(0);

    // Return Option price

    double temp_option_price = Option1.Discount(0, Option1.T, Option1.Expectation_Payoff());

    if (Option1.Put_or_Call == "CALL") {

        Option1.BS_Call_Price = temp_option_price;

        cout << "The price of the Call option at time 0 under the BS model using MC simulation is: "
             << Option1.BS_Call_Price << endl;

    }

    if (Option1.Put_or_Call == "PUT") {

        Option1.BS_Put_Price = temp_option_price;

        cout << "The price of the Put option at time 0 under the BS model using MC simulation is: "
             << Option1.BS_Put_Price << endl;

    }

    return 0;

    // Finite Difference Method Black Scholes

    // Obtain inputs for FDM

    if (Option1.Obtain_Grid_Inputs() == 1) return 1;

    // Develop stock price array and payoff arrays for time T

    Option1.St_Array();
    Option1.Payoff_Array();

    Option1.Payoff_Calc();

    cout << Option1.temp_payoff << endl;

      */

    // Asian Call Options Pricing

    Option1.MC_BS_Asian_Average_Array(0,Option1.T);
    Option1.temp_payoff = Option1.Expectation_Payoff();

    if (Option1.Put_or_Call == "CALL") {

        Option1.BS_Call_Price = Option1.temp_payoff;

        cout << "The price of an Asian Call option at time 0 under the BS model using MC simulation is: " << Option1.BS_Call_Price << endl;

    }

    if (Option1.Put_or_Call == "PUT") {

        Option1.BS_Put_Price = Option1.temp_payoff;

        cout << "The price of an Asian Put option at time 0 under the BS model using MC simulation is: " << Option1.BS_Put_Price << endl;

    }

}

}