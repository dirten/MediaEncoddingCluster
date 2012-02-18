/* 
 * File:   PocoRegExpTest.cpp
 * Author: jhoelscher
 *
 * Created on 18. Februar 2012, 13:34
 */

#include <cstdlib>
#include "Poco/RegularExpression.h"
#include <iostream>
#include "org/esb/util/Log.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open();
  if (argc < 3) {
    std::cout << "usage: " << argv[0] << " <regexp> <test-pattern>" << std::endl;
  }
  std::string pattern = argv[2];
  Poco::RegularExpression reholder(argv[1]);
  Poco::RegularExpression::MatchVec posVec;
  int a=0;
  if (int r = reholder.match(pattern, 0, posVec)) {
    LOGDEBUG("posVec:" << posVec.size());
    LOGDEBUG("r=" << r);
    for (int a = 1; a < posVec.size(); a++) {
      LOGDEBUG("Pattern group("<<a<<"):"<<pattern.substr(posVec[a].offset, posVec[a].length));
      
    }
  }

  return 0;
}

