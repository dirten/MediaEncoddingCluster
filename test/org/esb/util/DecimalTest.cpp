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
#include "org/esb/av/AV.h"
using namespace std;
using namespace org::esb::util;

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open();

  int base_a=av_rescale(2160*10, 44100, 90000);
  int base_b=11520;
  std::cout << "BaseA="<<base_a<<std::endl;
  std::cout << "BaseB="<<base_b<<std::endl;



  int gcd=av_gcd(base_a, base_b);
  std::cout << "GCD="<<gcd<<std::endl;



  int lcm=(base_a*base_b)/gcd;
  std::cout << "lcm="<<lcm<<std::endl;

  int packet_count=(lcm*10)/base_a;
  std::cout << "packet_count="<<packet_count<<std::endl;




  double d=1058.0;//100000./3.;
  //double d=1152.110000000001;//100000./3.;
  //d*=3;
  std::cout<<d<<std::endl;
  Decimal dec(d);

  Decimal::MantissaType i;
  Decimal::MantissaType f;
  Decimal::ExponentType exp = 0;
  dec.getIntegralFractionalExponent<Decimal::MantissaType>(i,f,exp,Decimal::ExponentType(dec.getExponent()));
  std::cout<<setprecision(19) <<i<<"-"<<dec.getMantissa()/pow((double)10, (dec.getExponent()+1)*-1)<<std::endl;
  std::cout << "Mantissa="<<setprecision(29)<< dec.getMantissa() << std::endl;
  std::cout << "Integer="<< dec.getInt64() << std::endl;
  std::cout << "Exponent="<< dec.getExponent() << std::endl;
  std::cout <<setprecision(29)<< "Fractional="<< f << std::endl;

  std::cout << "digits="<<dec.numberOfDigits<Decimal::MantissaType>(dec.getMantissa())<<std::endl;
  std::cout << "digits="<<pow((double)10, dec.getExponent()*-1)<<std::endl;

//  LOGDEBUG();
  return 0;
}

