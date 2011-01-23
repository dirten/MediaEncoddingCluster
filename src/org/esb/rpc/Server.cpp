/* 
 * File:   Server.cpp
 * Author: HoelscJ
 * 
 * Created on 12. Januar 2011, 18:49
 */

#include "Server.h"
#include "Controller.h"
#include "org/esb/rpc/rpc.pb.h"
#include "google/protobuf/descriptor.h"
#include "org/esb/util/Log.h"
#include "org/esb/signal/Messenger.h"
#include "org/esb/signal/Message.h"
#include "org/esb/io/StringOutputStream.h"
#include "org/esb/io/ObjectOutputStream.h"

#include "org/esb/hive/job/ProcessUnit.h"
namespace org {
  namespace esb {
    namespace rpc {

      class ConcreteDiscoveryService : public org::esb::rpc::DiscoveryService {
      public:

        ConcreteDiscoveryService(Server *aServer) : DiscoveryService(), mServer(aServer) {
        };

        void QueryForService(google::protobuf::RpcController* controller,
                const org::esb::rpc::QueryForServiceRequest* request,
                org::esb::rpc::QueryForServiceResponse* response,
                google::protobuf::Closure* done) {
          assert(mServer);
          google::protobuf::Service *aService = mServer->ServiceRegisteredForName(request->service_name());

          if (aService) {
            response->set_registered(true);
            response->set_service_name(aService->GetDescriptor()->full_name());
          } else {
            response->set_registered(false);
            response->set_service_name(request->service_name());
          }

        }

      private:
        Server *mServer;
      };

      Server::Server(int port) : _port(port) {
        registerService(new ConcreteDiscoveryService(this));
        //registerService(new NodeServiceImpl());
        //registerService(new ProcessUnitServiceImpl(this));
      }

      Server::~Server() {
        LOGDEBUG("shutting down Server!");
        std::map<const std::string, google::protobuf::Service*>::iterator it = mServiceInstanceMap.begin();

        for (; it != mServiceInstanceMap.end(); it++) {
          delete (*it).second;
        }
        mServiceInstanceMap.clear();
      }

      void Server::start() {
        try {
          _server = new org::esb::net::TcpServerSocket(_port);
          _server->bind();

          for (; true;) {
            org::esb::net::TcpSocket * clientSocket = _server->accept();
            boost::thread(boost::bind(&Server::handleClient, this, clientSocket));
            //handleClient(clientSocket);
          }
        } catch (std::exception & ex) {
          LOGERROR(ex.what());
        }
      }

      void Server::run() {
        boost::thread(boost::bind(&Server::start, this));
      }

      void Server::handleClient(org::esb::net::TcpSocket* s) {
        while (s->isConnected()) {
          std::string buffer;
          s->getInputStream()->read(buffer);
          if (!s->isConnected())return;
          if (buffer.length() <= 0) {
            LOGWARN("Request buffer is empty!");
            return;
          } else {
            LOGINFO("Request buffer size = " << buffer.length());
          }
          org::esb::rpc::Request rpcRequest;
          rpcRequest.ParseFromString(buffer);

          google::protobuf::Service* service = NULL;
          org::esb::rpc::Response anRPCResponse;

          service = mServiceInstanceMap[rpcRequest.service_name().c_str()];
          if (!service) {
            std::ostringstream errorString;
            errorString << "Service " << rpcRequest.service_name() << " not registered.";
            anRPCResponse.set_error_reason(SERVICE_NOT_FOUND);
            anRPCResponse.set_error(errorString.str());
            //return anRPCResponse;

          } else {
            Controller controller;

            const google::protobuf::MethodDescriptor* method = service->GetDescriptor()->FindMethodByName(rpcRequest.method_name());

            google::protobuf::Message *request = service->GetRequestPrototype(method).New();
            google::protobuf::Message *response = service->GetResponsePrototype(method).New();
            request->ParseFromString(rpcRequest.request_proto());
            LOGDEBUG("RequestProto:" << rpcRequest.request_proto());
            /*
            RPCCompletionStatus *completionStatus = new RPCCompletionStatus;
            completionStatus->rpcController = &controller;
            completionStatus->response = &anRPCResponse;
             */
            service->CallMethod(method, &controller, request, response, NULL /*NewCallback(this, &Server::ExecuteRPCDone, completionStatus)*/);
            delete request;
            std::string responseString;
            response->SerializeToString(&responseString);
            delete response;

            anRPCResponse.set_response_proto(responseString);
            if (controller.Failed()) {
              anRPCResponse.set_error_reason(RPC_FAILED);
              anRPCResponse.set_error(controller.ErrorText());
            }
          }
          std::string out;
          anRPCResponse.SerializeToString(&out);

          LOGDEBUG("send response size:" << out.length());
          s->getOutputStream()->write(out);
          LOGDEBUG("response sended");
        }
      }

      void Server::registerService(google::protobuf::Service *aService) {
        LOGDEBUG("register service : " << aService->GetDescriptor()->full_name());
        mServiceInstanceMap[aService->GetDescriptor()->full_name()] = aService;
      }

      google::protobuf::Service *Server::ServiceRegisteredForName(const std::string &serviceName) {
        return mServiceInstanceMap[serviceName];
      }
    }
  }
}
