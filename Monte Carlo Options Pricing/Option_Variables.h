#ifndef COURSEWORK_4_OPTION_VARIABLES_H
#define COURSEWORK_4_OPTION_VARIABLES_H
#include <iostream>
#include <cmath>
#include <string>

using namespace std;

class Option_Variables {

public:

    string Put_or_Call, Is_MC;
    double Mu, Sigma, r, S0, K, T;

    int N, array_size;

    double tau;

    void Obtain_Inputs();
    int Is_Monte_Carlo();

    //debugger
    void Debugger();

};

#endif //COURSEWORK_4_OPTION_VARIABLES_H
