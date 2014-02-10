/* 
 * File:   Environment.h
 * Author: HoelscJ
 *
 * Created on 2. November 2011, 13:22
 */

#ifndef ENVIRONMENT_H
#define	ENVIRONMENT_H
#include <map>
#include <vector>
#include "exports.h"

namespace org {
  namespace esb {
    namespace hive {

      class HIVE_EXPORT Environment {
        public:
          enum SYSTEM_TYPE{
            UNKNOWN ,
            WINDOWS,
            DARWIN,
            LINUX
          };
          const static std::string BASE_PATH;
          const static std::string USER_HOME;
          const static std::string PLUGIN_PATH;
          const static std::string EXE_PATH;
          const static std::string EXE_NAME;
          const static std::string SYSTEM;
          const static std::string DB_URL;
          static void build(int argc, char**argv);
          static std::string get(std::string key, std::string def="");
          static void set(std::string key, std::string val);
          static std::vector<std::string> getArguments();
          static void close();
        private:
          Environment();
          virtual ~Environment();
          static std::map<std::string, std::string> _environmentMap;
          static std::vector<std::string> _argumentMap;
      };
    }
  }
}

#endif	/* ENVIRONMENT_H */

