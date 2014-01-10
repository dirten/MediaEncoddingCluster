#ifndef CLUSTERSERVICE_H
#define CLUSTERSERVICE_H
#include "org/esb/core/ServicePlugin.h"

namespace plugin{
  class ClusterService: public org::esb::core::ServicePlugin
  {
    public:
      ClusterService();
      void startService();
      void stopService();
      org::esb::core::OptionsDescription getOptionsDescription();
    private:
      void run();
  };
  REGISTER_SERVICE("clustermanagement", ClusterService)

}
#endif // CLUSTERSERVICE_H
