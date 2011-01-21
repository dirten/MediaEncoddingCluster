/* 
 * File:   Server.h
 * Author: HoelscJ
 *
 * Created on 12. Januar 2011, 18:49
 */

#ifndef SERVER_H
#define	SERVER_H
#include "google/protobuf/service.h"
#include <map>
#include "org/esb/net/TcpServerSocket.h"
#include "org/esb/net/TcpSocket.h"
#include "org/esb/util/Log.h"
namespace org {
  namespace esb {
    namespace rpc {

      class Server {
        classlogger("org.esb.rpc.Server")
      public:
        Server(int port);
        virtual ~Server();
        virtual void registerService(google::protobuf::Service *aService);
        google::protobuf::Service *ServiceRegisteredForName(const std::string &serviceName);
        void start();
        void run();
      private:
        org::esb::net::TcpServerSocket * _server;
        std::map<const std::string, google::protobuf::Service*> mServiceInstanceMap;
        void handleClient(org::esb::net::TcpSocket*);
        int _port;
      };
    }
  }
}

#endif	/* SERVER_H */

