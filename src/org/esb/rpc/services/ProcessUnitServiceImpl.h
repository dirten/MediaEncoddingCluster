/* 
 * File:   ProcessUnitServiceImpl.h
 * Author: HoelscJ
 *
 * Created on 14. Januar 2011, 11:13
 */

#ifndef PROCESSUNITSERVICEIMPL_H
#define	PROCESSUNITSERVICEIMPL_H
#include "org/esb/rpc/rpc.pb.h"
#include "../Server.h"
namespace org {
  namespace esb {
    namespace rpc {

      class ProcessUnitServiceImpl : public org::esb::rpc::ProcessUnitService {
      public:
        ProcessUnitServiceImpl(Server *aServer);
        virtual ~ProcessUnitServiceImpl();
        void getProcessUnit(google::protobuf::RpcController* controller,
                const org::esb::rpc::ProcessUnitRequest* request,
                org::esb::rpc::ProcessUnitResponse* response,
                google::protobuf::Closure* done);
        void putProcessUnit(google::protobuf::RpcController* controller,
                const org::esb::rpc::ProcessUnitRequest* request,
                org::esb::rpc::ProcessUnitResponse* response,
                google::protobuf::Closure* done);
      private:
        Server *mServer;

      };
    }
  }
}

#endif	/* PROCESSUNITSERVICEIMPL_H */

