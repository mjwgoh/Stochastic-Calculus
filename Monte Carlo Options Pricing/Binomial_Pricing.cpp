#include "Binomial_Pricing.h"

void Vanilla_Binomial_Pricing::Obtain_Bin_Inputs() {

    /*
    R = 1 + (r * T) / N;
    U = R * exp((Sigma * sqrt(T)) / sqrt(N));
    D = R * exp(-1 * (Sigma * sqrt(T)) / sqrt(N));
     */

    R = 1 + r;
    U = 1.3;
    D = 0.9;

}

int Vanilla_Binomial_Pricing::Check_Input_Validity() {

    if (S0 <= 0 || U <= -1 || D <= -1 || N < 0) {

        cout << "Invalid inputs entered. Terminating Program!" << endl;

        return 1;

    }

    if (D >= U || R >= U) {

        cout << "Arbitrage exists! Terminating Program!" << endl;

        return 1;

    }

}

void Vanilla_Binomial_Pricing::Risk_Neutral_Probability() {

    // Calculates the Risk-Neutral Probability

    RN_Prob = (R-D) / (U-D);

}

double Vanilla_Binomial_Pricing::Vanilla_Call_Payoff(double Sn) {

    if (Sn > K) return (Sn - K);
    return 0;

}

double Vanilla_Binomial_Pricing::Vanilla_Put_Payoff(double Sn) {

    if (Sn < K) return (K - Sn);
    return 0;

}

// Here we generate a y-sized array with strings corresponding
// to the path that the underlying asset takes in the binomial model

void Vanilla_Binomial_Pricing::Calc_Array_UD() {

    Array_UD.resize(array_size, "S");

    for (int i = 1; i <= N; i++) {

        char temp_UD = 'U';
        int temp_partition_size = (array_size / pow(2, i));

        for (int j = 0; j < (array_size / temp_partition_size); j++) {

            for (int k = 0; k < temp_partition_size; k++) {

                string temp_string = Array_UD[(j * temp_partition_size) + k];

                temp_string += temp_UD;

                Array_UD[(j * temp_partition_size) + k] = temp_string;

                }

            if (temp_UD == 'U') temp_UD = 'D';
            else temp_UD = 'U';

            }

        }

    }

// Here we generate a y-sized array with probabilities for each node
// based on the path that the underlying asset takes

void Vanilla_Binomial_Pricing::Calc_Array_Probabilities(int time_step_n) {

    // Resize arrays. array_size = N^2 or number of simulations
    Array_Probabilities.resize(array_size, 1);

    for (int i = 0; i < array_size; i++) {

        string temp_string = Array_UD[i];

        for (int j = 1 ; j <= time_step_n; j++) {

            char temp_string_char = temp_string.at(j);

            if (temp_string_char == 'U') {

                Array_Probabilities[i] = Array_Probabilities[i] * RN_Prob;

            }

            else if (temp_string_char == 'D') {

                Array_Probabilities[i] = Array_Probabilities[i] * (1 - RN_Prob);

            }

        }

    }

}

// Here we generate a y-sized array with stock prices for each node
// based on the path that the underlying asset takes

void Vanilla_Binomial_Pricing::Calc_Array_Sn(int time_step_n) {

    // Resize arrays to size y. y = N^2 or number of simulations
    Array_Sn.resize(array_size, S0);

    for (int i = 0; i < array_size; i++) {

        string temp_string = Array_UD[i];

        for (int j = 1 ; j <= time_step_n; j++) {

            char temp_string_char = temp_string.at(j);

            if (temp_string_char == 'U') {

                Array_Sn[i] = Array_Sn[i] * U;

            }

            else if (temp_string_char == 'D') {

                Array_Sn[i] = Array_Sn[i] * D;

            }

        }

    }

}

// Here we generate a y-sized array with payoff for each node
// based on the stock prices generated in the previous array

// Calculate payoffs for European Options

