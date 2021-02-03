#include "payoff.h"
#include "option.h"
#include "pde.h"
#include "fdm.h"
#include <math.h>
#include <iostream>

// Guidelines:
//
// 1] Equation
// Even if the volatility and the rate are constant in the BS model,
// we can prove that, under certain assumptions, we can use the same
// PDE equation with volatility surface and rate curves. We could also
// want to take into account the repo (even if it could theoretically
// be part of the r factor). Therefore, it is more generic to solve
// the following generic equation;
// df/dt = a(x,t)d2f/dx2 + b(x,t)df/dx + c(x, t)f + d(x, t).
// The pricer should ask the coefficients a, b, c and d to an
// abstract class that will be inherited by classes implementing
// different models.
// 
// 2] Payoff
// The pricer should be able to price exotic options, such as
// barriers or asian options. To do so, the pricer must be able
// to call an external function between each step. Define an API
// that allows to register a function or an abstract class modeling
// a payoff.



int main(int argc, char **argv) {
  // Create the option parameters
  double K = 100;  // Strike price
  double r = 0.00;   // Risk-free rate (5%)
  double v = 0.2;    // Volatility of the underlying (20%)
  double T = 1.00;    // One year until expiry
  double S0 = 100;    // Spot value

  // FDM discretisation parameters
  double x_dom_sup = log(S0) + 5 * v  * sqrt(T);       // Spot goes between [log(s0) -+ 5 * v * sqrt(T)]
  double x_dom_inf = log(S0) - 5 * v  * sqrt(T);
  unsigned long J = 40; // That's very small, typical values are rather 500 or even 1000
  double t_dom = T;         // Time period as for the option
  unsigned long N = 20; 
  double theta = 0.5 ;    

  // Create the PayOff and Option objects
  PayOff* pay_off_call = new PayOffCall(K);
  VanillaOption* call_option = new VanillaOption(K, r, T, v, pay_off_call);

  // // Create the PayOff and Option objects
  // PayOff* pay_off_forward = new PayOffForward(K);
  // VanillaOption* forward = new VanillaOption(K, r, T, v, pay_off_forward);

  // Create the PDE and FDM objects
  BlackScholesPDE* bs_pde = new BlackScholesPDE(call_option);
  FDMThetaScheme fdm_theta_scheme(x_dom_sup, x_dom_inf, J, t_dom, N, theta,bs_pde);

  // Run the FDM solver
  fdm_theta_scheme.step_march("price.csv");
  fdm_theta_scheme.bs_price("bs_price.csv") ; 

  // Vega Calculation 
  // Create the Option Object with shocked volatility
  VanillaOption* call_option_shoched_vol = new VanillaOption(K, r, T, v + 0.01, pay_off_call);


  // Create the PDE and FDM objects with shocked volatility
  BlackScholesPDE* bs_pde_shoched_vol = new BlackScholesPDE(call_option_shoched_vol);

  FDMThetaScheme fdm_euler_shoched_vol(x_dom_sup, x_dom_inf, J, t_dom, N, theta,bs_pde_shoched_vol);

  // Run the FDM solver with schocked volatility
  fdm_euler_shoched_vol.step_march("price_shock_vol.csv");

  // Delete the PDE, PayOff and Option objects
  delete bs_pde;
  delete call_option;
  delete pay_off_call;


  // delete forward;
  // delete pay_off_forward;

  return 0;
}
