#include "pricing.h"

using namespace std;

double Option::Option_Inputs() {

    cout << "How many time steps, M, are there through option expiration: ";
    cin >> N;

    cout << "What is the drift, Mu: ";
    cin >> Mu;

    cout << "What is the volatility, Sigma: ";
    cin >> Sigma;

    cout << "What is todays stock price, S0: ";
    cin >> S0;

    cout << "What is the risk free rate, r: ";
    cin >> r;

    cout << "What is the expiration date, T: ";
    cin >> T;

    cout << "Please provide the upper bound: ";
    cin >> Upper_Bound;

    cout << "Please provide the lower bound: ";
    cin >> Lower_Bound;

    cout << "Are we looking to compute a call or a put? Enter 0 to compute a call, and 1 to compute a put: ";
    cin >> Put_or_Call;

}

double Option::Option_Parameters() {

    // Calculate size of vectors

    x = N + 1;
    y = pow(2, N);

    // Calculate R, U, and D

    R = 1 + (r * T) / N;
    U = R * exp(Sigma * sqrt(T) / sqrt(N));
    D = R * exp(-1 * Sigma * sqrt(T) / sqrt(N));

    // Checking for validity

    if (S0 <= 0 || U <= -1 || D <= -1 || N < 0) {

        cout << "Invalid inputs entered. Terminating Program!" << endl;

        return 1;

    }

    if (D >= U || R >= U) {

        cout << "Arbitrage exists! Terminating Program!" << endl;

        return 1;

    }

}

double Option::Risk_Neutral() {

    rn_P = (R - D) / (U - D);

}

// Create N x 2^N array with all up-down permutations

void Option::No_MC_UD_Array() {

    Array_UD.resize(x, vector<double> (y));

    double temp_UD = U;

    // filling in columns 1 through N^2 - 1 of array

    for (int i = 0; i < x; i++) {

        int N_Partitions = pow(2,i);
        int Size_of_Partition = (y / N_Partitions);

        for (int j = 0; j < N_Partitions; j++) {

            for (int k = 0; k < Size_of_Partition; k++) {

                Array_UD[i][(j * Size_of_Partition) + k] = temp_UD;

            }

            if (temp_UD == U) temp_UD = D;
            else if (temp_UD == D) temp_UD = U;

        }

    }

    // Filling in column 0 in array

    for (int i = 0; i < y; i++) Array_UD[0][i] = 1;

}

// Create x x y array to compute and store risk-neutral probabilities for every node

void Option::RN_Array() {

    Array_RN.resize(x, vector<double> (y));

    for (int i = 0; i < x; i++) {

        for (int j = 0; j < y; j++) {

            if (Array_UD[i][j] == U) Array_RN[i][j] = rn_P;
            if (Array_UD[i][j] == D) Array_RN[i][j] = (1 - rn_P);

        }

    }

    // Filling in column 0 in array

    for (int i = 0; i < y; i++) Array_RN[0][i] = 1;

}

// Create new array with probabilities for each node

void Option::P_Array() {

    Array_P.resize(x, vector<double> (y));

    // Filling in column 0 in array

    for (int i = 0; i < y; i++) Array_P[0][i] = 1;

    // filling in columns 1 through N^2 - 1 of array

    for (int i = 1; i < x; i++) {

        for (int j = 0; j < y; j++) {

            Array_P[i][j] = (Array_RN[i][j]) * (Array_P[i-1][j]);

        }

    }

}

// Create N x 2^N array to compute and store stock prices for every node

void Option::Stock_Array() {

    Array_Stock.resize(x, vector<double> (y));

    // Filling in column 0 in array

    for (int i = 0; i < y; i++) Array_Stock[0][i] = S0;

    // filling in columns 1 through N^2 - 1 of array

    for (int i = 1; i < x; i++) {

        for (int j = 0; j < y; j++) {

            Array_Stock[i][j] = (Array_Stock[i-1][j]) * (Array_UD[i][j]);

        }

    }

}

double Option::Derivative_Inputs() {

    cout << "What is the strike price, X: ";
    cin >> K;

    if (K < 0) {

        cout << "Invalid input! Terminating program!" << endl;
        return 1;

    }

}

double discounting(double &PV_Price, double R, int N) {

    PV_Price = PV_Price / pow(R, (N));

}

void BinPrice::EurPayoff(){

    Array_Eur_Payoff.resize(1, vector<double> (y,0));

    if (Put_or_Call == 0) {

        for (int i = 0; i < y; i++) {

            if (Array_Stock[x - 1][i] > K && Array_Stock[x - 1][i] < Upper_Bound) {

                Array_Eur_Payoff[0][i] = Array_Stock[x - 1][i] - K;

            }

        }
    }

    if (Put_or_Call == 1) {

        for (int i = 0; i < y; i++) {

            if (Array_Stock[x - 1][i] < K && Array_Stock[x - 1][i] > Upper_Bound) {

                Array_Eur_Payoff[0][i] = K - Array_Stock[x - 1][i];

            }

        }

    }

}



