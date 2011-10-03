/* 
 * File:   PluginLoaderTest.cpp
 * Author: HoelscJ
 *
 * Created on 30. August 2011, 13:15
 */

#include <cstdlib>
#include "org/esb/util/Log.h"
#include "org/esb/util/Foreach.h"

#include "org/esb/core/PluginLoader.h"
#include "org/esb/core/HookPlugin.h"
#include "org/esb/io/File.h"
#include "org/esb/av/FormatBaseStream.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/core/Request.h"
#include "org/esb/core/Response.h"
#include "org/esb/api/ServiceRequest.h"
#include "org/esb/api/ServiceResponse.h"

using namespace std;
using namespace org::esb::core;
using namespace org::esb::api;
/*
class TestReceiver : public org::esb::core::HookPlugin {
public:

  TestReceiver() {
  };

  ~TestReceiver() {
  };

  void hook(Request * req, Response*res) {
        //ServiceRequest* r=((ServiceRequest*) req);
        //ServiceRequest* sr=static_cast<ServiceRequest*>(req);
        //ServiceRequest* sr=dynamic_cast<ServiceRequest*>(req);
    //return;    
    std::cout << ((ServiceRequest*) req)->getRequestURI() << std::endl;
    std::cout << "Remote ip" << ((ServiceRequest*) req)->getRemoteIP() << std::endl;
    std::cout << "Remote port" << ((ServiceRequest*) req)->getRemotePort() << std::endl;
    std::string data;
    ((ServiceRequest*) req)->getInputstream()->read(data);
    ((ServiceResponse*) res)->setStatus(200);
    if (((ServiceRequest*) req)->getRequestURI() == "/")
      ((ServiceResponse*) res)->getOutputStream()->write("test data");
    else
      ((ServiceResponse*) res)->getOutputStream()->write("test nooooo");
    //std::cout << data << std::endl;
  };
} testInstance;
REGISTER_HOOK("web.api.Service", testInstance, TestReceiver::hook,1);
//REGISTER_HOOK("web.api.Service", testInstance, TestReceiver::hook,2);
//REGISTER_HOOK("web.api.Auth", testInstance, TestReceiver::hook,3);
 */

/*
 * 
 */

int main(int argc, char** argv) {
  Log::open();
  org::esb::io::File d("data");
  if (!d.exists())
    d.mkdir();
  //LOGDEBUG("test")
  //org::esb::core::PluginLoader loader1(WEBSERVER_PLUGIN);
  org::esb::core::PluginRegistry * i = org::esb::core::PluginRegistry::getInstance();
  i->load(std::string(MEC_SOURCE_DIR) + "/install/plugins");

  foreach(std::list<std::string>::value_type data, i->getPluginNameList()) {
    LOGDEBUG("Plugin List:" << data);
    OptionsDescription od = i->getOptionsDescription(data);
    typedef boost::shared_ptr<boost::program_options::option_description> option;
    foreach(const option value,od.options()){
      LOGDEBUG("Option:"<<value->description());
    }
  }
  //i->load(WEBSERVER_PLUGIN);
  //i->load(JSONSERVICE_PLUGIN);
  //org::esb::core::PluginLoader loader2(JSONSERVICE_PLUGIN);

  //i->startServices();
  //org::esb::lang::Thread::sleep2(60 * 1000);
  //i->stopServices();
  org::esb::core::PluginRegistry::close();
  Log::close();
  return 0;
}

