/* 
 * File:   FrameRateFilterTest.cpp
 * Author: jholscher
 *
 * Created on 17. November 2010, 21:29
 */

#include <cstdlib>
#include "org/esb/av/FrameRateFilter.h"
#include "org/esb/util/Log.h"
using namespace std;
using namespace org::esb::av;
/*
 * 
 */
int main(int argc, char** argv) {
  Log::open();
  Rational trg(100,2397);
  Rational src(1,24);
//  Rational trg(1,24);
  std::map<std::string, std::string> param;
  //param["frameratecompensate"]="0.66666667";
  //param["frameratecompensate"]="-0.166667";

  FrameRateFilter filter(src, trg);
  filter.setParameter(param);
  Frame in;
  Frame out;
  int outcount=0;
  int count=0;
  for(int a=0;a<800;a++){
    
    filter.process(in, out);
    outcount+=out.getFrameCount();
    LOGDEBUG("FrameCount:"<<out.getFrameCount());
    count++;
  }
  LOGDEBUG("outframes="<<outcount);
  LOGDEBUG("inframes="<<count);
  return 0;
}

