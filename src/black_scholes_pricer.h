#ifndef BLACK_SCHOLES_PRICE_H
#define BLACK_SCHOLES_PRICE_H

class BlackScholesOptionPricer {
private:
    // Calculate the standard normal cumulative distribution function
    double normalCDF(double x);
    
    // Calculate d1 and d2 parameters for Black-Scholes formula
    void calculateD1D2(double S, double K, double r, double sigma, double T,
                      double& d1, double& d2);

public:
    // Calculate call option price
    double calculateCall(double S, double K, double r, double sigma, double T);
    
    // Calculate put option price using put-call parity
    double calculatePut(double S, double K, double r, double sigma, double T);
};

#endif // BLACK_SCHOLES_PRICER_H