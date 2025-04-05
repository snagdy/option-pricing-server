#ifndef BLACK_SCHOLES_OPTIONS_H
#define BLACK_SCHOLES_OPTIONS_H

#include "black_scholes_pricer.h"

class BlackScholesCall {
   private:
    const double S;
    const double K;
    const double r;
    const double q;
    const double sigma;
    const double T;
    double d1;
    double d2;

   public:
    BlackScholesCall(const double& S, const double& K, const double& r, const double& q,
                     const double& sigma, const double& T);
    double price;
    double delta;
    double gamma;
    double vega;
};

class BlackScholesPut {
   private:
    const double S;
    const double K;
    const double r;
    const double q;
    const double sigma;
    const double T;
    double d1;
    double d2;

   public:
    BlackScholesPut(const double& S, const double& K, const double& r, const double& q,
                    const double& sigma, const double& T);
    double price;
    double delta;
    double gamma;
    double vega;
};

#endif  // BLACK_SCHOLES_OPTIONS_H