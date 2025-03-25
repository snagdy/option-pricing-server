#include "black_scholes_pricer.h"
#include <cmath>

// TODO(snagdy): modify for continuous dividend paying options, consider making an option class to prevent redundant calcs
// Calculate the standard normal cumulative distribution function
double BlackScholesOptionPricer::normalCDF(double x) {
    return 0.5 * (1 + erf(x / sqrt(2.0)));
}

// Calculate d1 and d2 parameters for Black-Scholes formula
void BlackScholesOptionPricer::calculateD1D2(double S, double K, double r, double q, double sigma, double T,
                    double& d1, double& d2) {
    d1 = (log(S/K) + (r - q + sigma * sigma/2)*T) / (sigma * sqrt(T));
    d2 = d1 - sigma * sqrt(T);
}

double BlackScholesOptionPricer::calculateCallPrice(double S, double K, double r, double q, double sigma, double T) {
    double d1, d2;
    calculateD1D2(S, K, r, q, sigma, T, d1, d2);
    
    return S * exp(-q * T) * normalCDF(d1) - K * exp(-r * T) * normalCDF(d2);
}

double BlackScholesOptionPricer::calculateCallDelta(double S, double K, double r, double q, double sigma, double T) {
    double d1, d2;
    calculateD1D2(S, K, r, q, sigma, T, d1, d2);
    return normalCDF(d1);
}

double BlackScholesOptionPricer::calculatePutPrice(double S, double K, double r, double q, double sigma, double T) {
    double d1, d2;
    calculateD1D2(S, K, r, q, sigma, T, d1, d2);
    return K * exp(-r * T) * normalCDF(-d2) - S * exp(-q * T) * normalCDF(-d1);
}

double BlackScholesOptionPricer::calculatePutDelta(double S, double K, double r, double q, double sigma, double T) {
    double d1, d2;
    calculateD1D2(S, K, r, q, sigma, T, d1, d2);
    return normalCDF(d1) - 1;
}

// Common greeks, do not change due to put-call parity
double BlackScholesOptionPricer::calculateGamma(double S, double K, double r, double q, double sigma, double T) {
    double d1, d2;
    calculateD1D2(S, K, r, q, sigma, T, d1, d2);
    return 1 / sqrt(2 *  M_PI) * exp(-d1 * d1 / 2) * 1 / (S * sigma * sqrt(T));
}

double BlackScholesOptionPricer::calculateVega(double S, double K, double r, double q, double sigma, double T) {
    double d1, d2;
    calculateD1D2(S, K, r, q, sigma, T, d1, d2);

    return S * normalCDF(d1) * sqrt(T);
}

