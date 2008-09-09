#ifndef ORG_ESB_HIVE_DATABASEUTIL_H
#define ORG_ESB_HIVE_DATABASEUTIL_H
#include "org/esb/sql/Connection.h"
using namespace org::esb::sql;
bool createDatabase(Connection & con);
bool checkDatabase(Connection & con);
#endif
