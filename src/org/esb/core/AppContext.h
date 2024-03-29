/* 
 * File:   AppContext.h
 * Author: HoelscJ
 *
 * Created on 29. August 2011, 15:52
 */

#ifndef APPCONTEXT_H
#define	APPCONTEXT_H
#include <map>
#include <string>
#include "exports.h"
namespace org {
  namespace esb {
    namespace core {

      class CORE_EXPORT AppContext {
      public:
        std::map<std::string, std::string> env;
      private:
        friend class PluginRegistry;
        AppContext();
        virtual ~AppContext();

      };
    }
  }
}

#endif	/* APPCONTEXT_H */

