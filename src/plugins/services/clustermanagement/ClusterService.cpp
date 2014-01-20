#include "ClusterService.h"
#include <string>
#include "org/esb/lang/Ptr.h"
#include "org/esb/signal/Messenger.h"
#include "org/esb/grid/GridNode.h"

namespace plugin{

  ClusterService::ClusterService()
  {
  }

  void ClusterService::onMessage(org::esb::signal::Message &msg) {
    if (msg.containsProperty("node_up_event")) {
      LOGDEBUG("Server Up event received:"<< msg.getProperty<org::esb::grid::GridNode> ("node_up_event"));
      std::string host = msg.getProperty<std::string > ("host");
      int port = msg.getProperty<int>("port");

    } else if (msg.containsProperty("server_down_event")) {
      LOGDEBUG("Server Down event received");
    } else if (msg.containsProperty("node_list")) {
      LOGDEBUG("node_list request received");
    }
  }

  void ClusterService::startService()
  {
    org::esb::signal::Messenger::getInstance().addMessageListener(*this);

  }

  void ClusterService::stopService()
  {

  }


  org::esb::core::OptionsDescription ClusterService::getOptionsDescription()
  {
    org::esb::core::OptionsDescription result("clusterservice");

    return result;
  }

}
