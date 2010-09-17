/* 
 * File:   ProcessTestExecutable.cpp
 * Author: HoelscJ
 *
 * Created on 17. September 2010, 14:46
 */

#include <cstdlib>
#include "org/esb/lang/Thread.h"
#include <stdio.h>
#include "org/esb/util/Log.h"
using namespace std;

int die() {
  char * t;
  sprintf(t,"%s", "bla");
  int local_var = 1;
  *reinterpret_cast<char*>(NULL) = 1;
  return 0;
}

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open("");
  LOGDEBUG("test arguments");

  for(int a=0;a<argc;a++){
    LOGDEBUG("Parameter #"<<a<<" "<<argv[a]);
  }
  while(true){
    LOGDEBUG("cycle");org::esb::lang::Thread::sleep2(500);
  }
  LOGDEBUG("and now die myself");
//  die();

  org::esb::lang::Thread::sleep2(10000);
  LOGDEBUG("returning child");
  return 0;
}

