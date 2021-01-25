#ifndef __FDM_H
#define __FDM_H

#include "pde.h"
#include <vector>
#include <iostream>
#include <string>

class FDMBase {
 protected:
  ConvectionDiffusionPDE* pde;

  // Space discretisation
  double x_dom_sup;
  double x_dom_inf;      // Space [x_dom_inf, x_dom_sup]
  unsigned long J;   // Number of space points
  double dx;         // Spatial step size
  std::vector<double> x_values;  // Coordinates of the x dimension

  // Time discretisation
  double t_dom;      // Time [0.0, t_dom]
  unsigned long N;   // Number of time points
  double dt;         // Temporal step size

  double prev_t, cur_t;   

  // Coefficients
  double alpha, beta, A, B, C, a1, a2, a3, b1, b2, b3;

  double theta;

  std::vector<double> new_result;
  std::vector<double> old_result;

  std::vector<double> c_prime;
  std::vector<double> d_prime;
  std::vector<double> d_vector;

  FDMBase(double _x_dom_sup, double _x_dom_inf, unsigned long _J,
          double _t_dom, unsigned long _N,
          double _theta,
          ConvectionDiffusionPDE* _pde);

  virtual void calculate_step_sizes() = 0;
  virtual void set_initial_conditions() = 0;
  virtual void calculate_boundary_conditions() = 0;
  virtual void calculate_inner_domain() = 0;

 public:
  virtual void step_march(const char* filename) = 0;
};

class FDMThetaScheme : public FDMBase {
 protected:
  void calculate_step_sizes();
  void set_initial_conditions();
  void calculate_boundary_conditions();
  void calculate_inner_domain();

 public:
  FDMThetaScheme(double _x_dom_sup, double _x_dom_inf,unsigned long _J,
                   double _t_dom, unsigned long _N,
                   double _theta,
                   ConvectionDiffusionPDE* _pde);

  void step_march(const char* filename);
  void bs_price(const char* filename) ;
};

#endif 
