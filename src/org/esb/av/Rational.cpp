/* 
 * File:   Rational.cpp
 * Author: jholscher
 * 
 * Created on 29. August 2010, 16:34
 */

#include "Rational.h"
namespace org {
  namespace esb {
    namespace av {

      Rational::Rational(int n, int d) {
        num = n;
        den = d;
      }

      Rational::Rational(AVRational r) {
        num = r.num;
        den = r.den;
      }

      bool Rational::operator==(const Rational & r) {
        return r.num==num&&r.den==den;
      }

      Rational::~Rational() {
      }
    }
  }
}

