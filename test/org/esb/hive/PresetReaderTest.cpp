/* 
 * File:   PresetReaderTest.cpp
 * Author: jholscher
 *
 * Created on 25. Oktober 2010, 23:21
 */

#include <cstdlib>
#include "org/esb/hive/PresetReader.h"
#include "org/esb/util/Log.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open("");
  std::string path=MEC_SOURCE_DIR;
  org::esb::hive::PresetReader reader(path+"/res/presets/test.preset");
  LOGDEBUG(reader.toString());
  
  return 0;
}

