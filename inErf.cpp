#include "inErf.h"

using namespace std;

double two_over_root_pi = 1.12837916709551257389615890312;

struct inErf_cache_node {
  double erf_value;
  double fp;
  double fpp;
  void set_values(double x) {
    erf_value = erf(x);
    fp = two_over_root_pi * exp( (-x) * x ); // (2*e^(-x^2)) / sqrt(pi)
    fpp = ( (-2) * x ) * fp; // (-4*x*e^(-x^2)) / sqrt(pi)
  }
};

class inErf_cache {
  public:
    inErf_cache_node whole[6];
    double half[5];
    inErf_cache() {
      double t;
      for (short i=0;i<6;i++) {
        t = i; // convert to double
        whole[i].set_values(t);
      }
      for (short i=0;i<5;i++) {
        t = i; // convert
        t += 0.5; // offset
        half[i] = erf(t);
      }
    }
};

inErf_cache my_inErf_cache;

short inErf_initial_guess(double x) {
  for (short i=0;i<5;i++) {
    if (x<=my_inErf_cache.half[i]) {
      return i;
    }
  }
  return 5;
}

double inErf(double x) {
  if (x<0.0) {
    return -inErf(-x); // it's an odd function, so we can do this
  }
  // x will be positive or zero past this point
  short guess_index = inErf_initial_guess(x);
  double guess = guess_index; // convert from short to float
  // we now have an initial guess
  // we can now use the cached data to skip the first iteration
  inErf_cache_node data;
  data = my_inErf_cache.whole[guess_index];
  data.erf_value = data.erf_value - x;
  guess = rootfinder::halley(guess,data.erf_value,data.fp,data.fpp);
  // we want to do three more iterations, but we have to compute stuff, we can't use cached data
  for (short i=0;i<3;i++) {
    data.set_values(guess);
    data.erf_value = data.erf_value - x;
    guess = rootfinder::halley(guess,data.erf_value,data.fp,data.fpp);
  }
  return guess;
}
