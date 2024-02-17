# Experiments with Options Pricing and Hedging

This repository contains various options pricing projects I completed during my studies. The projects within this repository cover a wide range of topics within options pricing and hedging, utilizing different methodologies and programming languages to explore the intricacies of financial derivatives. Below is an overview of the projects included in this monorepo.

## Projects Overview

### 1. Replication, Hedging, and Rebalancing (C++)

This project focuses on the fundamental concepts of replicating portfolios, dynamic hedging, and rebalancing strategies in options markets. Implemented in C++, it provides a deep dive into the practical aspects of hedging using delta-neutral strategies and the impact of rebalancing frequencies on hedging effectiveness.

### 2. Path Dependent (Lookback) Options Pricing (C++)

A detailed exploration of path-dependent options, specifically lookback options, which have payoffs dependent on the maximum or minimum asset prices over the option's life. This C++ project employs various numerical methods to accurately price these complex financial instruments, highlighting their unique characteristics and pricing challenges.

### 3. European vs. American Options Pricing (C++)

This project compares and contrasts the pricing methodologies for European and American options, with a particular focus on the early exercise features of American options. Using C++, it demonstrates the differences in valuation techniques, including the Binomial and Black-Scholes models, and discusses the implications for traders and investors.

### 4. Boundary Options (C++)

An investigation into boundary options (also known as barrier options), which are a type of exotic option where the payoff depends on whether the underlying asset's price reaches a certain level. This C++ project covers various types of boundary options, such as knock-in and knock-out options, and their pricing methods.

### 5. Monte Carlo Options Pricing (C++)

This project utilizes the Monte Carlo simulation method to price options, a technique particularly useful for pricing complex derivatives with multiple sources of uncertainty. Implemented in C++, it showcases how Monte Carlo simulations can be used to model the stochastic paths of an asset's price and calculate the expected payoff of options.

### 6. Edgeworth Options Pricing (Python)

A novel approach to options pricing using Edgeworth series expansions, this Python project extends traditional pricing models by incorporating higher moments (skewness, kurtosis) into the distribution of asset returns. This method provides a more accurate pricing model for options in markets where asset returns deviate significantly from the normal distribution.
