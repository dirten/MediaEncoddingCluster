#ifndef CLUSTERSERVICE_H
#define CLUSTERSERVICE_H
#include <list>
#include "org/esb/core/ServicePlugin.h"
#include "org/esb/signal/Message.h"
#include "org/esb/signal/MessageListener.h"
#include "org/esb/util/Log.h"
namespace plugin{
  class ClusterService: public org::esb::core::ServicePlugin, org::esb::signal::MessageListener
  {
      classlogger("clustermanagement.ClusterService")
    public:
      ClusterService();
      void onMessage(org::esb::signal::Message &);
      void startService();
      void stopService();
      org::esb::core::OptionsDescription getOptionsDescription();
    private:
      std::list<std::string> _nodes;
  };
  REGISTER_SERVICE("clustermanagement", ClusterService)

}
#endif // CLUSTERSERVICE_H
