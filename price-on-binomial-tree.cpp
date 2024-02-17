#include <iostream>
#include <cmath>
#include <string>

using namespace std;

// Request for inputs

double InputRequest (double &U, double &D, double &S0, double &R) {

    cout << "Please input S0: ";
    cin >> S0;

    cout << "Please input U: ";
    cin >> U;

    cout << "Please input D: ";
    cin >> D;

    cout << "Please input R: ";
    cin >> R;

    // Check for Valid Inputs

    if (S0 <= 0 || U <= -1 || D <= -1) {

        cout << "Invalid inputs entered. Terminating program." << endl;
        return 1;

    }

    // Check for Arbitrage

    if (R >= U || D >= R) {

        cout << "Arbitrage exists. Terminating program." << endl;
        return 1;

    }

    return 0;

}

// Obtaining Option-specific specifications

int specifications (int &N, double &K, string &put_call) {

    cout << "How many time steps are there for this option: ";
    cin >> N;

    if (N < 0) {

        cout << "Invalid option specifications entered. Terminating program." << endl;
        return 1;

    }

    cout << "Please input the strike price, K: ";
    cin >> K;
    cout << endl;

    if (K < 0) {

        cout << "Invalid option specifications entered. Terminating program." << endl;
        return 1;

    }

    cout << "Is the option a put or call option? Please enter 'put' or 'call': ";
    cin >> put_call;

    if (put_call == "put" || put_call == "call") return 0;

    cout << "Invalid option specifications entered. Terminating program." << endl;

    return 1;

}

// Risk neutral probability calculation

double RiskNeutral (double U, double D, double R) {

    return (R-D) / (U-D);

}

// Step-wise american binomial option pricing

double AmOptionPricing (double current_payoff, double payoff_up, double payoff_down, double RiskNeutralProb, double R) {

    double ev_next_step_payoff = ((payoff_up * RiskNeutralProb) + (payoff_down * (1 - RiskNeutralProb))) / (1 + R);

    if (ev_next_step_payoff > current_payoff) {

        current_payoff = ev_next_step_payoff;

    }

    return current_payoff;

}

// Step-wise european binomial option pricing

double EuOptionPricing (double payoff_up, double payoff_down, double RiskNeutralProb, double R) {

    double ev_next_step_payoff = ((payoff_up * RiskNeutralProb) + (payoff_down * (1 - RiskNeutralProb))) / (1 + R);

    return ev_next_step_payoff;

}

// Replicating strategy

double ReplicatingPortfolio (double stock_up, double stock_down, double option_up, double option_down,
                             double R, double &replicating_stock, double &replicating_bond) {

        replicating_stock = (option_up - option_down) / (stock_up - stock_down);

        replicating_bond = ((replicating_stock * stock_up) - option_up) / (1 + R);

}

int main() {

    double U, D, S0, R, K;

    int N;

    string put_call;

    int time_step_m, node_i;

    if (InputRequest(U, D, S0, R) == 1) return 1;

    cout << endl;

    double RiskNeutralProb = RiskNeutral(U, D, R);

    if (specifications(N, K, put_call) == 1) return 1;

    cout << endl;

    // Creating lattice of stock prices

    double stock_array[N+1][N+1];

    stock_array[0][0] = S0;

    for (int i = 1; i <= N; i++) {

        stock_array[i][0] = ((1 + U) * stock_array[i-1][0]);

        for (int j = 1; j <= i; j++) {

            stock_array[i][j] = ((1 + D) * stock_array[i-1][j-1]);

        }

    }

    // Creating lattice of payoffs (for American Options only)

    double payoff_array[N+1][N+1];

    if (put_call == "call") {

        for (int i = 0; i <= N; i++) {

            for (int j = 0; j <= i; j++) {

                payoff_array[i][j] = stock_array[i][j] - K;

                if (payoff_array[i][j] <= 0) {

                    payoff_array[i][j] = 0;

                }

            }

        }

    }

    if (put_call == "put") {

        for (int i = 0; i <= N; i++) {

            for (int j = 0; j <= i; j++) {

                payoff_array[i][j] = K - stock_array[i][j];

                if (payoff_array[i][j] <= 0) {

                    payoff_array[i][j] = 0;

                }

            }

        }

    }

    // Creating lattice of european option prices

    double option_array[N+1][N+1];

    for (int i = 0; i <= N; i++) {

        option_array[N][i] = payoff_array[N][i];

    }

    for (int i = (N - 1); i >= 0; i--) {

        for (int j = 0; j <= i; j++) {

            option_array[i][j] = EuOptionPricing (option_array[i+1][j], option_array[i+1][j+1], RiskNeutralProb, R);

        }

    }


    // Calculating replicating strategy

    double replicating_stock_array[N+1][N+1];
    double replicating_bond_array[N+1][N+1];

    double replicating_stock, replicating_bond;

    for (int i = 0; i < (N-1); i++) {

        for (int j = 0; j <= i; j++) {

            ReplicatingPortfolio (stock_array[i+1][j], stock_array[i+1][j+1], option_array[i+1][j], option_array[i+1][j+1], R, replicating_stock, replicating_bond);

            replicating_stock_array[i][j] = replicating_stock;
            replicating_bond_array[i][j] = replicating_bond;

        }

    }

    // Printing option prices and corresponding replicating strategy

    cout << "Which node are you looking to calculate the option price and replicating strategy for? Please enter time step m and node i." << endl;
    cout << "Time step m: ";
    cin >> time_step_m;

    cout << "Node i: ";
    cin >> node_i;

    cout << endl;

    cout << "Option price at time " << time_step_m << " and node " << node_i << " is: " << option_array[time_step_m][node_i] << endl;

    cout << "Replicating strategy at time " << time_step_m << " and node " << node_i << " is: " << endl;
    cout << "Units of stock: " << replicating_stock_array[time_step_m][node_i] << endl;
    cout << "Value of bond: " << replicating_bond_array[time_step_m][node_i] << endl;

}