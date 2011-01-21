/* 
 * File:   Channel.h
 * Author: HoelscJ
 *
 * Created on 11. Januar 2011, 17:07
 */

#ifndef CHANNEL_H
#define	CHANNEL_H
#include "google/protobuf/service.h"
#include "org/esb/net/TcpSocket.h"
namespace org {
  namespace esb {
    namespace rpc {

      class Channel : public google::protobuf::RpcChannel {
      public:
        Channel(org::esb::net::TcpSocket *);
        virtual ~Channel();
        // Call the given method of the remote service.  The signature of this
        // procedure looks the same as Service::CallMethod(), but the requirements
        // are less strict in one important way:  the request and response objects
        // need not be of any specific class as long as their descriptors are
        // method->input_type() and method->output_type().
        virtual void CallMethod(const google::protobuf::MethodDescriptor* method,
                google::protobuf::RpcController* controller,
                const google::protobuf::Message* request,
                google::protobuf::Message* response,
                google::protobuf::Closure* done);


      private:
        org::esb::net::TcpSocket * _socket;

      };
    }
  }
}
#endif	/* CHANNEL_H */