double BinPrice::EurBinOptPrice() {

    Eur_Bin_Price = 0;

    for (int i = 0; i < y; i++) {

        Eur_Bin_Price = Eur_Bin_Price + (Array_P[x - 1][i]) * (Array_Eur_Payoff[0][i]);

    }

    discounting(Eur_Bin_Price, R, N);

}

void BinPrice::AmPayoff(){

    Array_Am_Payoff.resize(x, vector<double> (y,0));

        // Filling in payoffs for each time period (Si - K)

        for (int j = 0; j < x; j++) {

            for (int i = 0; i < y; i++) {

                if (Put_or_Call == 0) {

                    if (Array_Stock[j][i] > K && Array_Stock[j][i] < Upper_Bound) {

                        Array_Am_Payoff[j][i] = Array_Stock[j][i] - K;

                    }
                }

                if (Put_or_Call == 1) {

                    if (Array_Stock[j][i] < K && Array_Stock[j][i] > Lower_Bound) {

                        Array_Am_Payoff[j][i] = K - Array_Stock[j][i];

                    }
                }

            }
        }

        // Determining prices for each period given early exercise possibility

        for (int i = 0; i < x - 1; i++) {

            int N_Partitions = pow(2,x - 2 - i);
            int Size_of_Partition = (y / N_Partitions);

            for (int j = 0; j < N_Partitions; j++) {

                double temp_PV_Payoff = 0;

                for (int k = 0; k < Size_of_Partition; k++) {

                    temp_PV_Payoff = temp_PV_Payoff + (Array_Am_Payoff[x-1-i][j * Size_of_Partition + k]) * Array_RN[x-1-i][j * Size_of_Partition + k];

                }

                temp_PV_Payoff = (temp_PV_Payoff / (R * (Size_of_Partition / 2)));

                if (x-2-i >= 0) {

                    for (int l = 0; l < (2 * Size_of_Partition); l++) {

                        if (temp_PV_Payoff > (Array_Am_Payoff[x - 2 - i][j * Size_of_Partition * 2 + l])) {

                            Array_Am_Payoff[x - 2 - i][j * Size_of_Partition + l] = temp_PV_Payoff;

                        }

                    }
                }

            }

        }

}

double BinPrice::AmBinOptPrice() {

    Am_Bin_Price = Array_Am_Payoff[0][0];

}

// Black Scholes Pricing Model

double BSPrice::BS_d1() {

    d1 = (log(S0 / K) + (r + 0.5 * pow(Sigma, 2)) * T) / (Sigma * sqrt(T));

}

double BSPrice::BS_d2() {

    d2 = (log(S0 / K) + (r - 0.5 * pow(Sigma, 2)) * T) / (Sigma * sqrt(T));

}

double BSPrice::BSCall() {

    BS_Price = (S0 * NormDist(d1)) - (K * exp(-1 * r * T) * NormDist(d2));

}

double BSPrice::BSPut() {

    BS_Price = (K * exp(-1 * r * T) * NormDist(-1 * d2)) - (S0 * NormDist(-1 * d1));

}

double BSPrice::BSPricer() {

    BS_d1();
    BS_d2();

    if (Put_or_Call == 0) BSCall();
    if (Put_or_Call == 1) BSPut();

}

double BSPrice::NormDist(double norm_x) {

    const double b1 =  0.319381530;
    const double b2 = -0.356563782;
    const double b3 =  1.781477937;
    const double b4 = -1.821255978;
    const double b5 =  1.330274429;
    const double p  =  0.2316419;
    const double c  =  0.39894228;

    if (norm_x >= 0.0) {

        double t = 1.0 / ( 1.0 + p * norm_x );
        return (1.0 - c * exp(-norm_x * norm_x / 2.0) * t * (t* (t* (t* (t * b5 + b4) + b3) + b2) + b1));

    }

    else {

        double t = 1.0 / ( 1.0 - p * norm_x );
        return (c * exp(-norm_x * norm_x / 2.0) * t * (t* (t* (t* (t * b5 + b4) + b3) + b2) + b1));

    }

}

// Calculate replicating strategies

double BSPrice::BSDeltaHedging() {

    if (Put_or_Call == 0) {

        xcall = NormDist(d1);
        ycall = BS_Price - (xcall * S0);

    }

    if (Put_or_Call == 1) {

        xput = NormDist(-1 * d1);
        yput = BS_Price - (xput * S0);

    }

}

