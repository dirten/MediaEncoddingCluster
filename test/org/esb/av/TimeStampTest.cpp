/* 
 * File:   TimeStampTest.cpp
 * Author: jholscher
 *
 * Created on 29. August 2010, 16:52
 */

#include <cstdlib>
#include "org/esb/av/TimeStamp.h"
using namespace std;
using namespace org::esb::av;
/*
 * 
 */
int main(int argc, char** argv) {
  Rational r;
  assert(r.num=1);
  assert(r.den=1000000);

  TimeStamp t(10000,Rational(1,90000));
  Rational base=t.getTimeBase();
  assert(base.num>0);
  assert(base.den>0);

  TimeStamp t2=t.rescaleTo(Rational());
  assert(t2.getTime()==111111);
  assert(TimeStamp(100,Rational(1,90))==TimeStamp(100,Rational(1,90)));

  return 0;
}

