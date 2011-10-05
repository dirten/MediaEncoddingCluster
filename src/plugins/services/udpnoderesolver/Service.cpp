/* 
 * File:   Service.cpp
 * Author: HoelscJ
 * 
 * Created on 5. Oktober 2011, 16:38
 */
#include "Service.h"
#include "config.h"
#include "org/esb/config/config.h"

Service::Service() {
  _resolver=NULL;
}

Service::~Service() {
  LOGDEBUG("Service::~Service()");
  delete _resolver;
  _resolver=NULL;
}

void Service::startService() {
  org::esb::plugin::Node node;
  node.setData("type", "server");
  node.setData("version", MHIVE_VERSION);
  node.setData("port", org::esb::config::Config::getProperty("client.port", "20200"));
  _resolver = new org::esb::plugin::NodeResolver(boost::asio::ip::address::from_string("0.0.0.0"), boost::asio::ip::address::from_string("239.255.0.1"), 6000, node);
  _resolver->start();

}

void Service::stopService() {
  _resolver->stop();
}

org::esb::core::OptionsDescription Service::getOptionsDescription() {
  return org::esb::core::OptionsDescription();
}

