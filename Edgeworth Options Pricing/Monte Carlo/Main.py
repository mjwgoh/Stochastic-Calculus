# Import dependencies
import numpy as np
import csv

# Import Parameters

inputs = open('Perfect Price Input.csv')
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

    St = 0.0 # Initialize stock price at time t
    StP = 0.0 # Initialize stock price for antithetic pair at time t
    S_sum = [0.0] * n # Initialize sum each pathway at time t to 0
    S_sumP = [0.0] * n # Initialize sum each pathway at time t to 0 for the antithetic pair
    S_avg = 0 # Initialize the average of each pathway at time t to 0
    S_avgP = 0 # Initialize the average of each pathway at time t to 0 for the antithetic pair

    Put_or_Call = 'Call'
    Payoff = 0.0
    PayoffP = 0.0

    MC_Payoff = [0.0] * int(N / 2) # Initialize the list of the MC simulations at time t to 0
    MC_avg = 0.0  # Initialize the avg of the MC simulation at time t to 0
    Sx2_List = [0.0] * int(N / 2) # Initialize list of Sx2
    Sx2 = 0.0
    SE = 0.0

    Price = 0.0

    # Check inputs
    if (S0 <= 0 or K < 0 or Sigma < 0 or r < 0 or T <= 0 or N <= 0):
        quit()

    for j in range(int(N / 2)):

        St = S0
        StP = S0
        S_avg = 0

        for i in range(n):

            U1 = np.random.uniform(low = 0.0, high = 1.0)
            U2 = np.random.uniform(low = 0.0, high = 1.0)

            Z1 = np.sqrt(-2 * np.log(U1)) * np.cos(2 * np.pi * U2)
            Z2 = np.sqrt(-2 * np.log(U1)) * np.sin(2 * np.pi * U2)

            Z1P = -Z1  # Antithetic Pairs
            Z2P = -Z2  # Antithetic Pairs
            St = St * np.exp((r - Sigma * Sigma * 0.5) * h + Sigma * Z1 * np.sqrt(h))
            StP = StP * np.exp((r - Sigma * Sigma * 0.5) * h + Sigma * Z1P * np.sqrt(h))

            S_sum[i] = St
            S_sumP[i] = StP

        S_avg = sum(S_sum) / n
        S_avgP = sum(S_sumP) / n

        if (Put_or_Call == 'Call'):
            Payoff = max(S_avg - K, 0)
            PayoffP = max(S_avgP - K, 0)

        if (Put_or_Call == 'Put'):
            Payoff = max(K - S_avg, 0)
            PayoffP = max(K - S_avgP, 0)

        MC_Payoff[j] = (Payoff + PayoffP) / 2

    MC_avg = sum(MC_Payoff) / (N / 2)

    for k in range(int(N / 2)):
        Sx2_List[k] = pow(MC_Payoff[k] - MC_avg, 2)

    Sx2 = sum(Sx2_List) / ((N / 2) - 1)
    SE = np.sqrt(Sx2 / (N / 2))

    Price = np.exp(-r * T) * MC_avg

    # Print Price and Standard Error

    print(Simulation_Number, Price, SE)