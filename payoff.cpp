#ifndef __PAY_OFF_CPP
#define __PAY_OFF_CPP

#include "payoff.h"
#include <math.h>

// Constructor and destructor
PayOff::PayOff()
{
}
 PayOff::~PayOff()
{
}




// ==========
// PayOffCall
// ==========

// Constructor with single strike parameter
PayOffCall::PayOffCall(const double& _K)
{
     K = _K;
}
// Destructor
PayOffCall::~PayOffCall()
{
}

double PayOffCall::operator() (const double& S) const
{
  return std::max(exp(S)-K, 0.0); // Standard European call pay-off
}




// =========
// PayOffPut
// =========

// Constructor with single strike parameter
PayOffPut::PayOffPut(const double& _K)
{
  K = _K;
}
PayOffPut::~PayOffPut()
{
}

double PayOffPut::operator() (const double& S) const
{
  return std::max(K-exp(S), 0.0); // Standard European put pay-off
}




// =========
// PayOffForward
// =========

// Constructor with single strike parameter
PayOffForward::PayOffForward(const double& _K)
{
  K = _K;
}
PayOffForward::~PayOffForward()
{
}

double PayOffForward::operator() (const double& S) const
{
  return exp(S)-K; // Standard Forward contract
}



#endif
