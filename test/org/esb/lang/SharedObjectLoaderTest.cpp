/* 
 * File:   ObjectTest.cpp
 * Author: HoelscJ
 *
 * Created on 10. März 2011, 11:38
 */

#include <cstdlib>
#include "org/esb/lang/SharedObjectLoader.h"
#include "org/esb/util/Log.h"
using namespace std;
using namespace org::esb::lang;

/*
 * 
 */
int main(int argc, char** argv) {
  typedef int (*myfptr);
  typedef myfptr fptr(int);
  Log::open();
  SharedObjectLoader loader("/Users/jhoelscher/bripper/build/test/org/esb/lang/Debug/libshared_object");
  *(void **)
          (&fptr) = loader.getFunctionHandle("initPlugin");
  fptr(1);
  return 0;
}

