/* 
 * File:   Service.cpp
 * Author: HoelscJ
 * 
 * Created on 7. Oktober 2011, 15:52
 */

#include "Service.h"
#include "org/esb/core/PluginContext.h"
#include "org/esb/lang/Thread.h"

namespace logserver {

  Service::Service() {
  }

  void Service::startService() {
    go(Service::run, this);
  }
  
  void Service::run() {
    int port = atoi(getContext()->env["logserver.port"].c_str());
    LOGDEBUG(" Start logserver on port " << port);
    server = new org::esb::net::TcpServerSocket(port);
    server->bind();
    for (; true;) {
      org::esb::net::TcpSocket * clientSocket = server->accept();
      go(Service::handleClient, this,clientSocket);
    }
  }

  void Service::stopService() {

  }

  org::esb::core::OptionsDescription Service::getOptionsDescription() {
    org::esb::core::OptionsDescription result("logserver");
    result.add_options()
            ("logserver.port", boost::program_options::value<int >()->default_value(8081), "logserver port listen on")
            ;
    return result;
  }

  void Service::handleClient(org::esb::net::TcpSocket *s) {
    unsigned char len[2];
    s->getInputStream()->read((unsigned char*)&len,sizeof(unsigned int));
    LOGDEBUG("data = "<<len);
  }

  Service::~Service() {
  }
}
