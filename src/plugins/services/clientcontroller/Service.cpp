/* 
 * File:   Service.cpp
 * Author: HoelscJ
 * 
 * Created on 10. Oktober 2011, 11:30
 */

#include "Service.h"
#include "org/esb/core/PluginContext.h"
#include "org/esb/lang/Process.h"
#include "org/esb/signal/Messenger.h"
#include "org/esb/util/Foreach.h"
namespace clientcontroller {

  Service::Service() {
  }

  Service::~Service() {
  }

  void Service::onMessage(org::esb::signal::Message &msg) {
    if (msg.containsProperty("server_up_event")) {
      LOGDEBUG("Server Up event received");
      string host = msg.getProperty<std::string > ("host");
      int port = msg.getProperty<int>("port");
      startClientNodes(host, port);

    } else if (msg.containsProperty("server_down_event")) {
      LOGDEBUG("Server Down event received");
      stopClientNodes();
    }
  }

  void Service::init() {

  }

  void Service::startService() {
    if (getContext()->getEnvironment<bool>("clientcontroller.autodiscovery")) {
      org::esb::signal::Messenger::getInstance().addMessageListener(*this);
    } else {
      string host = getContext()->getEnvironment<std::string > ("clientcontroller.host");
      int port = getContext()->getEnvironment<int>("clientcontroller.port");
      startClientNodes(host, port);
    }
  }

  void Service::stopService() {
    if (getContext()->getEnvironment<bool>("clientcontroller.autodiscovery")) {
      org::esb::signal::Messenger::getInstance().removeMessageListener(*this);
    } else {
      stopClientNodes();
    }
  }

  org::esb::core::OptionsDescription Service::getOptionsDescription() {
    org::esb::core::OptionsDescription result("clientcontroller");
    result.add_options()
            ("clientcontroller.autodiscovery", boost::program_options::value<bool >()->default_value(true), "use autodiscover to connect the client to the hiveserver")
            ("clientcontroller.port", boost::program_options::value<int >()->default_value(20200), "port the client connects to the hiveserver")
            ("clientcontroller.host", boost::program_options::value<std::string > ()->default_value("localhost"), "host the client connects to hiveserver")
            ("clientcontroller.count", boost::program_options::value<int > ()->default_value(org::esb::lang::Process::getCpuCount()), "how many clients to start on this node");
    return result;
  }

  void Service::startClientNodes(std::string host, int port) {
    int count = getContext()->getEnvironment<int>("clientcontroller.count");
    for (int a = 0; a < count; a++) {
      Ptr<org::esb::hive::HiveClient>c = new org::esb::hive::HiveClient(host, port);
      _client_list.push_back(c);
      boost::thread t(boost::bind(&org::esb::hive::HiveClient::start, c.get()));
    }
    //_client_audio=new org::esb::hive::HiveClientAudio(host, port);
    //boost::thread t(boost::bind(&org::esb::hive::HiveClientAudio::start, _client_audio.get()));
    
    //org::esb::signal::Messenger::getInstance().sendMessage(org::esb::signal::Message().setProperty("hiveclientaudio", org::esb::hive::START));
    

  }

  void Service::stopClientNodes() {
    boost::mutex::scoped_lock stopLock(stopMutex);

    LOGDEBUG("void Service::stopClientNodes()");
    foreach(Ptr<org::esb::hive::HiveClient> client, _client_list){
      LOGDEBUG("Client");
      client->stop();
      client.reset();
    }
    _client_list.clear();
    //_client_audio->stop();
    //_client_audio.reset();
  }

  org::esb::core::ServicePlugin::ServiceType Service::getServiceType() {
    return SERVICE_TYPE_CLIENT;
  }

}
