/* 
 * File:   Client.cpp
 * Author: HoelscJ
 * 
 * Created on 12. Januar 2011, 20:18
 */

#include "Client.h"
#include "Channel.h"
#include "org/esb/net/TcpSocket.h"
#include "rpc.pb.h"
#include "Controller.h"
#include "org/esb/util/Log.h"
#include "org/esb/io/StringInputStream.h"
#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/hive/job/ProcessUnit.h"

namespace org {
  namespace esb {
    namespace rpc {

      Client::Client(std::string host, int port) {
        org::esb::net::TcpSocket socket(host, port);
        try {
          socket.connect();
        } catch (std::exception & ex) {
          std::cout << ex.what() << std::endl;
        }
        if (socket.isConnected()) {
          Channel c(&socket);

          org::esb::rpc::ProcessService::Stub nodeS(&c);
          org::esb::rpc::DiscoveryService::Stub s1(&c);

          org::esb::rpc::QueryForServiceRequest request;
          org::esb::rpc::QueryForServiceResponse response;
          org::esb::rpc::Controller ctrl;
          request.set_service_name("org.esb.rpc.ProcessService");

          s1.QueryForService(&ctrl, &request, &response, NULL);
          /*
          if(ctrl.Failed()){
            LOGERROR("Service request failed:"<<ctrl.ErrorText());
          }
           */
          if (false && response.registered()) {
            LOGDEBUG("service " << response.service_name() << " registered");
            org::esb::rpc::Node node;

            org::esb::rpc::ProcessUnit pu;
            org::esb::rpc::ProcessUnitRequest pureq;
            org::esb::rpc::ProcessUnitResponse pures;

            org::esb::rpc::Controller ctrl;
            node.set_address("127.0.0.1");
            node.set_name("test node");
            *pureq.mutable_node() = node;
            while (true) {
              //nodeS.getProcessUnit(&ctrl, &pureq, &pures, NULL);
              if (ctrl.Failed()) {
                LOGERROR("Service request failed:" << ctrl.ErrorText());
                break;
              }

              //LOGDEBUG("NodeList:"<<pu.mutable_serialized_data()->length());
              std::string *indata = pures.mutable_unit_data()->mutable_serialized_data();
              LOGDEBUG("received datasize=" << indata->length());
              org::esb::io::StringInputStream sis(*indata);
              org::esb::io::ObjectInputStream ois(&sis);
              org::esb::hive::job::ProcessUnit unit;
              ois.readObject(unit);
              if (unit._input_packets.size() == 0) {
                break;
              }
              unit.process();
            }
          }
          if (response.registered()) {
            LOGDEBUG("service " << response.service_name() << " registered");
            org::esb::rpc::Node node;

            org::esb::rpc::Process proc;
            org::esb::rpc::ProcessRequest pcreq;
            org::esb::rpc::ProcessResponse pcres;

            org::esb::rpc::Controller ctrl;
            node.set_address("127.0.0.1");
            node.set_name("test node");
            *pcreq.mutable_node() = node;
            proc.set_process_name("core");
            *pcreq.mutable_process_data()=proc;
            nodeS.startProcess(&ctrl, &pcreq, &pcres, NULL);
            if (ctrl.Failed()) {
              LOGERROR("Service request failed:" << ctrl.ErrorText());
              //break;
            }
          }

        }
      }

      Client::~Client() {
      }
    }
  }
}
