#include "GridRegistry.h"

namespace org{
  namespace esb{
    namespace grid{

      GridRegistry * GridRegistry::_instance=NULL;

      GridRegistry::GridRegistry()
      {
        org::esb::signal::Messenger::getInstance().addMessageListener(*this);
      }

      void GridRegistry::init(){
        if(_instance==NULL){
          _instance=new GridRegistry();
        }
      }

      void GridRegistry::addNode(GridNode &node){
        _instance->_nodes.push_back(node);
      }

      void GridRegistry::removeNode(GridNode &node){
        _instance->_nodes.remove(node);
      }

      const std::list<GridNode> GridRegistry::nodes(){
        return _instance->_nodes;
      }

      void GridRegistry::onMessage(org::esb::signal::Message &msg) {
        if (msg.containsProperty("node_up_event")) {
          LOGDEBUG("Node Up event received:"<<  msg.getProperty<org::esb::grid::GridNode> ("node_up_event").toString());
          GridNode node=msg.getProperty<org::esb::grid::GridNode> ("node_up_event");
          _instance->removeNode(node);
          _instance->addNode(node);
        } else if (msg.containsProperty("node_down_event")) {
          LOGDEBUG("Server Down event received");
          GridNode node=msg.getProperty<org::esb::grid::GridNode> ("node_down_event");
          _instance->removeNode(node);
          _instance->addNode(node);
        } else if (msg.containsProperty("node_list")) {
          LOGDEBUG("node_list request received");
          msg.setProperty("node_list", _instance->nodes());
        }
      }
    }
  }
}
