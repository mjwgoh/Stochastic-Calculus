#include "pricing.h"

using namespace std;

double EurOption::Option_Inputs() {

    cout << "How many time steps, N, are there through option expiration: ";
    cin >> N;

    cout << "What is the discount factor, R: ";
    cin >> R;

    cout << "What is the value of U: ";
    cin >> U;

    cout << "What is the value of D: ";
    cin >> D;

    cout << "What is the initial stock price, S0: ";
    cin >> S0;

    if (S0 <= 0 || U <= -1 || D <= -1 || N < 0) {

        cout << "Invalid inputs entered. Terminating Program!" << endl;

        return 1;

    }

    if (D >= U || R >= U) {

        cout << "Arbitrage exists! Terminating Program!" << endl;

        return 1;

    }

}

double EurOption::Risk_Neutral() {

    rn_P = (R - D) / (U - D);

}

void EurOption::Is_Monte_Carlo() {

    cout << "Would you like to run a Monte Carlo simulation instead? "
            "If Yes, please enter 1. Else, enter 0: ";
    cin >> MC;

    if (MC == 1) {

        cout << "M should be smaller than " <<
        pow(2,N) << ". How many simulations, M, would you like to run: ";

        cin >> M;

        x = N;
        y = M;

    }

    else if (MC == 0) {

        x = N + 1;
        y = pow(2, N);

    }

}

// Create N x 2^N array with all up-down permutations

void EurOption::No_MC_UD_Array() {

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

// Create N x 2^N array with all up-down permutations

void EurOption::MC_UD_Array() {

    Array_UD.resize(x, vector<double> (y));

    // filling in columns 1 through N^2 - 1 of array

    for (int i = 1; i < x; i++) {

        for (int j = 0; j < y; j++) {

            double temp_double = (double) rand() / RAND_MAX;

            if (temp_double < rn_P) Array_UD[i][j] = U;
            else Array_UD[i][j] = D;

        }

    }

    // Filling in column 0 in array

    for (int i = 0; i < y; i++) Array_UD[0][i] = 1;

}

// Create x x y array to compute and store risk-neutral probabilities for every node

void EurOption::RN_Array() {

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

void EurOption::P_Array() {

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

void EurOption::Stock_Array() {

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

double EurOption::Derivative_Inputs() {

    cout << "What is the strike price, K: ";
    cin >> K;

    if (K < 0) {

        cout << "Invalid input! Terminating program!" << endl;
        return 1;

    }

}

void AsianCall::AsianCall_SK() {

    Array_AC_SK.resize(x, vector<double> (y));

    for (int i = 0; i < x; i++) {

        for (int j = 0; j < y; j++) {

            Array_AC_SK[i][j] = (Array_Stock[i][j]) - K;

        }

    }

}

double discounting(double &PV_Price, double R, int N) {

    PV_Price = PV_Price / pow(R, (N));

}

double Std_Error(double &Temp_Std_Error, int y, double R, int N) {

    Temp_Std_Error = (sqrt(Temp_Std_Error) / sqrt(y)) / pow(R, N);

}

void AsianCall::AsianCall_Payoff() {

    Array_AC_Payoff.resize(1, vector<double> (y,0));

    for (int i = 0; i < y; i++) {

        double temp_sum = 0;

        for (int j = 1; j < x; j++) {

            temp_sum = temp_sum + Array_AC_SK[j][i];

        }

        if (temp_sum > 0) {

            Array_AC_Payoff[0][i] = temp_sum / N;

        }

    }

}

double AsianCall::AC_Price() {

    AC_Call_Price = 0;
    AC_SE = 0;

    if (MC == 0) {

        for (int i = 0; i < y; i++) {

            AC_Call_Price = AC_Call_Price + (Array_P[x - 1][i]) * (Array_AC_Payoff[0][i]);

        }
    }

    if (MC == 1) {

        for (int i = 0; i < y; i++) {

            AC_Call_Price = AC_Call_Price + (Array_AC_Payoff[0][i]) / y;

        }

        for (int i = 0; i < y; i++) {

            AC_SE = AC_SE + ((((Array_AC_Payoff[0][i]) - AC_Call_Price)  * ((Array_AC_Payoff[0][i]) - AC_Call_Price)) / (y - 1));

        }


    }

    discounting(AC_Call_Price, R, N);
    Std_Error(AC_SE, y, R, x);

}

void LookbackOption::Lookback_Payoff() {

    Array_LB_Payoff.resize(1, vector<double> (y,0));

    for (int i = 0; i < y; i++) {

        double temp_min = Array_Stock[0][i];

        for (int j = 0; j < x; j++) {

            if (Array_Stock[j][i] < temp_min) temp_min = Array_Stock[j][i];

        }

        Array_LB_Payoff[0][i] = Array_Stock[x - 1][i] - temp_min;

    }

}

double LookbackOption::Lookback_Price() {

    LB_Price = 0;
    LB_SE = 0;

    if (MC == 0) {

        for (int i = 0; i < y; i++) {

            LB_Price = LB_Price + (Array_P[x - 1][i]) * (Array_LB_Payoff[0][i]);

        }
    }

    if (MC == 1) {

        for (int i = 0; i < y; i++) {

            LB_Price = LB_Price + (Array_LB_Payoff[0][i]) / y;

        }

        for (int i = 0; i < y; i++) {

            LB_SE = LB_SE + ((((Array_LB_Payoff[0][i]) - LB_Price)  * ((Array_LB_Payoff[0][i]) - LB_Price)) / (y - 1));

        }

    }

    discounting(LB_Price, R, N);
    Std_Error(LB_SE, y, R, x);

}

void NormalCallOption::NormCall_Payoff(){

    Array_Call_Payoff.resize(1, vector<double> (y,0));

    for (int i = 0; i < y; i++) {

            if (Array_Stock[x - 1][i] > K) {

                Array_Call_Payoff[0][i] = Array_Stock[x - 1][i] - K;

            }

        }

}

double NormalCallOption::NormCall_Price() {

    Call_Price = 0;

    // If non MC

    if (MC == 0) {

        for (int i = 0; i < y; i++) {

            Call_Price = Call_Price + (Array_P[x - 1][i]) * (Array_Call_Payoff[0][i]);

        }

    }

    if (MC == 1) {

        for (int i = 0; i < y; i++) {

            Call_Price = Call_Price + (Array_Call_Payoff[0][i]) / y;

        }

    }

    discounting(Call_Price, R, N);

}