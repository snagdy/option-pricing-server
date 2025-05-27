#include "black_scholes_pricer.h"

#include <array>
#include <cmath>
#include <functional>
#include <ranges>

double BlackScholesOptionPricer::normalCDF(double x) {
    return 0.5 * (1 + erf(x / sqrt(2.0)));
}

// calculate d1 and d2 parameters for Black-Scholes formula
void BlackScholesOptionPricer::calculateD1D2(const double& S, const double& K, const double& r,
                                             const double& q, const double& sigma, const double& T,
                                             double& d1, double& d2) {
    d1 = (log(S / K) + (r - q + sigma * sigma / 2) * T) / (sigma * sqrt(T));
    d2 = d1 - sigma * sqrt(T);
}

double BlackScholesOptionPricer::calculateCallPrice(const double& S, const double& K,
                                                    const double& r, const double& q,
                                                    const double& T, const double& d1, 
                                                    const double& d2) {
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

// common greeks, do not change due to put-call parity
double BlackScholesOptionPricer::calculateGamma(const double& S, const double& sigma,
                                                const double& T, const double& d1) {
    return 1 / sqrt(2 * M_PI) * exp(-d1 * d1 / 2) * 1 / (S * sigma * sqrt(T));
}

double BlackScholesOptionPricer::calculateVega(const double& S, const double& T, const double& d1) {
    return S * normalCDF(d1) * sqrt(T);
}

// implied vol calculation via Newton's method for convergence
double BlackScholesOptionPricer::calculateImpliedVolatility(const double& P, 
                                                            const double& S, 
                                                            const double& K, 
                                                            const double& r, 
                                                            const double& q, 
                                                            const double& T,
                                                            const PricerOptionType& option_type,
                                                            const double& sigma_guess) {
    const int max_iterations = 1000;
    const double tolerance = 1e-8;
    double sigma = sigma_guess;
    double d1;
    double d2;
    std::function<double(const double& S, const double& K,
                         const double& r, const double& q,
                         const double& T, const double& d1, 
                         const double& d)>
                         price_function = option_type == PricerOptionType::PUT ? calculatePutPrice : calculateCallPrice;

    for (int i = 0; i < max_iterations; ++i) {
        calculateD1D2(S, K, r, q, sigma, T, d1, d2);
        double price_diff = price_function(S, K, r, q, T, d1, d2) - P;
        double sigma_new = sigma - price_diff / calculateVega(S, T, d1);

        if (abs(sigma_new - sigma) < tolerance) {
            break;
        }
        sigma = sigma_new;
    }

    return sigma;
};