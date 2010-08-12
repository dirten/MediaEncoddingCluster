#ifndef ORG_ESB_HIVE_JOBUTIL_H
#define ORG_ESB_HIVE_JOBUTIL_H

#include <string>
#include "org/esb/db/hivedb.hpp"
#include "org/esb/lang/Ptr.h"

int jobcreator(int fileid, int profileid, std::string outpath);
int jobcreator(db::MediaFile, db::Profile, std::string outpath);
int jobcreator(int argc, char*argv[]);

namespace org {
  namespace esb {
    namespace hive {

      class JobUtil {
      public:
        static void createJob(Ptr<db::Project> p);
        static void createJob(db::MediaFile, db::Profile, std::vector<db::Filter>, std::string outpath);
      private:
        JobUtil();
        ~JobUtil();
      };

    }
  }
}
#endif


