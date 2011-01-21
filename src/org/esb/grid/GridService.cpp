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
#include "org/esb/rpc/Server.h"
#include "org/esb/rpc/services/ProcessServiceImpl.h"
#include "org/esb/rpc/services/NodeServiceImpl.h"
#include "org/esb/config/config.h"
#include "org/esb/io/File.h"
#include "org/esb/lang/CtrlCHitWaiter.h"

namespace org {
  namespace esb {
    namespace grid {

      GridService::GridService() {


      }

      GridService::~GridService() {
      }
      
      void GridService::start() {
      /*
        org::esb::net::TcpServerSocket *server = new org::esb::net::TcpServerSocket(2020);
        server->bind();
        while(true) {
          Ptr<org::esb::net::TcpSocket> clientSocket = server->accept();
          LOGDEBUG("new client");
          Ptr<ProtocolDispatcher> dispatcher=new ProtocolDispatcher(clientSocket);
          boost::thread(boost::bind(&ProtocolDispatcher::run, dispatcher));
        }*/
      }
    }
  }
}

int main(int argc, char**argv){
  org::esb::io::File f(argv[0]);
  std::string base_path = org::esb::io::File(f.getParent()).getParent();
  org::esb::config::Config::setProperty("hive.base_path", base_path);

  Log::open();
  org::esb::rpc::Server server(6000);
  server.registerService(new org::esb::rpc::ProcessServiceImpl());
  server.registerService(new org::esb::rpc::NodeServiceImpl());
  
  server.run();
  org::esb::lang::CtrlCHitWaiter::wait();



}