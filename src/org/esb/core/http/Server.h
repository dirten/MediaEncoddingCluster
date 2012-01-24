/* 
 * File:   Server.h
 * Author: HoelscJ
 *
 * Created on 24. Januar 2012, 10:45
 */

#ifndef SERVER_H
#define	SERVER_H
#include "Poco/Net/ServerSocket.h"
#include "RequestHandlerFactory.h"
#include "Poco/Net/HTTPServer.h"
#include "org/esb/lang/Ptr.h"
namespace org {
  namespace esb {
    namespace core {
      namespace http {

        class Server {
        public:
          Server(int port);
          virtual ~Server();
          void start();
          void stop();
          void setRequestHandlerFactory(RequestHandlerFactory &);
        private:
          Ptr<Poco::Net::ServerSocket> _socket;
          Ptr<Poco::Net::HTTPServer> _httpserver;
          Poco::Net::HTTPServerParams * _params;
        };
      }
    }
  }
}
#endif	/* SERVER_H */

