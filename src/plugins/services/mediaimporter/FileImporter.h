#ifndef ORG_ESB_HIVE_FILEIMPORTER_H
#define ORG_ESB_HIVE_FILEIMPORTER_H

#include "org/esb/io/File.h"
#include "org/esb/core/ServicePlugin.h"
#include "org/esb/signal/MessageListener.h"
#include "exports.h"
namespace db {
  class HiveDb;
}
namespace org {
  namespace esb {
    namespace plugin {

      class MEDIAIMPORTER_EXPORT FileImporter : public org::esb::core::ServicePlugin, org::esb::signal::MessageListener {
      public:
        FileImporter();
        db::MediaFile import(org::esb::io::File);
        void startService();
        void stopService();
        void onMessage(org::esb::signal::Message & msg);
        org::esb::core::OptionsDescription getOptionsDescription();
        org::esb::core::ServicePlugin::ServiceType getServiceType();

      private:

      };
      REGISTER_SERVICE("mediaimporter", FileImporter)
    }
  }
}
#endif
