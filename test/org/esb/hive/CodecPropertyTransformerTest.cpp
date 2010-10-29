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
  std::multimap<std::string, std::string> data;
  data.insert(std::pair<std::string,std::string>("parti8x8", "1"));
  data.insert(std::pair<std::string,std::string>("partb8x8", "1"));
  data.insert(std::pair<std::string,std::string>("parti4x4", "1"));
  data.insert(std::pair<std::string,std::string>("partp4x4", "0"));
  data.insert(std::pair<std::string,std::string>("partitions","partp16x16"));

  data.insert(std::pair<std::string,std::string>("ac", "2"));
  data.insert(std::pair<std::string,std::string>("g", "250"));
  data.insert(std::pair<std::string,std::string>("g", "500"));
  data.insert(std::pair<std::string,std::string>("bt","300000"));
  data.insert(std::pair<std::string,std::string>("b","1024000"));
  data.insert(std::pair<std::string,std::string>("ab","128000"));
  data.insert(std::pair<std::string,std::string>("cmp","chroma"));
  data.insert(std::pair<std::string,std::string>("chroma","1"));
  //data.insert(std::pair<std::string,std::string>("nsse","1"));
  data.insert(std::pair<std::string,std::string>("mixed_refs","1"));
  data.insert(std::pair<std::string,std::string>("dct8x8","0"));
  data.insert(std::pair<std::string,std::string>("blafasel","test unknown"));
  data.insert(std::pair<std::string,std::string>("frame_rate_num","1"));
  data.insert(std::pair<std::string,std::string>("frame_rate_den","25"));
  org::esb::hive::CodecPropertyTransformer * trans=new org::esb::hive::CodecPropertyTransformer(data);
//  trans.getCodecProperties();
  LOGDEBUG(trans->toString());
  return 0;
}

