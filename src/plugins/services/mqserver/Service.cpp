/* 
 * File:   Service.cpp
 * Author: HoelscJ
 * 
 * Created on 7. Oktober 2011, 15:52
 */

#include "Service.h"
#include "org/esb/core/PluginContext.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/config/config.h"
namespace mqserver {

  Service::Service() {
  }

  void Service::startService() {
      _data_dir=org::esb::config::Config::get("hive.data_path");
      LOGDEBUG("Using mqserver datadir="<<_data_dir);
    go(Service::run, this);
    org::esb::lang::Thread::sleep2(2000);
  }
  org::esb::core::ServicePlugin::ServiceType Service::getServiceType() {
    return org::esb::core::ServicePlugin::SERVICE_TYPE_SERVER;
  }

  void Service::run() {
    _qm=new org::esb::mq::QueueManager(_data_dir);
    _qm->start();
  }

  void Service::stopService() {
    if(_qm)
      _qm->stop();
  }

  org::esb::core::OptionsDescription Service::getOptionsDescription() {
    _data_dir=org::esb::config::Config::get("hive.data_path");
    org::esb::core::OptionsDescription result("mqserver");
    
    result.add_options()
            ("mqserver.datadir", boost::program_options::value<std::string >()->default_value(_data_dir), "mqserver data path")
            ;
    return result;
  }

  Service::~Service() {
  }
}
