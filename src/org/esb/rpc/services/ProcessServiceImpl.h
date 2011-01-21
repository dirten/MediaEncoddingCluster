/* 
 * File:   ProcessServiceImpl.h
 * Author: HoelscJ
 *
 * Created on 14. Januar 2011, 13:00
 */

#ifndef PROCESSSERVICEIMPL_H
#define	PROCESSSERVICEIMPL_H
#include "org/esb/rpc/rpc.pb.h"
#include "org/esb/lang/Process.h"
#include "org/esb/util/Log.h"

#include <map>
#include <list>
namespace org {
  namespace esb {
    namespace rpc {

      class ProcessServiceImpl: public org::esb::rpc::ProcessService {
        classlogger("org.esb.rpc.ProcessService");
      public:
        ProcessServiceImpl();
        virtual ~ProcessServiceImpl();
        void startProcess(google::protobuf::RpcController* controller,
                const org::esb::rpc::ProcessRequest* request,
                org::esb::rpc::ProcessResponse* response,
                google::protobuf::Closure* done);
        void stopProcess(google::protobuf::RpcController* controller,
                const org::esb::rpc::ProcessRequest* request,
                org::esb::rpc::ProcessResponse* response,
                google::protobuf::Closure* done);
      private:
        std::map<std::string, std::string> _exec_map;
        std::map<std::string, std::list<std::string> > _args_map;
        std::map<std::string, org::esb::lang::Process*> _process_map;
      };
    }
  }
}

#endif	/* PROCESSSERVICEIMPL_H */

