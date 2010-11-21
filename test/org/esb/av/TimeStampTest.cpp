/* 
 * File:   TimeStampTest.cpp
 * Author: jholscher
 *
 * Created on 29. August 2010, 16:52
 */

#include <cstdlib>
#include <iostream>
#include "org/esb/av/TimeStamp.h"
#include "org/esb/util/Log.h"
using namespace std;
using namespace org::esb::av;
/*
 * 
 */
int main(int argc, char** argv) {
  Log::open();
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

  {
    TimeStamp tlower(1,Rational(1,25));
    TimeStamp tupper(1,Rational(2,25));

    assert(tlower<tupper);
    assert(tupper>tlower);
  }

  {
    TimeStamp tt1(1,Rational(2,25));
    TimeStamp tt2(1,Rational(1,25));

    assert(tt2<tt1);
    assert(tt1>tt2);

    assert(!(tt2>tt1));
    assert(!(tt1<tt2));
  }
  {
    TimeStamp tt1(1,Rational(2,25));
    TimeStamp tt2(5,Rational(1,25));
    assert(fabs(tt1.toDouble() - 0.08) < 0.00001);
    assert(fabs(tt2.toDouble() - 0.2) < 0.00001);
  }



  return 0;
}

