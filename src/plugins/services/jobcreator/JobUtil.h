#ifndef ORG_ESB_HIVE_JOBUTIL_H
#define ORG_ESB_HIVE_JOBUTIL_H

#include <string>
#include "org/esb/db/hivedb.hpp"
#include "org/esb/lang/Ptr.h"
#include "org/esb/util/Log.h"
#include "org/esb/core/ServicePlugin.h"
#include "org/esb/signal/MessageListener.h"
#include "exports.h"
//int jobcreator(int fileid, int profileid, std::string outpath);
//int jobcreator(db::MediaFile, db::Profile, std::string outpath);
//int jobcreator(int argc, char*argv[]);

namespace org {
  namespace esb {
    namespace plugin {

      class JOBCREATOR_EXPORT JobUtil:public org::esb::core::ServicePlugin, org::esb::signal::MessageListener {
        classlogger("org::esb::plugin::JobUtil");
      public:
        JobUtil();
        ~JobUtil();
        void startService();
        void stopService();
        void onMessage(org::esb::signal::Message & msg);

      private:
        void createJob(Ptr<db::Project> p);
        int createJob(db::MediaFile, db::Profile, std::string outpath);
        int createJob(db::MediaFile, db::Preset, std::string outpath);
      };
        REGISTER_SERVICE("jobcreator", JobUtil)
    }
  }
}
#endif


