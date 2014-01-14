/* 
 * File:   Rational.cpp
 * Author: jholscher
 * 
 * Created on 29. August 2010, 16:34
 */

#include "Rational.h"
#include <iostream>
#include <sstream>

namespace org {
  namespace esb {
    namespace av {

      Rational::Rational(double val) {
        AVRational tmp = av_d2q(val, INT_MAX);
        num = tmp.num;
        den = tmp.den;
      }

      Rational::Rational(int n, int d) {
        num = n;
        den = d;
      }

      Rational::Rational(AVRational r) {
        num = r.num;
        den = r.den;
      }

      bool Rational::operator==(const Rational & r) {
        return av_cmp_q(*this, r) == 0;
      }

      bool Rational::operator!=(const Rational &r) {
        return !(*this == r);
      }

      bool Rational::operator>(const Rational &r) {
        return av_cmp_q(*this, r) == 1;
      }

      bool Rational::operator<(const Rational &r) {
        return av_cmp_q(*this, r) == -1;
      }

      Rational::~Rational() {
      }
      
      std::string Rational::toString(){
        std::ostringstream oss;
        oss<<num;
        oss << "/";
        oss << den;
        return oss.str();
      }
    }
  }
}

