#ifndef BLACK_SCHOLES_PRICE_H
#define BLACK_SCHOLES_PRICE_H

class BlackScholesOptionPricer {
   public:
    // Calculate the standard normal cumulative distribution function
    static double normalCDF(double x);

    // Calculate d1 and d2 parameters for Black-Scholes formula
    static void calculateD1D2(const double& S, const double& K, const double& r, const double& q,
                              const double& sigma, const double& T, double& d1, double& d2);

    // Calculate call option price
    static double calculateCallPrice(const double& S, const double& K, const double& r,
                                     const double& q, const double& T, double& d1, double& d2);
    static double calculateCallDelta(const double& d1);

    // Calculate put option price
    static double calculatePutPrice(const double& S, const double& K, const double& r,
                                    const double& q, const double& T, const double& d1,
                                    const double& d2);
    static double calculatePutDelta(const double& d1);

    // Common greek calculations
    static double calculateGamma(const double& S, const double& sigma, const double& T,
                                 const double& d1);
    static double calculateVega(const double& S, const double& T, const double& d1);
};

#endif  // BLACK_SCHOLES_PRICER_H