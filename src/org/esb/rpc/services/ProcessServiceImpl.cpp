/* 
 * File:   ProcessServiceImpl.cpp
 * Author: HoelscJ
 * 
 * Created on 14. Januar 2011, 13:00
 */

#include "ProcessServiceImpl.h"
#include "org/esb/config/config.h"
#include "org/esb/lang/ProcessException.h"
namespace org {
  namespace esb {
    namespace rpc {

      ProcessServiceImpl::ProcessServiceImpl() : ProcessService() {
        std::string path = org::esb::config::Config::get("hive.base_path");

        std::list<std::string> args;

        args.push_back("-r");
        _process_map["core"] = new org::esb::lang::Process(path + "/bin/mhive", args);

        args.clear();
        args.push_back("-i");
        _process_map["client"] = new org::esb::lang::Process(path + "/bin/mhive", args);

        args.clear();
        args.push_back("-q");
        _process_map["queue"] = new org::esb::lang::Process(path + "/bin/mhive", args);
      }

      ProcessServiceImpl::~ProcessServiceImpl() {
        LOGDEBUG("shutting down service");
        std::map<std::string, org::esb::lang::Process*>::iterator it = _process_map.begin();

        for (; it != _process_map.end(); it++) {
          delete (*it).second;
        }
      }

      void ProcessServiceImpl::startProcess(google::protobuf::RpcController* controller,
              const org::esb::rpc::ProcessRequest* request,
              org::esb::rpc::ProcessResponse* response,
              google::protobuf::Closure* done) {
        return;
        if (!request->has_process_data()) {
          LOGDEBUG("request does not contain process data");
        }
        std::string process_name = request->process_data().process_name();
        LOGDEBUG("try starting process " << process_name);
        if (_process_map.count(process_name) > 0) {
          LOGDEBUG("ProcessDefinition found for " << process_name);
          //if(!_process_map.find(process_name)->second->isRunning())
          //  _process_map.find(process_name)->second->run();
        }
      }

      void ProcessServiceImpl::stopProcess(google::protobuf::RpcController* controller,
              const org::esb::rpc::ProcessRequest* request,
              org::esb::rpc::ProcessResponse* response,
              google::protobuf::Closure* done) {
        return;
        if (!request->has_process_data()) {
          LOGDEBUG("request does not contain process data");
        }
        std::string process_name = request->process_data().process_name();
        LOGDEBUG("try stopping process " << process_name);
        if (_process_map.count(process_name) > 0) {
          LOGDEBUG("ProcessDefinition found for " << process_name);
          try{
            _process_map.find(process_name)->second->stop();
          }catch(ProcessException & ex){
            LOGERROR(ex.what())
            try{
            _process_map.find(process_name)->second->kill();
            }catch(ProcessException & ex2){
              LOGERROR(ex2.what());
            }
          }
        }
      }
    }
  }
}
