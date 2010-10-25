/* 
 * File:   DatabaseConfigLoader.cpp
 * Author: HoelscJ
 * 
 * Created on 20. September 2010, 14:52
 */
#include "hivedb.hpp"
#include "org/esb/config/config.h"
#include "DatabaseConfigLoader.h"
#include "org/esb/hive/DatabaseService.h"

namespace org {
  namespace esb {
    namespace db {

      void DatabaseConfigLoader::load() {
        /*load params from database*/
        /*try {
          if (std::string(org::esb::config::Config::getProperty("db.url")).length() > 0) {
            ::db::HiveDb db=org::esb::hive::DatabaseService::getDatabase();
            vector< ::db::Config> configs = litesql::select< ::db::Config > (db).all();
            vector< ::db::Config>::iterator confit = configs.begin();
            for (; confit != configs.end(); confit++) {
              if ((*confit).configkey != "db.url" &&
                      (*confit).configkey != "mode.client" &&
                      (*confit).configkey != "mode.server" &&
                      (*confit).configkey != "hive.base_path" &&
                      (*confit).configkey != "hive.mode") {
                org::esb::config::Config::setProperty((*confit).configkey.value(), (*confit).configval.value());
              }
            }
          }
        } catch (...) {
          
        }*/
      }

      void DatabaseConfigLoader::save() {

      }

      DatabaseConfigLoader::DatabaseConfigLoader() {
      }

      DatabaseConfigLoader::DatabaseConfigLoader(const DatabaseConfigLoader& orig) {
      }

      DatabaseConfigLoader::~DatabaseConfigLoader() {
      }
    }
  }
}

