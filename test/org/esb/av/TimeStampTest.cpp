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
  assert(t.getTime()==10000);
  assert(base.num==1);
  assert(base.den==90000);

  TimeStamp t2=t.rescaleTo(Rational());
  assert(t2.getTime()==111111);

  assert(TimeStamp(100,Rational(1,90))==TimeStamp(100,Rational(1,90)));
  assert(TimeStamp(100,Rational(1,90))!=TimeStamp(101,Rational(1,90)));

  TimeStamp t1(1000l,Rational(122222222,900000l));
  TimeStamp t3=t1;
  assert(t1==t3);

  assert(t1!=t3.rescaleTo(Rational()));

  return 0;
}

