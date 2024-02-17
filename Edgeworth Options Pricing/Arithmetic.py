# Import dependencies
import numpy as np
import math
import csv
from scipy.stats import norm
from scipy.stats import lognorm

# Import Parameters

inputs = open('Edgeworth Input.csv')
csv_inputs = csv.reader(inputs)

for row in csv_inputs:

    # Initialise parameters
    Simulation_Number = row[0]
    S0 = float(row[1]) # Stock price at time t = 0
    K = float(row[2]) # Fixed strike
    Sigma = float(row[3]) # Volatility of the stock
    r = float(row[4]) # Risk-free rate
    T = float(row[5]) # Time to maturity
    n = int(row[6]) # Number of time periods
    N = int(row[7]) # Number of simulations
    Type = str(row[8]) # Type of Simulation

    h = float(T / n) # Size of each time period

    # Derived inputs
    k = float(n * K / S0)

    first_term = 0.0
    second_term = 0.0
    third_term = 0.0
    fourth_term = 0.0

    Put_or_Call = 'Call'

    Price = 0.0

    # Check inputs
    if (S0 <= 0 or K < 0 or Sigma < 0 or r < 0 or T <= 0 or N <= 0):
        quit()

    # Derivatives

    def g(derivative, x, Mu, Sigma):
        gx = (1 / (x * Sigma * np.sqrt(2 * np.pi))) * np.exp(-1 * pow(np.log(x) - Mu, 2) / (2 * Sigma * Sigma))
        temp_frac = ((Mu - np.log(x)) / pow(Sigma, 2))

        if derivative == 0:
            output = gx

        if derivative == 1:
            output = gx / x * (temp_frac - 1)

        if derivative == 2:
            output = gx / pow(x, 2) * ((temp_frac - 1) * (temp_frac - 2) - (1 / pow(Sigma, 2)))

        return output

    # Moments of True Distribution

    def logmoment(n, Mu, SigmaSq):
        lm = np.exp(Mu * n + 0.5 * SigmaSq * pow(n, 2))
        return lm

    R1 = logmoment(1, (r - pow(Sigma, 2) * 0.5) * h, pow(Sigma, 2) * h)
    R2 = logmoment(2, (r - pow(Sigma, 2) * 0.5) * h, pow(Sigma, 2) * h)
    R3 = logmoment(3, (r - pow(Sigma, 2) * 0.5) * h, pow(Sigma, 2) * h)
    R4 = logmoment(4, (r - pow(Sigma, 2) * 0.5) * h, pow(Sigma, 2) * h)

    L1 = L2 = L3 = L4 = 1.0

    for i in range(n - 1):
        L4 = 1 + (R4 * L4) + (4 * R3 * L3) + (6 * R2 * L2) + (4 * R1 * L1)
        L3 = 1 + (R3 * L3) + (3 * R2 * L2) + (3 * R1 * L1)
        L2 = 1 + (R2 * L2) + (2 * R1 * L1)
        L1 = 1 + (R1 * L1)

    M1F = R1 * L1
    M2F = R2 * L2
    M3F = R3 * L3
    M4F = R4 * L4

    # Moments of Approximating Lognormal Distribution

    Mu_Approx = 2 * np.log(M1F) - np.log(M2F) / 2
    Sigma_Approx_Sq = np.log(M2F) - 2 * np.log(M1F)

    M1G = M1F
    M2G = M2F
    M3G = logmoment(3, Mu_Approx, Sigma_Approx_Sq)
    M4G = logmoment(4, Mu_Approx, Sigma_Approx_Sq)

    # Calculating Cumulants

    K1F = M1F
    K2F = M2F - pow(M1F, 2)
    K3F = M3F + 2 * pow(M1F, 3) - 3 * M2F * M1F
    K4F = M4F - 6 * pow(M1F, 4) - 4 * M3F * M1F + 12 * M2F * pow(M1F, 2) - 3 * pow(M2F, 2)

    K1G = M1G
    K2G = M2G - pow(M1G, 2)
    K3G = M3G + 2 * pow(M1G, 3) - 3 * M2G * M1G
    K4G = M4G - 6 * pow(M1G, 4) - 4 * M3G * M1G + 12 * M2G * pow(M1G, 2) - 3 * pow(M2G, 2)

    # Calculating d1 and d2

    d1 = (-1 * np.log(k) + Mu_Approx + Sigma_Approx_Sq) / np.sqrt(Sigma_Approx_Sq)
    d2 = d1 - np.sqrt(Sigma_Approx_Sq)

    # First term

    first_term = np.exp(-r * T) * (S0 / n) * (np.exp(Mu_Approx + 0.5 * Sigma_Approx_Sq) * norm.cdf(d1) - (k * norm.cdf(d2)))

    # Second term

    second_term = np.exp(-r * T) * (S0 / n) * (((K2F - K2G) / math.factorial(2)) * g(0, k, Mu_Approx, Sigma))

    # Third term

    third_term = np.exp(-r * T) * (S0 / n) * (((K3F - K3G) / math.factorial(3)) * g(1, k, Mu_Approx, Sigma))

    # Fourth term

    fourth_term = np.exp(-r * T) * (S0 / n) * (((K4F - K4G + 3 * pow(K2F - K2G, 2)) / math.factorial(4)) * g(2, k, Mu_Approx, Sigma))

    # Print Price and Standard Error

    print(first_term,
          first_term + second_term,
          first_term + second_term + third_term,
          first_term + second_term + third_term + fourth_term)