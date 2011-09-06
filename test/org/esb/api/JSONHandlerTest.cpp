/* 
 * File:   JSONHandlerTest.cpp
 * Author: HoelscJ
 *
 * Created on 1. September 2011, 15:57
 */

#include <cstdlib>
#include "org/esb/util/Log.h"
#include "org/esb/core/PluginLoader.h"
#include "org/esb/core/PluginRegistry.h"
#include "org/esb/lang/CtrlCHitWaiter.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open();

  //org::esb::core::PluginLoader *loader=new 
          org::esb::core::PluginLoader a1(WEBSERVER_PLUGIN);
  
  //org::esb::core::PluginLoader * bla=new 
          org::esb::core::PluginLoader a2(JSONSERVICE_PLUGIN);
  
  org::esb::lang::CtrlCHitWaiter::wait();
  org::esb::core::PluginRegistry::close();
  //Log::close();
  return 0;
}

