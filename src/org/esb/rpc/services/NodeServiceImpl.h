/* 
 * File:   NodeServiceImpl.h
 * Author: HoelscJ
 *
 * Created on 14. Januar 2011, 16:08
 */

#ifndef NODESERVICEIMPL_H
#define	NODESERVICEIMPL_H
#include "org/esb/rpc/rpc.pb.h"
#include "org/esb/util/Log.h"
#include <set>
namespace org {
  namespace esb {
    namespace rpc {

      class NodeServiceImpl : public org::esb::rpc::NodeService {
        classlogger("org.esb.rpc.NodeService")
      public:
        NodeServiceImpl();
        virtual ~NodeServiceImpl();
        void registerNode(::google::protobuf::RpcController* controller, const ::org::esb::rpc::Node* request, ::org::esb::rpc::NodeList* response, ::google::protobuf::Closure* done);
        void unregisterNode(::google::protobuf::RpcController* controller, const ::org::esb::rpc::Node* request, ::org::esb::rpc::NodeList* response, ::google::protobuf::Closure* done);

      private:

        struct nodecomp {
          bool operator() (const Node& lhs, const Node& rhs) const {
            return lhs.name()<rhs.name();
          }
        };

        std::set<org::esb::rpc::Node, nodecomp> _node_set;
        void propagateNewNode(const org::esb::rpc::Node&);
        void propagateLostNode(const org::esb::rpc::Node&);
      };
    }
  }
}

#endif	/* NODESERVICEIMPL_H */

