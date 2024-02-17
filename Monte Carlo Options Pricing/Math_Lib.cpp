#include "Math_Lib.h"

double Norm_Dist(double norm_x) {

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

// Read through and try to understand Trapezoid integration method

double TrapezoidIntegrate(double from_a, double to_b, double N_periods ,double (*on_function_f)(double x)) {

    double h = (to_b - from_a) / N_periods;

    double TrapIntResult = 0.5 * on_function_f(from_a);

    for (int n = 1; n < N_periods; n++) {

        TrapIntResult = TrapIntResult + on_function_f(from_a + n * h);

    }

    TrapIntResult = TrapIntResult + 0.5 * on_function_f(to_b);

    return h * TrapIntResult;

}

// Read through and try to understand Simpson integration method

double SimpsonIntegrate(double from_a, double to_b, double N_periods, double (*on_function_f)(double x)) {

    double h = (to_b - from_a) / N_periods;
    double SimpIntResult = on_function_f(from_a);

    for (int n = 1; n < N_periods; n++) {

        SimpIntResult = SimpIntResult + 4 * on_function_f(from_a + n * h - 0.5 * h) + 2 * on_function_f(from_a + n * h);

    }

    SimpIntResult = SimpIntResult + 4 * on_function_f(to_b - 0.5 * h) + on_function_f(to_b);
    return SimpIntResult*h/6;

}

// Generate Random Numbers

double random_prob() {

    double rand_prob = rand() / (double)(RAND_MAX);

    return rand_prob;

}

char random_UD(double random_prob) {

    if (random_prob >= 0.5) return 'U';
    if (random_prob < 0.5) return 'D';

}

// Normal Inverse using Box Muller

double Norm_Invs_X(double norm_x_is_random_prob, double norm_y_is_random_prob) {

    double pi = 3.14159265358979311600;

    double Z; // Z ~ N(0,1) random variable

    Z = sqrt(-2 * log(norm_x_is_random_prob)) * cos(2 * pi * norm_y_is_random_prob);

    return Z;

}

double Norm_Invs_Y(double norm_x_is_random_prob, double norm_y_is_random_prob) {

    double pi = 3.14159265358979311600;

    double Z; // Z ~ N(0,1) random variable

    Z = sqrt(-2 * log(norm_x_is_random_prob)) * sin(2 * pi * norm_y_is_random_prob);

    return Z;

}
