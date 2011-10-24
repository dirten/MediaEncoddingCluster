/* 
 * File:   PluginContext.h
 * Author: HoelscJ
 *
 * Created on 4. Oktober 2011, 10:52
 */

#ifndef PLUGINCONTEXT_H
#define	PLUGINCONTEXT_H
//#include "org/esb/db/hivedb.hpp"
#include <map>
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
namespace db{
  class HiveDb;
}
namespace org {
  namespace esb {
    namespace core {

      class PluginContext {
      public:
        db::HiveDb * database;
        template<typename T>
        T getEnvironment(std::string key){
          return boost::lexical_cast<T>(env[key]);
        }
        virtual ~PluginContext();
      private:
        friend class PluginRegistry;
        PluginContext();
        std::map<std::string, std::string> env;
      };
    }
  }
}

#endif	/* PLUGINCONTEXT_H */

