/* 
 * File:   CodecPropertyTransformerTest.cpp
 * Author: jholscher
 *
 * Created on 21. Oktober 2010, 11:56
 */

#include <cstdlib>
#include "org/esb/hive/CodecPropertyTransformer.h"
#include "org/esb/av/FormatBaseStream.h"
#include "org/esb/util/Log.h"
using namespace std;


/*
 * 
 */
int main(int argc, char** argv) {
  Log::open("");
  org::esb::av::FormatBaseStream::initialize();
  std::map<std::string, std::string> data;
  data["parti8x8"]="1";
  data["partb8x8"]="1";
  data["parti4x4"]="1";
  data["partp4x4"]="0";
  data["ac"]="2";
  data["g"]="250";
  data["bt"]="300000";
  data["b"]="1024000";
  data["ab"]="128000";
  data["chroma"]="1";
  data["mixed_refs"]="1";
  data["dct8x8"]="1";
  data["blafasel"]="test unknown";
  org::esb::hive::CodecPropertyTransformer * trans=new org::esb::hive::CodecPropertyTransformer(data);
//  trans.getCodecProperties();
  LOGDEBUG(trans->toString());
  return 0;
}

