#ifndef WRITERSERVICE_H
#define WRITERSERVICE_H
#include "org/esb/signal/MessageListener.h"
#include "org/esb/core/ServicePlugin.h"

namespace plugin {
  class WriterService : public org::esb::core::ServicePlugin , org::esb::signal::MessageListener
  {
    public:
      WriterService();
      void onMessage(org::esb::signal::Message &);
      void startService();
      void stopService();
      org::esb::core::OptionsDescription getOptionsDescription();
      org::esb::core::ServicePlugin::ServiceType getServiceType();
      void init();
    private:
      enum STATUS {
        NONE,
        RUNNING,
        STOPPING,
        STOPPED
      };

      void run();
      STATUS _status;
  };
  REGISTER_SERVICE("writerservice", WriterService)
}
#endif // WRITERSERVICE_H
