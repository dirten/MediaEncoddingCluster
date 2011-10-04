/* 
 * File:   PluginContext.h
 * Author: HoelscJ
 *
 * Created on 4. Oktober 2011, 10:52
 */

#ifndef PLUGINCONTEXT_H
#define	PLUGINCONTEXT_H
#include <map>
#include <string>
#include "org/esb/db/hivedb.hpp"
namespace org {
  namespace esb {
    namespace core {

      class PluginContext {
      public:
        db::HiveDb * database;
        std::map<std::string, std::string> env;
      private:
        friend class PluginRegistry;
        PluginContext();
        virtual ~PluginContext();

      };
    }
  }
}

#endif	/* PLUGINCONTEXT_H */

