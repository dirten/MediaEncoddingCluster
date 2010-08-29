/* 
 * File:   MediaLocatorTest.cpp
 * Author: jholscher
 *
 * Created on 29. August 2010, 15:13
 */

#include <cstdlib>
#include "org/esb/av/MediaLocator.h"
#include "org/esb/io/File.h"
using namespace std;
using namespace org::esb::av;
using namespace org::esb::io;

/*
 * 
 */
int main(int argc, char** argv) {


  std::string basepath = MEC_SOURCE_DIR;
  std::string path="file://";
  MediaLocator locator(path+basepath+"/test.dvd");
  assert(locator.getProtocol()=="file");
  cout<< locator.getRemainder()<<endl;
  return 0;
}

