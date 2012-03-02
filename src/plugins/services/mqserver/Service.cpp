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
    go(Service::run, this);
  }
  
  void Service::run() {
    _qm=new org::esb::mq::QueueManager(_data_dir);
    _qm->start();
  }

  void Service::stopService() {
    _qm->stop();
  }

  org::esb::core::OptionsDescription Service::getOptionsDescription() {
    _data_dir=org::esb::config::Config::get("hive.data_path");
    org::esb::core::OptionsDescription result("logserver");
    
    result.add_options()
            ("mqserver.datadir", boost::program_options::value<std::string >()->default_value(_data_dir), "logserver port listen on")
            ;
    return result;
  }

  Service::~Service() {
  }
}
