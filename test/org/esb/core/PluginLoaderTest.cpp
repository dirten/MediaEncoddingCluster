/* 
 * File:   PluginLoaderTest.cpp
 * Author: HoelscJ
 *
 * Created on 30. August 2011, 13:15
 */

#include <cstdlib>
#include "org/esb/util/Log.h"
#include "org/esb/core/PluginLoader.h"
#include "org/esb/core/HookPlugin.h"
#include "org/esb/io/File.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/core/Request.h"
#include "org/esb/core/Response.h"
#include "org/esb/api/ServiceRequest.h"
#include "org/esb/api/ServiceResponse.h"

using namespace std;
using namespace org::esb::core;
using namespace org::esb::api;

class TestReceiver:public org::esb::core::HookPlugin{
public:
  TestReceiver(){};
  ~TestReceiver(){};
  void hook(Request * req,Response*res){
  std::cout << static_cast<ServiceRequest*>(req)<<std::endl;
  std::cout << res<<std::endl;
  };
}testInstance;
REGISTER_HOOK("web.api.url",testInstance,TestReceiver::hook);

TestReceiver testInstance2;
REGISTER_HOOK("web.api.url",testInstance2,TestReceiver::hook);

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open();
  
  LOGDEBUG("test")
  org::esb::core::PluginLoader loader(WEBSERVER_PLUGIN);
  org::esb::lang::Thread::sleep2(40*1000);
  //org::esb::core::PluginLoader(JSONSERVICE_PLUGIN);
  return 0;
}

