/* 
 * File:   MHiveServerConnection.h
 * Author: HoelscJ
 *
 * Created on 4. April 2011, 16:54
 */

#ifndef MHIVESERVERCONNECTION_H
#define	MHIVESERVERCONNECTION_H
#include <string>
#include <list>
/*
#include "org/esb/rpc/Channel.h"
#include "org/esb/rpc/rpc.pb.h"
#include "org/esb/rpc/Controller.h"
 */
#include "org/esb/lang/Ptr.h"
#include "MediaFile.h"
#include "org/esb/rpc/rpc.pb.h"
#include "org/esb/rpc/Channel.h"
#include "org/esb/rpc/rpc.pb.h"
#include "org/esb/rpc/Controller.h"
#include "exports.h"
namespace org {
  namespace esb {
    namespace net{
      class TcpSocket;
    }
    namespace api {

      class API_EXPORT MHiveServerConnection {
      public:
        MHiveServerConnection(std::string host,std::string user, std::string passwd);
        virtual ~MHiveServerConnection();
        void connect();
        void disconnect();
        std::list<org::esb::rpc::Mediafile> getMediaFiles();
      private:
        std::string _name;
        std::string _passwd;
        std::string _host;
        Ptr<org::esb::rpc::Controller> _ctrl;
        Ptr<org::esb::rpc::ApiService::Stub> _api;
        Ptr<org::esb::rpc::Channel> ch;
        Ptr<org::esb::net::TcpSocket> _socket;
        std::string _token;
      };
    }
  }
}
#endif	/* MHIVESERVERCONNECTION_H */

