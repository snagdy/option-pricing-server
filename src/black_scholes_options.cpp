#include "black_scholes_options.h"

BlackScholesCall::BlackScholesCall(const double& S, const double& K, const double& r,
                                   const double& q, const double& sigma, const double& T)
    : S{S}, K{K}, r{r}, q{q}, sigma{sigma}, T{T} {
    BlackScholesOptionPricer::calculateD1D2(S, K, r, q, sigma, T, d1, d2);
    price = BlackScholesOptionPricer::calculateCallPrice(S, K, r, q, T, d1, d2);
    delta = BlackScholesOptionPricer::calculateCallDelta(d1);
    gamma = BlackScholesOptionPricer::calculateGamma(S, sigma, T, d1);
    vega = BlackScholesOptionPricer::calculateVega(S, T, d1);
};

BlackScholesPut::BlackScholesPut(const double& S, const double& K, const double& r, const double& q,
                                 const double& sigma, const double& T)
    : S{S}, K{K}, r{r}, q{q}, sigma{sigma}, T{T} {
    BlackScholesOptionPricer::calculateD1D2(S, K, r, q, sigma, T, d1, d2);
    price = BlackScholesOptionPricer::calculatePutPrice(S, K, r, q, T, d1, d2);
    delta = BlackScholesOptionPricer::calculatePutDelta(d1);
    gamma = BlackScholesOptionPricer::calculateGamma(S, sigma, T, d1);
    vega = BlackScholesOptionPricer::calculateVega(S, T, d1);
};

BlackScholesCallImpliedVol::BlackScholesCallImpliedVol(const double& P, const double& S, const double& K, const double& r, const double& q, const double& T)
    : P{P}, S{S}, K{K}, r{r}, q{q}, T{T} {
    double sigma_guess = 0.02;
    implied_vol = BlackScholesOptionPricer::calculateImpliedVolatility(P, S, K, r, q, T, PricerOptionType::CALL, sigma_guess);
    delta = BlackScholesOptionPricer::calculateCallDelta(d1);
    gamma = BlackScholesOptionPricer::calculateGamma(S, implied_vol, T, d1);
    vega = BlackScholesOptionPricer::calculateVega(S, T, d1);
};

BlackScholesPutImpliedVol::BlackScholesPutImpliedVol(const double& P, const double& S, const double& K, const double& r, const double& q, const double& T)
    : P{P}, S{S}, K{K}, r{r}, q{q}, T{T} {
    double sigma_guess = 0.02;
    implied_vol = BlackScholesOptionPricer::calculateImpliedVolatility(P, S, K, r, q, T, PricerOptionType::PUT, sigma_guess);
    delta = BlackScholesOptionPricer::calculatePutDelta(d1);
    gamma = BlackScholesOptionPricer::calculateGamma(S, implied_vol, T, d1);
    vega = BlackScholesOptionPricer::calculateVega(S, T, d1);
};
