#include "black_scholes_pricer.h"

#include <cmath>

double BlackScholesOptionPricer::normalCDF(double x) {
    return 0.5 * (1 + erf(x / sqrt(2.0)));
}

// Calculate d1 and d2 parameters for Black-Scholes formula
void BlackScholesOptionPricer::calculateD1D2(const double& S, const double& K, const double& r,
                                             const double& q, const double& sigma, const double& T,
                                             double& d1, double& d2) {
    d1 = (log(S / K) + (r - q + sigma * sigma / 2) * T) / (sigma * sqrt(T));
    d2 = d1 - sigma * sqrt(T);
}

double BlackScholesOptionPricer::calculateCallPrice(const double& S, const double& K,
                                                    const double& r, const double& q,
                                                    const double& T, double& d1, double& d2) {
    return S * exp(-q * T) * normalCDF(d1) - K * exp(-r * T) * normalCDF(d2);
}

double BlackScholesOptionPricer::calculateCallDelta(const double& d1) {
    return normalCDF(d1);
}

double BlackScholesOptionPricer::calculatePutPrice(const double& S, const double& K,
                                                   const double& r, const double& q,
                                                   const double& T, const double& d1,
                                                   const double& d2) {
    return K * exp(-r * T) * normalCDF(-d2) - S * exp(-q * T) * normalCDF(-d1);
}

double BlackScholesOptionPricer::calculatePutDelta(const double& d1) {
    return normalCDF(d1) - 1;
}

// Common greeks, do not change due to put-call parity
double BlackScholesOptionPricer::calculateGamma(const double& S, const double& sigma,
                                                const double& T, const double& d1) {
    return 1 / sqrt(2 * M_PI) * exp(-d1 * d1 / 2) * 1 / (S * sigma * sqrt(T));
}

double BlackScholesOptionPricer::calculateVega(const double& S, const double& T, const double& d1) {
    return S * normalCDF(d1) * sqrt(T);
}
