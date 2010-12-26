/* 
 * File:   GridService.cpp
 * Author: HoelscJ
 * 
 * Created on 22. Dezember 2010, 11:04
 */

#include "GridService.h"
#include "org/esb/net/TcpServerSocket.h"
#include "org/esb/net/TcpSocket.h"
#include "org/esb/lang/Ptr.h"
#include "protocol/ProtocolDispatcher.h"
#include "org/esb/util/Log.h"
namespace org {
  namespace esb {
    namespace grid {

      GridService::GridService() {


      }

      GridService::~GridService() {
      }
      
      void GridService::start() {
        org::esb::net::TcpServerSocket *server = new org::esb::net::TcpServerSocket(2020);
        server->bind();
        while(true) {
          LOGDEBUG("new client");
          Ptr<org::esb::net::TcpSocket> clientSocket = server->accept();
          Ptr<ProtocolDispatcher> dispatcher=new ProtocolDispatcher(clientSocket);
          boost::thread(boost::bind(&ProtocolDispatcher::run, dispatcher));
        }
      }
    }
  }
}

int main(int argc, char**argv){
  Log::open();
  org::esb::grid::GridService service;
  service.start();
}