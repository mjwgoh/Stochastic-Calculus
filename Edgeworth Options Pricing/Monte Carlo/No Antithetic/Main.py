# Import dependencies
import numpy as np
import csv

# Import Parameters

inputs = open('No Antithetic Input.csv')
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

    St = 0.0 # Initialize stock price at time t to 0
    S_sum = [0.0] * n # Initialize sum each pathway at time t to 0
    S_avg = 0 # Initialize the average of each pathway at time t to 0

    Put_or_Call = 'Call'
    Payoff = 0.0

    MC_Payoff = [0.0] * N # Initialize the list of the MC simulations at time t to 0
    MC_avg = 0.0  # Initialize the avg of the MC simulation at time t to 0
    Sx2_List = [0.0] * N # Initialize list of Sx2
    Sx2 = 0.0
    SE = 0.0

    Price = 0.0

    # Check inputs
    if (S0 <= 0 or K < 0 or Sigma < 0 or r < 0 or T <= 0 or N <= 0):
        quit()

    for j in range(N):

        St = S0
        S_avg = 0

        for i in range(n):

            U1 = np.random.uniform(low = 0.0, high = 1.0)
            U2 = np.random.uniform(low = 0.0, high = 1.0)

            Z1 = np.sqrt(-2 * np.log(U1)) * np.cos(2 * np.pi * U2)
            Z2 = np.sqrt(-2 * np.log(U1)) * np.sin(2 * np.pi * U2)

            Z1P = -Z1  # Antithetic Pairs
            Z2P = -Z2  # Antithetic Pairs
            St = St * np.exp((r - Sigma * Sigma * 0.5) * h + Sigma * Z1 * np.sqrt(h))

            S_sum[i] = St

        S_avg = sum(S_sum) / n

        if (Put_or_Call == 'Call'):
            Payoff = max(S_avg - K, 0)

        if (Put_or_Call == 'Put'):
            Payoff = max(K - S_avg, 0)

        MC_Payoff[j] = Payoff

    MC_avg = sum(MC_Payoff) / N

    for k in range(N):
        Sx2_List[k] = pow(MC_Payoff[k] - MC_avg, 2)

    Sx2 = sum(Sx2_List) / (N - 1)
    SE = np.sqrt(Sx2 / N)

    Price = np.exp(-r * T) * MC_avg

    # Print Price and Standard Error

    print(Price, SE)