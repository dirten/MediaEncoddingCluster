#ifndef ORG_ESB_HIVE_JOBUTIL_H
#define ORG_ESB_HIVE_JOBUTIL_H

#include <string>
int jobcreator(int fileid, int profileid, std::string outpath);
int jobcreator(int argc, char*argv[]);

#include "org/esb/db/hivedb.hpp"
#include "org/esb/lang/Ptr.h"
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


