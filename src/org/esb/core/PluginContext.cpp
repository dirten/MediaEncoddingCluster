/* 
 * File:   PluginContext.cpp
 * Author: HoelscJ
 * 
 * Created on 4. Oktober 2011, 10:52
 */

#include "org/esb/db/hivedb.hpp"

#include "PluginContext.h"
#include "org/esb/config/config.h"

namespace org {
  namespace esb {
    namespace core {

      PluginContext::PluginContext() {
        database = new db::HiveDb("sqlite3", org::esb::config::Config::get("db.url"));
      }


      PluginContext::~PluginContext() {
        delete database;
      }
      
      bool PluginContext::contains(std::string key) {
        return _props.count(key)>0;
      }
    }
  }
}
