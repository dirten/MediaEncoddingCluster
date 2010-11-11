#ifndef ORG_ESB_HIVE_JOBUTIL_H
#define ORG_ESB_HIVE_JOBUTIL_H

#include <string>
#include "org/esb/db/hivedb.hpp"
#include "org/esb/lang/Ptr.h"
#include "org/esb/util/Log.h"
//int jobcreator(int fileid, int profileid, std::string outpath);
//int jobcreator(db::MediaFile, db::Profile, std::string outpath);
//int jobcreator(int argc, char*argv[]);

namespace org {
  namespace esb {
    namespace hive {

      class JobUtil {
        classlogger("org::esb::hive::JobUtil");
      public:
        static void createJob(Ptr<db::Project> p);
        static int createJob(db::MediaFile, db::Profile, std::string outpath);
        static int createJob(db::MediaFile, db::Preset, std::string outpath);
      private:
        JobUtil();
        ~JobUtil();
      };

    }
  }
}
#endif


