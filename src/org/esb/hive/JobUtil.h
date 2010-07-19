#ifndef ORG_ESB_HIVE_JOBUTIL_H
#define ORG_ESB_HIVE_JOBUTIL_H

#include "org/esb/db/hivedb.hpp"
int jobcreator(int fileid, int profileid, std::string outpath);
int jobcreator(int argc, char*argv[]);

#endif
