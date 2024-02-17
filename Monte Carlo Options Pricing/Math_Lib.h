#ifndef MAIN_CPP_MATH_LIB_H
#define MAIN_CPP_MATH_LIB_H

#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include "Math_Lib.h"

double Norm_Dist(double norm_x);
double TrapezoidIntegrate(double from_a, double to_b, double N_periods ,double (*on_function_f)(double x));
double SimpsonIntegrate();
double random_prob();
char random_UD(double random_prob);

double Norm_Invs_X(double norm_x_is_random_prob, double norm_y_is_random_prob);
double Norm_Invs_Y(double norm_x_is_random_prob, double norm_y_is_random_prob);

#endif //MAIN_CPP_MATH_LIB_H
