/* 
 * File:   Server.cpp
 * Author: HoelscJ
 * 
 * Created on 24. Januar 2012, 10:44
 */

#include "Server.h"
#include "HTTPException.h"

namespace org {
  namespace esb {
    namespace core {
      namespace http {

        Server::Server(int port) {
          _socket=new Poco::Net::ServerSocket(port);
          _params=new Poco::Net::HTTPServerParams();
          _params->setMaxQueued(100);
          _params->setMaxThreads(16);
        }
        
        void Server::setRequestHandlerFactory(RequestHandlerFactory *factory){
          _httpserver=new Poco::Net::HTTPServer(factory,*_socket, _params);
        }

        Server::~Server() {
          delete _httpserver;
          _httpserver=NULL;
          delete _socket;
          _socket=NULL;
        }
        
        void Server::start(){
          if(!_httpserver){
            throw HTTPException(__FILE__,__LINE__,"no httpserver");
          }
          _httpserver->start();
        }
        
        void Server::stop(){
          if(!_httpserver){
            throw HTTPException(__FILE__,__LINE__,"no httpserver");
          }
          _httpserver->stop();
        }
      }
    }
  }
}
