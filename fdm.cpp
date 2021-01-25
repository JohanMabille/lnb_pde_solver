#ifndef __FDM_CPP
#define __FDM_CPP

#include <fstream>
#include "fdm.h"
#include <iostream>
#include "math.h"
#include "norm.h"
using namespace std ;

FDMBase::FDMBase(double _x_dom_sup, double _x_dom_inf, unsigned long _J,
                 double _t_dom, unsigned long _N,
                 double _theta,
                 ConvectionDiffusionPDE* _pde) 
  : x_dom_sup(_x_dom_sup), x_dom_inf(_x_dom_inf), J(_J), t_dom(_t_dom), N(_N), theta(_theta),pde(_pde) {}

FDMThetaScheme::FDMThetaScheme(double _x_dom_sup, double _x_dom_inf, unsigned long _J,
                                   double _t_dom, unsigned long _N,
                                   double _theta,
                                   ConvectionDiffusionPDE* _pde) 
  : FDMBase(_x_dom_sup, _x_dom_inf,_J, _t_dom, _N, _theta,_pde) {
  calculate_step_sizes();
  set_initial_conditions();
}

void FDMThetaScheme::calculate_step_sizes() {
  dx = (x_dom_sup - x_dom_inf)/static_cast<double>(J-1);
  dt = t_dom/static_cast<double>(N-1);
}

void FDMThetaScheme::set_initial_conditions() {
  double cur_spot = x_dom_inf;

  old_result.resize(J, 0.0);
  new_result.resize(J, 0.0);
  x_values.resize(J, 0.0);

  for (unsigned long j=0; j<J; j++) {
    cur_spot += dx;
    old_result[j] = pde->init_cond(cur_spot);
    x_values[j] = cur_spot;
  }

  prev_t = 0.0;
  cur_t = 0.0;
}

void FDMThetaScheme::calculate_boundary_conditions() {
  new_result[0] = pde->boundary_left(dt, old_result[0]);
  new_result[J-1] = pde->boundary_right(dt, old_result[J-1]);
}

void FDMThetaScheme::calculate_inner_domain() {
  alpha = pde->alpha() / (dx * dx) ;
  beta = pde->beta() / (2 * dx) ; 
  A = dt * (alpha - beta) ; 
  B = dt * (pde->r() - 2 * alpha) ;
  C = dt * (alpha + beta) ; 

  a1 = - A *(1 - theta) ; 
  a2 = (1 - (1 - theta)* B) ;
  a3 = - C*(1- theta) ; 
  b1 = A*theta ; 
  b2 = (1+ theta * B);
  b3 = C * theta ; 

  c_prime.resize(J-1, 0.0);
  d_prime.resize(J, 0.0);
  d_vector.resize(J, 0.0) ; 
  
  c_prime[0] = 0 ; 
  d_vector[0] = new_result[0];
  d_vector[J-1] =  new_result[J-1];
  d_prime[0] = d_vector[0] ; 

  for (unsigned long j = 1 ; j < J-1; j++){
    c_prime[j] = b3 / (b2 - b1 * c_prime[j-1]) ;
    d_vector[j] = a1 * old_result[j-1] + a2 * old_result[j] + a3 * old_result[j+1] ; 
  }
  
  for(unsigned long j = 1 ; j < J; j++){
    d_prime[j] = (d_vector[j] - b1 * d_prime[j-1])/(b2 - b1 * c_prime[j-1]) ;
  }
  
  for (unsigned long j=J-2; j > 0; j--) {
    new_result[j] = d_prime[j] - c_prime[j] * d_vector[j+1] ; 
  }
}



void FDMThetaScheme::bs_price(const char* filename) { 
  std::ofstream bs_price(filename);
  prev_t = 0.0;
  cur_t = 0.0;
  for (int j=0; j<J; j++) {
      bs_price << x_values[j] << " " << cur_t << " " << call_price(exp(x_values[j]), pde->K(), pde->r(),  pde->Sigma(), 0) << std::endl;
    }

  while(cur_t < t_dom) { 
    cur_t = prev_t + dt;
    for (int j=0; j<J; j++) {
      bs_price << x_values[j] << " " << cur_t << " " << call_price( exp(x_values[j]), pde->K(), pde->r(),  pde->Sigma(), cur_t) << std::endl;
      if ( j > 0 && j < J-1) {
      }
    
    }
    prev_t = cur_t;
  }
  bs_price.close();  
}


void FDMThetaScheme::step_march(const char* filename) { 
  
  
  std::ofstream fdm_out(filename);
  std::ofstream delta_out("delta.csv");
  std::ofstream gamma_out("gamma.csv");
  std::ofstream theta_out("theta.csv");
  std::ofstream grid_size_values("grid_size.csv");

  grid_size_values << J << " " << N << std::endl; 
  grid_size_values.close() ; 

  for (int j=0; j<J; j++) {
      fdm_out << x_values[j] << " " << prev_t << " " << old_result[j] << std::endl;
    }
  while(cur_t < t_dom) { 
    cur_t = prev_t + dt;

    calculate_boundary_conditions();
    calculate_inner_domain();
    for (int j=0; j<J; j++) {
      fdm_out << x_values[j] << " " << cur_t << " " << new_result[j] << std::endl;
      if ( j > 0 && j < J-1) {
        delta_out << x_values[j] << " " << cur_t << " " << ((new_result[j] - new_result[j-1])/(dx)) * exp(-x_values[j]) << std::endl;
        gamma_out << x_values[j] << " " << cur_t << " " << ((new_result[j+1] + new_result[j-1] - 2 * new_result[j])/(dx * dx)) * exp(-2 * x_values[j]) - ((new_result[j] - new_result[j-1])/(dx)) * exp(-2 * x_values[j])  << std::endl;
      }
      theta_out << x_values[j] << " " << cur_t << " " << (old_result[j] - new_result[j]) / dt << std::endl;
      
    }
    
    old_result = new_result;
    prev_t = cur_t;
  }
  cout << "finished calculating" << endl;
  fdm_out.close();
  delta_out.close();
  gamma_out.close();
  theta_out.close();

}

#endif
