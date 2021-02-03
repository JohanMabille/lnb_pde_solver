#ifndef __PDE_H
#define __PDE_H

#include "option.h"

// Convection Diffusion Equation - Second-order PDE
class ConvectionDiffusionPDE {
 protected:
     ConvertibleDiffusionPDE() = default;
 public:
    // Entity semantics:
    virtual ~ConvectionDiffusionPDE() = default;

    ConvectionDiffusionPDE(const ConvectionDiffusionPDE&) = delete;
    ConvectionDiffusionPDE& operator=(const ConvectionDiffusionPDE&) = delete;
    ConvectionDiffusionPDE(ConvectionDiffusionPDE&&) = delete;
    ConvectionDiffusionPDE& operator=(ConvectionDiffusionPDE&&) = delete;

  // PDE Coefficients .
  // This is not specific to the PDE but to the option
  // should not be part of this API
  virtual double Sigma() const = 0;
  virtual double K() const = 0;
  virtual double r() const  = 0;


  virtual double alpha() const  = 0;
  virtual double beta() const  = 0;
  virtual double diff_coeff(double t, double x) const = 0;
  virtual double conv_coeff(double t, double x) const = 0;
  virtual double zero_coeff(double t, double x) const = 0;
  virtual double source_coeff(double t, double x) const = 0;

  // Boundary conditions should have a dedicated
  // hierarchy of classes (Dirichlet, Neumann)
  // Boundary and initial conditions
  virtual double boundary_left(double t, double x) const = 0;
  virtual double boundary_right(double t, double x) const = 0;
  virtual double init_cond(double x) const = 0;
};

// Black-Scholes PDE
class BlackScholesPDE : public ConvectionDiffusionPDE {
 public:
  VanillaOption* option;
  BlackScholesPDE(VanillaOption* _option);
  // Using override keyword would be better
  double Sigma() const ;
  double K() const ; 
  double r() const ;
  double alpha() const ;
  double beta() const ; 
  double diff_coeff(double t, double x) const;
  double conv_coeff(double t, double x) const;
  double zero_coeff(double t, double x) const;
  double source_coeff(double t, double x) const;

  double boundary_left(double t, double x) const;
  double boundary_right(double t, double x) const;
  double init_cond(double x) const;
};

#endif
