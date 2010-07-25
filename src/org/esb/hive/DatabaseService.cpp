/*----------------------------------------------------------------------
*  File    : DatabaseService.cpp
*  Author  : Jan Hölscher <jan.hoelscher@esblab.com>
*  Purpose : Global Database Service for the Database Connection
*  Created : 18 Feb 2009 by Jan Hölscher <jan.hoelscher@esblab.com>
*
*
* MediaEncodingCluster, Copyright (C) 2001-2009   Jan Hölscher
*
* This program is free software; you can redistribute it and/or
*  modify it under the terms of the GNU General Public License as
*  published by the Free Software Foundation; either version 2 of the
*  License, or (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software
*  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
*  02111-1307 USA
*
* ----------------------------------------------------------------------
*/
#include "org/esb/db/hivedb.hpp"
#include "DatabaseService.h"
#include "org/esb/signal/Messenger.h"
#include "org/esb/sql/my_sql.h"
#include "org/esb/util/Log.h"
#include "org/esb/util/StringTokenizer.h"

#include "org/esb/config/config.h"

#include <iostream>
namespace org {
  namespace esb {
    namespace hive {
      bool DatabaseService::_running = false;
      std::string DatabaseService::_base_path;
      std::map<boost::thread::id,int> DatabaseService::_thread_map;
      DatabaseService::DatabaseService(std::string base_path) {
        _base_path = base_path;
        _running = false;
      }

      void DatabaseService::onMessage(org::esb::signal::Message & msg) {
        if (msg.getProperty("databaseservice") == org::esb::hive::START) {
          start(_base_path);
        }
        if (msg.getProperty("databaseservice") == org::esb::hive::STOP) {
          stop();
        }
      }

      void DatabaseService::start(std::string base_path) {
        LOGINFO("starting Database Service");
        if (_running == false) {
          _base_path=base_path;
          std::string lang = "--language=";
          lang.append(base_path);
          lang.append("/res");

          std::string datadir = "--datadir=";
          datadir.append(base_path);
          datadir.append("/");
          char * dbdir = const_cast<char*> (datadir.c_str());
          char * langdir = const_cast<char*> (lang.c_str());

          static char *server_options[] = {
            const_cast<char*> ("dbservice"),
            const_cast<char*> (datadir.c_str()),
            const_cast<char*> (lang.c_str()),
            NULL
          };
          int num_elements = (sizeof (server_options) / sizeof (char *)) - 1;
          static char *server_groups[] = {
            const_cast<char*> ("embedded"),
            const_cast<char*> ("server"),
            const_cast<char*> ("dbservice_SERVER"),
            (char*) NULL
          };
          if (mysql_server_init(num_elements, server_options, NULL/*server_groups*/) > 0) {
            LOGFATAL("error initialising DatabaseService datadir=" << datadir << " resource=" << lang);
          }
          _running = true;
          LOGINFO("Database Service running");
        }
      }
      bool DatabaseService::databaseExist() {
        bool result=true;
        try{
          db::HiveDb db("mysql", org::esb::config::Config::getProperty("db.url"));
        }catch(...){
          result=false;
        }
        return result;
      }
      void DatabaseService::createDatabase() {
        using namespace org::esb::util;
        std::string url = org::esb::config::Config::getProperty("db.url");
        StringTokenizer tk_outer(url, ";");
        std::string create_url;
        std::string database_name;
        while (tk_outer.hasMoreTokens()) {
          std::string param = tk_outer.nextToken();
          StringTokenizer tk_inner(param, "=");
          if (tk_inner.nextToken() != "database") {
            create_url += param + ";";
          } else {
            database_name = tk_inner.nextToken();
          }
        }
        LOGDEBUG("Create Url=" + create_url);
        LOGDEBUG("Create Database name=" + database_name);
        db::HiveDb db("mysql", create_url);
        db.query("CREATE DATABASE " + database_name);

      }

      void DatabaseService::createTables() {
        db::HiveDb db("mysql", org::esb::config::Config::getProperty("db.url"));
        db.create();
      }

      void DatabaseService::updateTables() {
        db::HiveDb db("mysql", org::esb::config::Config::getProperty("db.url"));
        if (db.needsUpgrade()) {
          LOGDEBUG("Upgrade database");
          db.upgrade();
        }
      }

      void DatabaseService::dropDatabase() {
        using namespace org::esb::util;
        std::string url = org::esb::config::Config::getProperty("db.url");
        StringTokenizer tk_outer(url, ";");
        std::string create_url;
        std::string database_name;
        while (tk_outer.hasMoreTokens()) {
          std::string param = tk_outer.nextToken();
          StringTokenizer tk_inner(param, "=");
          if (tk_inner.nextToken() != "database") {
            create_url += param + ";";
          } else {
            database_name = tk_inner.nextToken();
          }
        }
        LOGDEBUG("Drop Url=" + create_url);
        LOGDEBUG("Drop Database name=" + database_name);

        db::HiveDb db("mysql", create_url);
        db.query("DROP DATABASE " + database_name);
      }

      void DatabaseService::dropTables() {
        db::HiveDb db("mysql", org::esb::config::Config::getProperty("db.url"));
        db.drop();
      }

      void DatabaseService::loadPresets() {
        db::HiveDb db("mysql", org::esb::config::Config::getProperty("db.url"));

        db.query("load data infile '"+_base_path+"/sql/config.txt' IGNORE into table Config_ fields terminated by \":\"");
        db.query("load data infile '"+_base_path+"/sql/profiles.txt' IGNORE into table Profile_ fields terminated by \",\"");
        db.query("load data infile '"+_base_path+"/sql/codec.txt' IGNORE into table CodecPreset_ fields terminated by \"#\"");
      }

      void DatabaseService::stop() {
        LOGINFO("stopping Database Service");
        if (_running)
          mysql_library_end();
        _running = false;
      }

      void DatabaseService::thread_init() {
        boost::thread::id id=boost::this_thread::get_id();
        LOGDEBUG("void DatabaseService::thread_init():"<<id);
        if(_thread_map.find(id)==_thread_map.end()){
          _thread_map[id]=0;
        }
        _thread_map[id]++;
        if(_thread_map[id]==1){
          LOGDEBUG("mysql_thread_init()->thread_map count : "<<_thread_map[id]);
          mysql_thread_init();
        }
      }

      void DatabaseService::thread_end() {
          boost::thread::id id=boost::this_thread::get_id();
          LOGDEBUG("void DatabaseService::thread_end():"<<id)
          _thread_map[id]--;
        if(_thread_map[id]==0){
          LOGDEBUG("mysql_thread_end()->thread_map count : "<<_thread_map[id]);
          mysql_thread_end();
        }
      }
    }
  }
}
