#include "BS_Pricing.h"

// Vanilla Black Scholes Pricing

void Vanilla_BS_Pricing::Calculate_D(double St, double start_t) {

    d_plus = (log(St/K) + (r + 0.5 * pow(Sigma, 2)) * (T - start_t)) / (Sigma * sqrt(T - start_t));
    d_minus = (log(St/K) + (r - 0.5 * pow(Sigma, 2)) * (T - start_t)) / (Sigma * sqrt(T - start_t));

}

// Such that norm_x and norm_y are random probabilities between 0 and 1. Can be generated with the random_prob() function.

// For path independent options

double Vanilla_BS_Pricing::St(double St_start, double change_in_t, double norm_x, double norm_y) {

    double St;

    St = St_start * exp((r - 0.5 * Sigma * Sigma) * (change_in_t)
            + Sigma * sqrt(change_in_t) * Norm_Invs_X(norm_x, norm_y));

    return St;

}

void Vanilla_BS_Pricing::Vanilla_BS_Call_Option(double St, double start_t) {

    BS_Call_Price = St * Norm_Dist(d_plus) - K * exp(-1 * r * (T - start_t)) * Norm_Dist(d_minus);

}

void Vanilla_BS_Pricing::Vanilla_BS_Put_Option(double St, double start_t) {

    BS_Put_Price =  K * exp(-1 * r * (T - start_t)) * Norm_Dist(-1 * d_minus) - St * Norm_Dist(-1 * d_plus);

}

// Develop Monte Carlo Simulation for BS model

// Develop Sn prices for options at time T

void Vanilla_BS_Pricing::MC_BS_St_Array(double change_in_t) {

    Array_St.resize(array_size, 0);

    for (int i = 0; i < array_size; i++) {

        Array_St[i] = St(Array_St[i], change_in_t, random_prob(), random_prob());

    }

}

// Payoffs for St

double Vanilla_BS_Pricing::Payoff(double St) {

    double Payoff = 0;

    if (Put_or_Call == "CALL" && (St > K)) {

        Payoff = (St - K);

    }

    if (Put_or_Call == "PUT" && (St < K)) {

        Payoff = (K - St);

    }

    return Payoff;

}

// Develop Payoffs at time T

void Vanilla_BS_Pricing::MC_BS_Payoff_Array(double initial_payoff) {

    Array_Payoff.resize(array_size, initial_payoff);

    for (int i = 0; i < array_size; i++) {

        Array_Payoff[i] = Payoff(Array_St[i]);

    }

}

// Calculate expectation of array

double Vanilla_BS_Pricing::Expectation_Payoff() {

    double expected_value = 0;

    for (int i = 0; i < array_size; i++) {

       expected_value = expected_value + Array_Payoff[i];

    }

    expected_value = expected_value / array_size;

    return expected_value;

}

// Discount expectation

double Vanilla_BS_Pricing::Discount(double from_t, double to_T, double EV_at_T) {

    double payoff_at_zero = exp(-r * (to_T - from_t)) * EV_at_T;

    return payoff_at_zero;

}


// Develop Asian Call Average Price.
// Note that Array_St_2 is being used as the average stock price array

void Vanilla_BS_Pricing::MC_BS_Asian_Average_Array(double Option_buy_date, double Option_expiration_date) {

    Array_St.resize(array_size, S0);
    Array_Payoff_2.resize(array_size, Payoff(S0));

    for (int j = 0; j < N; j ++) {

        MC_BS_St_Array(tau);
        MC_BS_Payoff_Array(0);

        for (int i = 1; i < array_size; i++) {

            Array_Payoff_2[i] = Array_Payoff_2[i] + Array_Payoff[i];

        }
    }

    for (int i = 0; i < array_size; i++) {

        Array_Payoff_2[i] = Array_Payoff_2[i] / (N + 1);

    }

    Array_Payoff = Array_Payoff_2;

}

// Pricing using grids

int Grid_BS_Pricing::Obtain_Grid_Inputs() {

    cout << "What is the minimum stock price, smin: ";
    cin >> smin;

    cout << "What is the maximum stock price, smax: ";
    cin >> smax;

    imin = 0;

    cout << "What is the number of stock steps, imax: ";
    cin >> imax;

    cout << "Recall that the total time period, T, is: " << T << ", and the total number of time steps is: " << N <<endl;

    // Check input validity

    if (smin > smax || imin > imax) return 1;

    // Check with Capinski if the sizes below are correct. Answers are very convoluted

    // Determine size of each stock step, size_i, and the size of each time step, tau

    size_i = (smax - smin) / (imax - imin);

    cout << "The size of each stock step is: " << size_i << endl;
    cout << "The size of each time step is: " << tau << endl;

    // Determine number of stock steps, i0, for stock price S0

    i0 = S0 / size_i + imin;

    // Size of array

    array_size = imax + 1;

}

// Develop stock price array for time T

double Grid_BS_Pricing::S(int stock_step_i) {

    double temp_Si = 0;

    temp_Si = size_i * stock_step_i;

    return temp_Si;

}

void Grid_BS_Pricing::St_Array() {

    Array_St.resize(array_size, 0);

    for (int i = imin; i <= imax; i++) {

        Array_St[i] = S(i);

    }

}

// Develop Payoffs

double Grid_BS_Pricing::Payoff(double St) {

    if (Put_or_Call == "CALL" && (St > K)) {

        return St - K;

    }

    if (Put_or_Call == "PUT" && (K > St)) {

        return K - St;

    }

}

// Develop payoff arrays

void Grid_BS_Pricing::Payoff_Array() {

    Array_Payoff.resize(array_size, 0);

    for (int i = imin; i <= imax; i++) {

        Array_Payoff[i] = Payoff(Array_St[i]);

    }

}

// Calculating a, b, and c

double Grid_BS_Pricing::a(int stock_step_i) {

    double temp_a = 0;

    temp_a = (tau * 0.5 * Sigma * Sigma * S(stock_step_i) * S(stock_step_i) / size_i / size_i) - (0.5 * tau * r * S(stock_step_i) / size_i);

    return temp_a;

}

double Grid_BS_Pricing::b(int stock_step_i) {

    double temp_b = 0;

    temp_b = (1 - (tau * Sigma * Sigma * S(stock_step_i) * S(stock_step_i) / size_i / size_i) - (tau * r));

    return temp_b;

}

double Grid_BS_Pricing::c(int stock_step_i) {

    double temp_c = 0;

    temp_c = (tau * 0.5 * Sigma * Sigma * S(stock_step_i) * S(stock_step_i) / size_i / size_i) + (0.5 * tau * r * S(stock_step_i) / size_i);

    return temp_c;

}

// Payoff calculations

void Grid_BS_Pricing::Payoff_Calc() {

    Array_Payoff_2.resize(array_size, 0);

    for (int t = N - 1; t >= 0; t--) {

        for (int i = imin + 1; i < imax; i++) {

            Array_Payoff_2[i] = a(i) * Array_Payoff[i - 1] + b(i) * Array_Payoff[i] + c(i) * Array_Payoff[i + 1];

        }

        Array_Payoff_2[imax] = smax - K * exp(-r * (T - t * tau));

        Array_Payoff = Array_Payoff_2;

    }

    temp_payoff = Array_Payoff[i0];

}
