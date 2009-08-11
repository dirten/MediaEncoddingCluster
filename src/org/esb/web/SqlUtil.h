#ifndef ORG_ESB_WEB_SQLUTIL
#define ORG_ESB_WEB_SQLUTIL
#include <map>
#include <string>
namespace org {
  namespace esb {
    namespace web {

      class SqlUtil {
      private:
        SqlUtil();
        ~SqlUtil();
      public:
        static void map2sql(std::string tablename, std::map<std::string, std::string>);
        static void sql2map(std::string tablename, int key, std::map<std::string, std::string>&);
      };
    }
  }
}
#endif