void Vanilla_Binomial_Pricing::Calc_Array_Eur_Payoff() {

    Array_Payoff.resize(array_size, 0);

    if (Put_or_Call == "CALL") {

        for (int i = 0; i < array_size; i++) {

            double Sn = Array_Sn[i];
            Array_Payoff[i] = Vanilla_Call_Payoff(Sn);

        }
    }

    if (Put_or_Call == "PUT") {

        for (int i = 0; i < array_size; i++) {

            double Sn = Array_Sn[i];
            Array_Payoff[i] = Vanilla_Put_Payoff(Sn);

        }
    }
}

double Vanilla_Binomial_Pricing::Expected_Payoff() {

    double expected_payoff = 0;

    for (int i = 0; i < array_size; i++) {

        expected_payoff = expected_payoff + (Array_Payoff[i] * Array_Probabilities[i]);

    }

    return expected_payoff;

}

double Vanilla_Binomial_Pricing::discounting(double Expected_Payoff, double disc_period) {

    temp_Put_Call_Price = Expected_Payoff / pow(R, N);

}

// Here, we calculate Option prices based on payoffs derived
// Note that for EurOption prices, we need to set time_step_n = N

void Vanilla_Binomial_Pricing::Eur_Option_Pricing() {

    Calc_Array_Probabilities(N);
    Calc_Array_Sn(N);

    Calc_Array_Eur_Payoff();
    discounting(Expected_Payoff(), T);
    Bin_Call_Price = temp_Put_Call_Price;

}

// Here, we calculate American Option prices using the payoff array

void Vanilla_Binomial_Pricing::Calc_Array_Am_Payoff() {

    Am_Array_Payoff.resize(array_size, 0);

    if (Put_or_Call == "CALL") {

        for (int i = 0; i < array_size; i++) {

            double Sn = Array_Sn[i];
            Am_Array_Payoff[i] = Vanilla_Call_Payoff(Sn);

        }
    }

    if (Put_or_Call == "PUT") {

        for (int i = 0; i < array_size; i++) {

            double Sn = Array_Sn[i];
            Am_Array_Payoff[i] = Vanilla_Put_Payoff(Sn);

        }
    }
}

double Vanilla_Binomial_Pricing::Am_Option_Pricing(double Put_Call_Price) {

    double discount_factor = 1 / R;

    Calc_Array_Sn(N);
    Calc_Array_Am_Payoff();

    for (int time_step_n = N; time_step_n > 0; time_step_n--) {

        int temp_partition_size = (array_size / pow(2, time_step_n - 1));

        // TODO: ENSURE THAT THERE TIME_STEP_N - 1 IS VALID AND DOES NOT EXCEED ARRAY. USE IF FORMULA

        Calc_Array_Sn(time_step_n - 1);
        Calc_Array_Eur_Payoff();

        // Discounting payoffs with RN_probabilities and risk-free rate

        for (int i = 0; i < array_size; i++) {

            string temp_string = Array_UD[i];
            char temp_string_char = temp_string.at(time_step_n);

            if (temp_string_char == 'U') Am_Array_Payoff[i] = Am_Array_Payoff[i] * RN_Prob * discount_factor;
            if (temp_string_char == 'D') Am_Array_Payoff[i] = Am_Array_Payoff[i] * (1 - RN_Prob) * discount_factor;

        }

        // Calculating expected option value for time T

        for (int i = 0; i < (N / temp_partition_size); i++) {

            double temp_expected_payoff = 0;

            for (int j = 0; j < temp_partition_size; j++) {

                temp_expected_payoff = temp_expected_payoff + (Am_Array_Payoff[(i * temp_partition_size) + j] / temp_partition_size);

            }

            for (int j = 0; j < temp_partition_size; j++) {

                Am_Array_Payoff[(i * temp_partition_size) + j] = temp_expected_payoff;

            }

        }

        // If EUR Option Payoffs are greater, replace Am Array Payoffs

        for (int i = 0; i < array_size; i++) {

            if (Am_Array_Payoff[i] < Array_Payoff[i]) Am_Array_Payoff[i] = Array_Payoff[i];

        }

    }

    Put_Call_Price = Am_Array_Payoff[0];

}

// Create function to calculate Asian Option prices using payoff array

