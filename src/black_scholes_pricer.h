#ifndef BLACK_SCHOLES_PRICE_H
#define BLACK_SCHOLES_PRICE_H

enum class PricerOptionType {
    PUT,
    CALL
};

class BlackScholesOptionPricer {
   public:
    // calculate the standard normal cumulative distribution function
    static double normalCDF(double x);

    // calculate d1 and d2 parameters for Black-Scholes formula
    static void calculateD1D2(const double& S, const double& K, const double& r, const double& q,
                              const double& sigma, const double& T, double& d1, double& d2);

    // calculate call option price
    static double calculateCallPrice(const double& S, const double& K, const double& r,
                                     const double& q, const double& T, const double& d1, 
                                     const double& d2);
    static double calculateCallDelta(const double& d1);

    // calculate put option price
    static double calculatePutPrice(const double& S, const double& K, const double& r,
                                    const double& q, const double& T, const double& d1,
                                    const double& d2);
    static double calculatePutDelta(const double& d1);

    // common greek calculations
    static double calculateGamma(const double& S, const double& sigma, const double& T,
                                 const double& d1);
    static double calculateVega(const double& S, const double& T, const double& d1);

    // calculate option implied vol
    static double calculateImpliedVolatility(const double& P, 
                                             const double& S, 
                                             const double& K, 
                                             const double& r, 
                                             const double& q, 
                                             const double& T,
                                             const PricerOptionType& option_type,
                                             const double& sigma_guess);
};

#endif  // BLACK_SCHOLES_PRICER_H