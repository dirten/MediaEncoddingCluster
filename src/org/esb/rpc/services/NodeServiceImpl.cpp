/* 
 * File:   NodeServiceImpl.cpp
 * Author: HoelscJ
 * 
 * Created on 14. Januar 2011, 16:08
 */

#include "NodeServiceImpl.h"
#include "org/esb/net/TcpSocket.h"
#include "../Channel.h"
#include "../Controller.h"

namespace org {
  namespace esb {
    namespace rpc {

      NodeServiceImpl::NodeServiceImpl() : org::esb::rpc::NodeService() {
      }

      NodeServiceImpl::~NodeServiceImpl() {
      }

      void NodeServiceImpl::registerNode(::google::protobuf::RpcController* controller, const ::org::esb::rpc::Node* request, ::org::esb::rpc::NodeList* response, ::google::protobuf::Closure* done) {
        LOGDEBUG("NodeServiceImpl::registerNode");

        if (!_node_set.count(*request)) {
          LOGDEBUG("register node " << request->DebugString());
          _node_set.insert(*request);
          propagateNewNode(*request);
        } else {
          LOGDEBUG("node allready present\n" << request->DebugString());
        }
        //org::esb::rpc::Node *newnode = request;
        std::set<org::esb::rpc::Node>::iterator it = _node_set.begin();
        for (; it != _node_set.end(); it++) {
          org::esb::rpc::Node *node = response->add_nodes();
          node->MergeFrom((*it));
        }
      }

      void NodeServiceImpl::unregisterNode(::google::protobuf::RpcController* controller, const ::org::esb::rpc::Node* request, ::org::esb::rpc::NodeList* response, ::google::protobuf::Closure* done) {
        LOGDEBUG("NodeServiceImpl::unregisterNode");

        if (_node_set.count(*request)) {
          _node_set.erase(*request);
          propagateLostNode(*request);
        } else {
          LOGDEBUG("node not present\n" << request->DebugString());
        }
        std::set<org::esb::rpc::Node>::iterator it = _node_set.begin();
        for (; it != _node_set.end(); it++) {
          org::esb::rpc::Node *node = response->add_nodes();
          node->MergeFrom((*it));
        }
      }

      void NodeServiceImpl::propagateNewNode(const org::esb::rpc::Node & node) {
        std::set<org::esb::rpc::Node>::iterator it = _node_set.begin();
        for (; it != _node_set.end(); it++) {
          try {
            org::esb::net::TcpSocket socket(node.address(), 6000);
            socket.connect();
            org::esb::rpc::Channel c(&socket);

            org::esb::rpc::NodeService::Stub nodeS(&c);
            org::esb::rpc::NodeList nodelist;
            org::esb::rpc::Controller ctrl;
            nodeS.registerNode(&ctrl, &node, &nodelist, NULL);

          } catch (std::exception & ex) {
            LOGERROR(ex.what());
          }
        }
      }

      void NodeServiceImpl::propagateLostNode(const org::esb::rpc::Node& node) {
        std::set<org::esb::rpc::Node>::iterator it = _node_set.begin();
        for (; it != _node_set.end(); it++) {
          try {
            org::esb::net::TcpSocket socket(node.address(), 6000);
            socket.connect();
            org::esb::rpc::Channel c(&socket);

            org::esb::rpc::NodeService::Stub nodeS(&c);
            org::esb::rpc::NodeList nodelist;
            org::esb::rpc::Controller ctrl;
            nodeS.unregisterNode(&ctrl, &node, &nodelist, NULL);

          } catch (std::exception & ex) {
            LOGERROR(ex.what());
          }
        }
      }
    }
  }
}
