/* 
 * File:   HTTPServerTest.cpp
 * Author: HoelscJ
 *
 * Created on 24. Januar 2012, 13:10
 */

#include <cstdlib>
#include "org/esb/core/http/Server.h"
#include "org/esb/lang/CtrlCHitWaiter.h"

#include "org/esb/core/http/RequestHandlerFactory.h"
using namespace std;


class HandlerFactory:public org::esb::core::http::RequestHandlerFactory{
  
};
/*
 * 
 */
int main(int argc, char** argv) {
  org::esb::core::http::Server server(4000);
  
  //HandlerFactory factory;
  //server.setRequestHandlerFactory(factory);
  server.start();
  org::esb::lang::CtrlCHitWaiter::wait();
  server.stop();
  return 0;
}

