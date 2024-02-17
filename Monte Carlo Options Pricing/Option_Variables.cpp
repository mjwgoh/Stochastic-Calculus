#include "Option_Variables.h"

using namespace std;

void Option_Variables::Obtain_Inputs() {

    cout << "What is the underlying asset price at time 0, S0: ";
    cin >> S0;
    cout << endl;

    cout << "What is the risk-free rate, r: ";
    cin >> r;
    cout << endl;

    cout << "What is the number of time steps, N: ";
    cin >> N;
    cout << endl;

    cout << "What is the volatility of the underlying, Sigma: ";
    cin >> Sigma;
    cout << endl;

    cout << "What is the drift of the underlying, Mu: ";
    cin >> Mu;
    cout << endl;

    cout << "What is the strike price of the option, K: ";
    cin >> K;
    cout << endl;

    cout << "What is the expiration date of the option, T: ";
    cin >> T;
    cout << endl;

    tau = T / N;

    cout << "Are you trying to price a put or a call option? If Put, enter 'PUT'. If Call, enter 'CALL': ";
    cin >> Put_or_Call;
    cout << endl;

    cout << "Would we like to conduct a Monte Carlo simulation? If Yes, enter 'YES'. If No, enter 'NO': ";
    cin >> Is_MC;
    cout << endl;

}

void Option_Variables::Debugger() {

    S0 = 100;
    r = 0.05;
    N = 2000;
    T = 0.0833333333333;
    Sigma = 0.2;
    Mu = 0.15;
    K = 100;
    Put_or_Call = "CALL";
    Is_MC = "YES";

    tau = T / N;

}

int Option_Variables::Is_Monte_Carlo() {

    if (Is_MC == "YES") {

        cout << "How many simulations would you like to run: ";
        cin >> array_size;

        return 0;

    }

    if (Is_MC == "NO") {

        array_size = pow(2, N);
        return 0;

    }

    return 1;

}