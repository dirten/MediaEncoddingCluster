#ifndef STORAGESERVICE_H
#define STORAGESERVICE_H
#include "org/esb/core/ServicePlugin.h"
#include "org/esb/core/StoragePlugin.h"
#include "org/esb/signal/MessageListener.h"
#include "org/esb/signal/Message.h"
#include "org/esb/util/Log.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/core/StorageEngine.h"
namespace mhivestorage{
  class StorageService : public org::esb::core::StoragePlugin, org::esb::signal::MessageListener
  {
    public:
      StorageService();
      void onMessage(org::esb::signal::Message &);
      void startup();
      void shutdown();
      org::esb::core::OptionsDescription getOptionsDescription();
      //org::esb::core::ServicePlugin::ServiceType getServiceType();
    private:
      Ptr<org::esb::core::StorageEngine> _storageEngine;
  };
}

#endif // STORAGESERVICE_H
