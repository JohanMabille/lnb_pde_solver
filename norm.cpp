#ifndef __NORM_CPP
#define __NORM_CPP

#define _USE_MATH_DEFINES
#include <cmath>

// This implementation is redundant with that in closed_form.hpp / closed_form.cpp
// Besides it does not provides vectorized version of the functions
double norm_pdf(const double& x) {
    // Relying on std::erfc would be more accurate
    return (1.0/(pow(2*M_PI,0.5)))*exp(-0.5*x*x);
}


double norm_cdf(const double& x) {
    double k = 1.0/(1.0 + 0.2316419*x);
    double k_sum = k*(0.319381530 + k*(-0.356563782 + k*(1.781477937 + k*(-1.821255978 + 1.330274429*k))));
    if (x >= 0.0) {
        return (1.0 - (1.0/(pow(2*M_PI,0.5)))*exp(-0.5*x*x) * k_sum);

    } else {
        return 1.0 - norm_cdf(-x);
    }
}

double d_j(const int& j, const double& S, const double& K, const double& r, const double& v, const double& T) {
    return (log(S/K) + (r + (pow(-1,j-1))*0.5*v*v)*T)/(v*(pow(T,0.5)));
}

double call_price(const double& S, const double& K, const double& r, const double& v, const double& T) {
    // This implementation is not stable on the wings and can generate oscillations
    // A more robust implementation is done in closed_form.cpp, where the formula is plit
    // into the payoff and the time value.
    // Also the formula in closed_form.cpp takes the forward instead of the spot, avoiding the need
    // to pass the rate (but you need to discount the price if the rate is not null).
    return S * norm_cdf(d_j(1, S, K, r, v, T))-K*exp(-r*T) * norm_cdf(d_j(2, S, K, r, v, T));

}

#endif
