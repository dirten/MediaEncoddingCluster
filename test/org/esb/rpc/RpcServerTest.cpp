/* 
 * File:   SimpleRpcTest.cpp
 * Author: HoelscJ
 *
 * Created on 11. Januar 2011, 17:06
 */

#include <cstdlib>
#include "org/esb/rpc/Server.h"
#include "org/esb/util/Log.h"

#include "org/esb/rpc/services/ProcessUnitServiceImpl.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open();
  org::esb::rpc::Server server(6000);
  server.registerService(new org::esb::rpc::ProcessUnitServiceImpl(&server));

  server.start();
  Log::close();
  return 0;
}

