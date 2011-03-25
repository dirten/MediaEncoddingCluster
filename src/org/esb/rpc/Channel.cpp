/* 
 * File:   Channel.cpp
 * Author: HoelscJ
 * 
 * Created on 11. Januar 2011, 17:07
 */

#include "Channel.h"
#include "org/esb/rpc/rpc.pb.h"
#include "google/protobuf/descriptor.h"

namespace org {
  namespace esb {
    namespace rpc {

      Channel::Channel(org::esb::net::TcpSocket * s):_socket(s) {
      }

      Channel::~Channel() {
      }
      // Call the given method of the remote service.  The signature of this
      // procedure looks the same as Service::CallMethod(), but the requirements
      // are less strict in one important way:  the request and response objects
      // need not be of any specific class as long as their descriptors are
      // method->input_type() and method->output_type().

      void Channel::CallMethod(const google::protobuf::MethodDescriptor* method,
              google::protobuf::RpcController* controller,
              const google::protobuf::Message* request,
              google::protobuf::Message* response,
              google::protobuf::Closure* done) {
        //std::cout << "void Channel::CallMethod"<<std::endl;
        org::esb::rpc::Request anRPCRequest;
        org::esb::rpc::Response anRPCResponse;
        std::string requestRawString;
        std::string RPCResponseString;

        request->SerializeToString(&requestRawString);
        anRPCRequest.set_request_proto(requestRawString);
        anRPCRequest.set_service_name(method->service()->full_name());
        anRPCRequest.set_method_name(method->name());

        if (_socket) {
          std::string outdata;
          anRPCRequest.SerializeToString(&outdata);
          _socket->getOutputStream()->write(outdata);

          std::string indata;
          _socket->getInputStream()->read(indata);
          anRPCResponse.ParseFromString(indata);
        } else {
          fprintf(stderr, "%s: No Server or Socket set !\n", __FUNCTION__);
        }


        if (anRPCResponse.has_error_reason() && anRPCResponse.error_reason() != NO_ERROR_OCCURRED) {
          controller->SetFailed(anRPCResponse.error());
        } else if (anRPCResponse.response_proto().size() > 0) {
          // Build a response from the response_proto returned by the RPC.
          response->ParseFromString(anRPCResponse.response_proto());
        }

        if (done) {
          done->Run();
        }


      }
    }
  }
}
