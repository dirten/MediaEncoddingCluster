#ifndef STORAGESERVICE_H
#define STORAGESERVICE_H
#include "org/esb/core/ServicePlugin.h"
#include "org/esb/signal/MessageListener.h"
#include "org/esb/signal/Message.h"
#include "org/esb/util/Log.h"
#include "org/esb/lang/Thread.h"
#include "engines/Engine.h"
namespace mhivestorage{
  class StorageService : public org::esb::core::ServicePlugin, org::esb::signal::MessageListener
  {
    public:
      StorageService();
      void onMessage(org::esb::signal::Message &);
      void startService();
      void stopService();
      org::esb::core::OptionsDescription getOptionsDescription();
      //org::esb::core::ServicePlugin::ServiceType getServiceType();
    private:
      Ptr<engines::Engine> _storageEngine;
  };
}

#endif // STORAGESERVICE_H
