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
#include "../core/exports.h"
#include <boost/any.hpp>
#include <boost/lexical_cast.hpp>

namespace org {
  namespace esb {
    namespace hive {

      class CORE_EXPORT Environment {
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
        const static std::string DB_TYPE;
        static void build(int argc, char**argv);


        template<class T>
        static T get(std::string key, std::string def="") {
          if(_env.find(key)==_env.end()){
            return getenv(key.c_str())!=NULL?getenv(key.c_str()):def;
          }else{
            return boost::any_cast<T > (_env[key]);
          }
        }
        static bool contains(std::string key);
        static boost::any getAnyType(std::string key);

        static std::string get(std::string key, std::string def="");

        static void set(std::string key, char * value);
        static void set(std::string key, boost::any val);
        static std::vector<std::string> getArguments();
        static void close();
        static std::map<std::string, boost::any> _env;
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

