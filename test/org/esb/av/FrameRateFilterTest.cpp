/* 
 * File:   FrameRateFilterTest.cpp
 * Author: jholscher
 *
 * Created on 17. November 2010, 21:29
 */

#include <cstdlib>
#include "org/esb/av/FrameRateFilter.h"
using namespace std;
using namespace org::esb::av;
/*
 * 
 */
int main(int argc, char** argv) {
  Rational src(30,1);
  Rational trg(25,1);

  FrameRateFilter filter(src, trg);
  Frame in;
  Frame out;


  filter.process(in, out);

  
  return 0;
}

