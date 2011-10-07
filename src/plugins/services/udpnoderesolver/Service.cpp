/* 
 * File:   Service.cpp
 * Author: HoelscJ
 * 
 * Created on 5. Oktober 2011, 16:38
 */
#include "Service.h"
#include "config.h"
#include "org/esb/config/config.h"
#include "NodeResolver.h"
class NodeListener : public org::esb::plugin::NodeListener {
  classlogger("NodeListener")
public:
  NodeListener() {
  }

  void onNodeUp(org::esb::plugin::Node & node) {
    LOGDEBUG("Node up")
  }

  void onNodeDown(org::esb::plugin::Node & node) {
    LOGDEBUG("Node down")
  }
private:
};

Service::Service() {
  _resolver=NULL;
  _agent=NULL;
}

Service::~Service() {
  LOGDEBUG("Service::~Service()");
  delete _resolver;
  _resolver=NULL;
  delete _agent;
  _agent=NULL;
}

void Service::startService() {
  org::esb::plugin::Node node;
  node.setData("type", "server");
  node.setData("version", MHIVE_VERSION);
  node.setData("port", org::esb::config::Config::getProperty("hiveserver.port", "20200"));
  _resolver = new org::esb::plugin::NodeResolver(boost::asio::ip::address::from_string("0.0.0.0"), boost::asio::ip::address::from_string("239.255.0.1"), 6000, node);
  _agent=new NodeListener();
  _resolver->setNodeListener(_agent);
  _resolver->start();

}

void Service::stopService() {
  _resolver->stop();
}

org::esb::core::OptionsDescription Service::getOptionsDescription() {
  return org::esb::core::OptionsDescription();
}

