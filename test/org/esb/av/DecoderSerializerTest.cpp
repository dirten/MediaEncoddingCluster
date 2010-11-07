/* 
 * File:   DecoderSerializerTest.cpp
 * Author: jholscher
 *
 * Created on 7. November 2010, 21:00
 */

#include <cstdlib>
#include "org/esb/util/Log.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open("");
  std::string filename=MEC_SOURCE_DIR;
  filename.append("/test.dvd");
  if(argc>1){
    filename=argv[1];
  }

  return 0;
}

