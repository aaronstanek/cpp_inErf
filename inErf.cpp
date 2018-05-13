#include "inErf.h"

#include <iostream>

using namespace std;

double inErf_initial_guess(double x) {
  if (x<=0.5204998778130466) { // erf(0.5)
    return 0.0;
  }
  if (x<=0.9661051464753108) { // erf(1.5)
    return 1.0;
  }
  if (x<=0.999593047982555) { // erf(2.5)
    return 2.0;
  }
  if (x<=0.9999992569016276) { // erf(3.5)
    return 3.0;
  }
  if (x<=0.9999999998033839) { // erf(4.5)
    return 4.0;
  }
  return 5.0;
}

double inErf(double x) {
  if (x<0.0) {
    return -inErf(-x); // it's an odd function, so we can do this
  }
  // x will be positive or zero past this point
  double guess = inErf_initial_guess(x);
  // we now have an initial guess
  // initialize some more variables
  double f;
  double fp;
  double fpp;
  for (short i=0;i<4;i++) {
    f = erf(guess) - x;
    fp = 1.12837916709551257389615890312 * exp( (-guess) * guess ); // (2*e^(-guess^2)) / sqrt(pi)
    fpp = ( (-2) * guess ) * fp; // (-4*x*e^(-guess^2)) / sqrt(pi)
    guess = rootfinder::halley(guess,f,fp,fpp);
  }
  return guess;
}
