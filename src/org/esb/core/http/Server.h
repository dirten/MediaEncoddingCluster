/* 
 * File:   Server.h
 * Author: HoelscJ
 *
 * Created on 24. Januar 2012, 10:45
 */

#ifndef HTTP_SERVER_H
#define	HTTP_SERVER_H
#include "Poco/Net/ServerSocket.h"
#include "RequestHandlerFactory.h"
#include "Poco/Net/HTTPServer.h"
#include "org/esb/lang/Ptr.h"
#include "exports.h"
namespace org {
  namespace esb {
    namespace core {
      namespace http {

        class CORE_HTTP_EXPORT Server {
        public:
          Server(int port);
          virtual ~Server();
          void start();
          void stop();
          void setRequestHandlerFactory(RequestHandlerFactory *);
        private:
          Poco::Net::ServerSocket * _socket;
          Poco::Net::HTTPServer * _httpserver;
          Poco::Net::HTTPServerParams * _params;
        };
      }
    }
  }
}
#endif	/* SERVER_H */

