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

    Put_or_Call = 'Call'

    Price = 0.0

    # Check inputs
    if (S0 <= 0 or K < 0 or Sigma < 0 or r < 0 or T <= 0 or N <= 0):
        quit()

    # Moments of True / Lognormal Distribution

    Mu = np.log(S0) + ((r - 0.5 * pow(Sigma, 2)) * h / n) * 0.5 * (n * (n + 1))

    SigmaSq = pow(Sigma, 2) * h * ((n) * (n + 1) * (2 * n + 1) / (6 * pow(n, 2)))

    # Calculating d1 and d2

    d1 = (-1 * np.log(K) + Mu + SigmaSq) / np.sqrt(SigmaSq)
    d2 = d1 - np.sqrt(SigmaSq)

    # First term

    first_term = np.exp(-r * T) * (np.exp(Mu + 0.5 * SigmaSq) * norm.cdf(d1) - (K * norm.cdf(d2)))

    print(first_term)