/* 
 * File:   Service.cpp
 * Author: HoelscJ
 * 
 * Created on 5. Oktober 2011, 16:38
 */
#include "Service.h"
//#include "config.h"
#include "org/esb/config/config.h"
#include "NodeResolver.h"
#include "org/esb/signal/Message.h"
#include "org/esb/signal/Messenger.h"

class NodeListener : public org::esb::plugin::NodeListener {

  classlogger("NodeListener")
public:
  NodeListener() {
  }

  void onNodeUp(org::esb::plugin::Node & node) {
    if (node.getData("type") == "server") {
      LOGDEBUG("Server Node up")
      org::esb::signal::Messenger::getInstance().sendMessage(
              org::esb::signal::Message().setProperty("server_up_event", "").
              setProperty("host", node.getIpAddress().to_string()).
              setProperty("port", atoi(node.getData("port").c_str()))
              );
    }
  }

  void onNodeDown(org::esb::plugin::Node & node) {
    if (node.getData("type") == "server") {
      LOGDEBUG("Server Node down")
      org::esb::signal::Messenger::getInstance().sendMessage(
              org::esb::signal::Message().setProperty("server_down_event", "").
              setProperty("host", node.getIpAddress().to_string()).
              setProperty("port", atoi(node.getData("port").c_str()))
              );
    }
  }
private:
};
namespace udpnoderesolver {

  Service::Service() {
    _resolver = NULL;
    _agent = NULL;
  }

  Service::~Service() {
    LOGDEBUG("Service::~Service()");
    delete _resolver;
    _resolver = NULL;
    delete _agent;
    _agent = NULL;
  }

  void Service::startService() {
    org::esb::plugin::Node node;
    node.setData("type", org::esb::config::Config::getProperty("mode", "server"));
    node.setData("version", "0.0.5.0");
    node.setData("port", org::esb::config::Config::getProperty("hiveserver.port", "20200"));
    _resolver = new org::esb::plugin::NodeResolver(boost::asio::ip::address::from_string("0.0.0.0"), boost::asio::ip::address::from_string("239.255.0.1"), 6000, node);
    _agent = new NodeListener();
    _resolver->setNodeListener(_agent);
    _resolver->start();

  }

  void Service::stopService() {
    _resolver->stop();
  }

  org::esb::core::OptionsDescription Service::getOptionsDescription() {
    return org::esb::core::OptionsDescription();
  }
}
