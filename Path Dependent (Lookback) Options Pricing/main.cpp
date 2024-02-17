#include <iostream>
#include <cmath>
#include <string>
#include <ctime>
#include <chrono>
#include "pricing.h"

using namespace std;

int main() {

    srand(time(NULL));

    // Obtain inputs

    DerivativesPricing Option1;

    if(Option1.Option_Inputs() == 1) return 1;
    if(Option1.Derivative_Inputs() == 1) return 1;
    Option1.Is_Monte_Carlo();

    // Start timer

    auto start = std::chrono::high_resolution_clock::now();

    // Calculate risk neutral probabilities

    Option1.Risk_Neutral();

    // If Monte Carlo Simulation is selected

    if (Option1.MC == 1) {

        // Creating (N+1) by M array with randomized jumps for each node
        Option1.MC_UD_Array();

    }

    // If Monte Carlo simulation is not selected

    if (Option1.MC == 0) {

        // Creating (N+1) by N^2 array with appropriate partitions to identify UD for each node
        Option1.No_MC_UD_Array();

        // Deriving RN array from UD Array
        Option1.RN_Array();

        // Creating a probability tree
        Option1.P_Array();

    }

    // Creating array of stock prices for each node
    Option1.Stock_Array();

    /* Calculating Asian Call Option Prices */

    //Creating array of payoffs for each node
    Option1.AsianCall_SK();
    Option1.AsianCall_Payoff();

    // Calculating price of asian call
    Option1.AC_Price();

    cout << "The Asian Call option price at time t0 is: " << Option1.AC_Call_Price << endl;

    /* Calculating Lookback Option Prices */

    Option1.Lookback_Payoff();
    Option1.Lookback_Price();

    cout << "The lookback option price at time t0 is: " << Option1.LB_Price << endl;

    /* Calculating Normal European Call Option Prices (for checking) */


    Option1.NormCall_Payoff();
    Option1.NormCall_Price();

    cout << "The call option price at time t0 is: " << Option1.Call_Price << endl;

    // Stop Timer

    auto stop = std::chrono::high_resolution_clock::now();

    // Calculate and output duration

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    cout << "The runtime is: " << duration.count() << " microseconds." << endl;

    // Output Standard Errors

    cout << "The Standard Error for the Asian Call is: " << Option1.AC_SE << endl;
    cout << "The Standard Error for the Lookback Option is: " << Option1.LB_SE << endl;

    // Graceful Exit
    return 0;
}