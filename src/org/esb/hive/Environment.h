/* 
 * File:   Environment.h
 * Author: HoelscJ
 *
 * Created on 2. November 2011, 13:22
 */

#ifndef ENVIRONMENT_H
#define	ENVIRONMENT_H
#include <map>
#include "exports.h"

namespace org {
  namespace esb {
    namespace hive {

      class HIVE_EXPORT Environment {
      public:
        static void build(int argc, char**argv);
        static std::string get(std::string key, std::string def="");
        static void set(std::string key, std::string val);
      private:
        Environment();
        virtual ~Environment();
        static std::map<std::string, std::string> _environmentMap;
      };
    }
  }
}

#endif	/* ENVIRONMENT_H */

