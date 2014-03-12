/* 
 * File:   ProcessUnitServiceImpl.cpp
 * Author: HoelscJ
 * 
 * Created on 14. Januar 2011, 11:13
 */

#include "ProcessUnitServiceImpl.h"
#include "org/esb/signal/Message.h"
#include "org/esb/signal/Messenger.h"
#include "org/esb/io/StringOutputStream.h"
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/hive/job/ProcessUnit.h"
namespace org {
  namespace esb {
    namespace rpc {

      ProcessUnitServiceImpl::ProcessUnitServiceImpl(Server *aServer) : ProcessUnitService(), mServer(aServer) {

      }

      ProcessUnitServiceImpl::~ProcessUnitServiceImpl() {
      }

      void ProcessUnitServiceImpl::getProcessUnit(google::protobuf::RpcController* controller,
              const org::esb::rpc::ProcessUnitRequest* request,
              org::esb::rpc::ProcessUnitResponse* response,
              google::protobuf::Closure* done) {
        assert(mServer);
        LOGDEBUG("getProcessUnit");
        LOGDEBUG(request->node().DebugString());

        org::esb::signal::Message msg;
        msg.setProperty("processunitcontroller", std::string("GET_PROCESS_UNIT"));
        org::esb::signal::Messenger::getInstance().sendRequest(msg);
        boost::shared_ptr<org::esb::hive::job::ProcessUnit >un = msg.getProperty<boost::shared_ptr<org::esb::hive::job::ProcessUnit > >("processunit");
        if (!un) {
          controller->SetFailed("could not get the ProcessUnit from the ProcessUnitController");
          return;
        }
        std::string data;
        org::esb::io::StringOutputStream sos(data);
        org::esb::io::ObjectOutputStream oos(&sos);
        oos.writeObject(*un.get());
        response->mutable_unit_data()->set_serialized_data(data.c_str(), data.length());
        response->set_status("OK");
        LOGDEBUG("sended data length=" << data.length());
      }

      void ProcessUnitServiceImpl::putProcessUnit(google::protobuf::RpcController* controller,
              const org::esb::rpc::ProcessUnitRequest* request,
              org::esb::rpc::ProcessUnitResponse* response,
              google::protobuf::Closure* done) {
        LOGDEBUG("putProcessUnit");

      }

    }
  }
}
