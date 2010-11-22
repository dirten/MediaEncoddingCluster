/* 
 * File:   DecimalTest.cpp
 * Author: HoelscJ
 *
 * Created on 22. November 2010, 12:48
 */

#include <cstdlib>
#include <math.h>
#include "org/esb/util/Decimal.h"
#include "org/esb/util/Log.h"
using namespace std;
using namespace org::esb::util;

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open();

  double d=100000./3.;
  d*=3;
  Decimal dec(d);
  Decimal::MantissaType i;
  Decimal::MantissaType f;
  Decimal::ExponentType exp = 0;
  dec.getIntegralFractionalExponent<Decimal::MantissaType>(i,f,exp,Decimal::ExponentType(dec.getExponent()));
  std::cout<<setprecision(19) <<i<<"-"<<f/pow(10, dec.getExponent()*-1)<<std::endl;
  std::cout << "digits="<<dec.numberOfDigits<Decimal::MantissaType>(dec.getMantissa())<<std::endl;
  std::cout << "digits="<<pow((double)10, dec.getExponent()*-1)<<std::endl;
//  LOGDEBUG();
  return 0;
}

