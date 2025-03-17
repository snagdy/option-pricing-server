#include "black_scholes_pricer.h"
#include <cmath>


// Calculate the standard normal cumulative distribution function
double BlackScholesOptionPricer::normalCDF(double x) {
    return 0.5 * (1 + erf(x / sqrt(2.0)));
}

// Calculate d1 and d2 parameters for Black-Scholes formula
void BlackScholesOptionPricer::calculateD1D2(double S, double K, double r, double sigma, double T,
                    double& d1, double& d2) {
    d1 = (log(S/K) + (r + sigma*sigma/2)*T) / (sigma*sqrt(T));
    d2 = d1 - sigma*sqrt(T);
}

// Calculate call option price
double BlackScholesOptionPricer::calculateCall(double S, double K, double r, double sigma, double T) {
    double d1, d2;
    calculateD1D2(S, K, r, sigma, T, d1, d2);
    
    return S * normalCDF(d1) - K * exp(-r*T) * normalCDF(d2);
}

// Calculate put option price using put-call parity
double BlackScholesOptionPricer::calculatePut(double S, double K, double r, double sigma, double T) {
    double callPrice = calculateCall(S, K, r, sigma, T);
    return callPrice - S + K * exp(-r*T);
}