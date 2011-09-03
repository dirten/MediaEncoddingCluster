/* 
 * File:   LogTest.cpp
 * Author: jhoelscher
 *
 * Created on 3. September 2011, 13:40
 */

#include <cstdlib>
#include "org/esb/util/Log.h"
using namespace std;

class logtest{
public:
  logtest(){
    LOGDEBUG("aaahhh");
  }
};
/*
 * 
 */
int main(int argc, char** argv) {
  Log::open();
  LOGDEBUG("test log debug");
  logtest t;
  return 0;
}

