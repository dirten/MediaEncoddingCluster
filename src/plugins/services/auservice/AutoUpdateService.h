#ifndef AUTOUPDATESERVICE_H
#define AUTOUPDATESERVICE_H
#include "org/esb/core/ServicePlugin.h"
#include "org/esb/signal/MessageListener.h"
#include "org/esb/signal/Message.h"
#include "org/esb/util/Log.h"
#include "org/esb/lang/Thread.h"

namespace auservice {

  class AutoUpdateService: public org::esb::core::ServicePlugin, org::esb::signal::MessageListener
  {
      classlogger("auservice.AutoUpdateService")
    public:
      AutoUpdateService();
      ~AutoUpdateService();
      void onMessage(org::esb::signal::Message &);
      void startService();
      void stopService();
      org::esb::core::OptionsDescription getOptionsDescription();
      //org::esb::core::ServicePlugin::ServiceType getServiceType();
      void init();
      void run();
    private:
      std::string _host;
      std::string _port;
      bool _stop;

  };
  REGISTER_SERVICE("auservice", AutoUpdateService)
}
#endif // AUTOUPDATESERVICE_H
