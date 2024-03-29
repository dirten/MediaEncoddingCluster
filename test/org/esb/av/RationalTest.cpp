/*
 * File:   RationalTest.cpp
 * Author: jholscher
 *
 * Created on 29. August 2010, 17:02
 */

#include <assert.h>
#include <cstdlib>
#include <iostream>
#include "org/esb/av/Rational.h"
using namespace std;
using namespace org::esb::av;
/*
 * 
 */
int main(int argc, char** argv) {
  Rational r;
  assert(r.num==1);
  assert(r.den==1000000);

  Rational r2(1,90000);
  assert(r2.num==1);
  assert(r2.den==90000);

  assert(r!=r2);
  assert(r2>r);

  AVRational avr=r;
  assert(avr.num==1);
  assert(avr.den==1000000);

  assert(r==avr);

  AVRational avr2=r;
  assert(avr2.num==1);
  assert(avr2.den==1000000);


  av_rescale_q(10000,avr, avr2);
  av_rescale_q(10000,r, avr2);
  cout << av_rescale_q(1,Rational(1,44100), Rational())<<endl;
  return 0;
}

