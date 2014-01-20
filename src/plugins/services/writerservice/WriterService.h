#ifndef WRITERSERVICE_H
#define WRITERSERVICE_H
#include "org/esb/signal/MessageListener.h"
#include "org/esb/core/ServicePlugin.h"

namespace plugin {
  class WriterService : public org::esb::core::ServicePlugin
  {
    public:
      WriterService();
      void startService();
      void stopService();
      org::esb::core::OptionsDescription getOptionsDescription();
      org::esb::core::ServicePlugin::ServiceType getServiceType();
    private:
      enum STATUS {
        NONE,
        RUNNING,
        STOPPING,
        STOPPED
      };

      void run();
      void observeProcessUnits();
      STATUS _status;
  };
  REGISTER_SERVICE("writerservice", WriterService)
}
#endif // WRITERSERVICE_H
