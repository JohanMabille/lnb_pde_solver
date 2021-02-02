// Name starting with _ and __ are reserved for the standard
#ifndef __PAY_OFF_HPP
#define __PAY_OFF_HPP

#include <algorithm>

class PayOff
{
 public:
    // This constructor could be protected
  PayOff(); // Default (no parameter) constructor
  virtual ~PayOff(); // Virtual destructor
  // Entity semantics:
  PayOff(const PayOff&) = delete;
  PayOff& operator=(const PayOff&) = delete;
  PayOff(PayOff&&) = delete;
  PayOff& operator=(PayOff&&) = delete;

  // The argument of the payoff should be the forward, not the spot
  // Overloaded () operator, turns the PayOff into an abstract function object
  virtual double operator() (const double& S) const = 0;
};


// ==========
// PayOffCall
// ==========

class PayOffCall : public PayOff
{
 private:
  double K; // Strike price

 public:
  PayOffCall(const double& K_);
  virtual ~PayOffCall();

  // Virtual function is now over-ridden (not pure-virtual anymore)
  virtual double operator() (const double& S) const;
};

// =========
// PayOffPut
// =========

class PayOffPut : public PayOff
{
 private:
  double K;

 public:
  PayOffPut(const double& K_);
  virtual ~PayOffPut();
  virtual double operator() (const double& S) const;
};


// =========
// PayOffForward
// =========

class PayOffForward : public PayOff
{
 private:
  double K;

 public:
  PayOffForward(const double& K_);
  virtual ~PayOffForward();
  virtual double operator() (const double& S) const;
};


#endif
