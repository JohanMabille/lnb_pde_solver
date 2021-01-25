#ifndef __PDE_CPP
#define __PDE_CPP

#include "pde.h"
#include <math.h>

BlackScholesPDE::BlackScholesPDE(VanillaOption* _option) : option(_option) {}

// Diffusion coefficient
double BlackScholesPDE::alpha() const {
  double vol = option->sigma;
  return -0.5*vol*vol; 
}

double BlackScholesPDE::beta() const {
  double vol = option->sigma;
  return 0.5*vol*vol - option->r;
}

double BlackScholesPDE::r() const {
  return option->r; 
}

double BlackScholesPDE::K() const {
  return option->K;
}

double BlackScholesPDE::Sigma() const {
  return option->sigma; 
}


double BlackScholesPDE::diff_coeff(double t, double x) const {
  double vol = option->sigma;
  return 0.5*vol*vol*exp(x)*exp(x); 
}

// Convection coefficient
double BlackScholesPDE::conv_coeff(double t, double x) const {
  return (option->r)*exp(x); 
}

// Zero-term coefficient
double BlackScholesPDE::zero_coeff(double t, double x) const {
  return -(option->r); 
}

// Source coefficient
double BlackScholesPDE::source_coeff(double t, double x) const {
  return 0.0;
}

// Left boundary-condition (vanilla call option)
double BlackScholesPDE::boundary_left(double t, double x) const {
  return x*exp(-(option->r)*t); 
} 

// Right boundary-condition (vanilla call option)
double BlackScholesPDE::boundary_right(double t, double x) const {
  return x*exp(-(option->r)*t); 
}

// Initial condition (vanilla call option)
double BlackScholesPDE::init_cond(double x) const {
  return option->pay_off->operator()(x);
}

#endif
