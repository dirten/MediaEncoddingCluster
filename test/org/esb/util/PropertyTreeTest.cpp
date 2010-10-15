/* 
 * File:   PropertyTreeTest.cpp
 * Author: HoelscJ
 *
 * Created on 13. Oktober 2010, 13:06
 */

#include <cstdlib>
#include "org/esb/util/PropertyTree.h"
#include "org/esb/util/Log.h"
using namespace std;
using namespace org::esb::util;
/*
 * 
 */
int main(int argc, char** argv) {
  Log::open("");
  std::string path=MEC_SOURCE_DIR;
  PropertyTree tree(path+"/test/test.props");
  std::string data=tree.get<std::string>("test.bla");
  LOGDEBUG("Data="<<data);
  return 0;
}

