#ifndef ORG_ESB_WEB_SQLUTIL
#define ORG_ESB_WEB_SQLUTIL
#include <map>
#include <string>
#include <stdint.h>
#include "org/esb/util/Log.h"
namespace org {
  namespace esb {
    namespace web {

      class SqlUtil {
        classlogger("org.esb.web.SqlUtil")
      private:
        SqlUtil();
        ~SqlUtil();
      public:
        static int64_t map2sql(std::string tablename, std::map<std::string, std::string>);
        static void sql2map(std::string tablename, int key, std::map<std::string, std::string>&);
        static void sql2map(std::string tablename,std::string keyfield, std::string key, std::map<std::string, std::string>&);
      };
    }
  }
}
#endif

