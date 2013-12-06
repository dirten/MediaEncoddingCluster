/* 
 * File:   Environment.h
 * Author: HoelscJ
 *
 * Created on 2. November 2011, 13:22
 */

#ifndef ENVIRONMENT_H
#define	ENVIRONMENT_H
#include "exports.h"
namespace org {
  namespace esb {
    namespace hive {

      class HIVE_EXPORT Environment {
      public:
        static void build(int argc, char**argv);
        static std::string get(std::string key, std::string def="");
      private:
        Environment();
        virtual ~Environment();

      };
    }
  }
}

#endif	/* ENVIRONMENT_H */

