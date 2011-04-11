/* 
 * File:   ApiService.h
 * Author: HoelscJ
 *
 * Created on 4. April 2011, 15:05
 */

#ifndef APISERVICE_H
#define	APISERVICE_H
#include "org/esb/hive/DatabaseService.h"
#include "org/esb/rpc/rpc.pb.h"
#include "org/esb/util/Log.h"

namespace org {
  namespace esb {
    namespace rpc {
      class Server;

      class ApiServiceImpl : public org::esb::rpc::ApiService {
        classlogger("org.esb.rpc.ApiService");
      public:
        ApiServiceImpl(Server*);
        virtual ~ApiServiceImpl();
        void authenticate(::google::protobuf::RpcController* controller,
                const ::org::esb::rpc::User* request,
                ::org::esb::rpc::AuthToken* response,
                ::google::protobuf::Closure* done);

        void getMediafile(::google::protobuf::RpcController* controller,
                const ::org::esb::rpc::MediafileRequest* request,
                ::org::esb::rpc::MediafileResponse* response,
                ::google::protobuf::Closure* done);
        void addMediafile(::google::protobuf::RpcController* controller,
                const ::org::esb::rpc::MediafileRequest* request,
                ::org::esb::rpc::MediafileResponse* response,
                ::google::protobuf::Closure* done);
        void getMediafileList(::google::protobuf::RpcController* controller,
                const ::org::esb::rpc::MediafileRequest* request,
                ::org::esb::rpc::MediafileResponse* response,
                ::google::protobuf::Closure* done);

      private:
        db::HiveDb _db;
        std::string _authtoken;
      };
    }
  }
}
#endif	/* APISERVICE_H */

