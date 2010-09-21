/* 
 * File:   DatabaseBootstrapTest.cpp
 * Author: HoelscJ
 *
 * Created on 20. September 2010, 17:45
 */

#include <cstdlib>
#include "org/esb/db/DatabaseBootstrap.h"
#include "org/esb/util/Log.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open("");
  org::esb::db::DatabaseBootstrap::bootstrap();
  return 0;
}

