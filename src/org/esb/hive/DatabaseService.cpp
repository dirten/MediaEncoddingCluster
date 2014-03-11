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
#include "DatabaseService.h"
//#include "config.h"
#include "org/esb/signal/Messenger.h"
#include "org/esb/util/Log.h"
#include "org/esb/util/Foreach.h"
#include "org/esb/util/StringTokenizer.h"
#include "org/esb/io/File.h"

#include "org/esb/config/config.h"
#include "org/esb/hive/Environment.h"

#include "HiveException.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/util/StringUtil.h"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "boost/lexical_cast.hpp"
#include <iostream>

namespace org {
  namespace esb {
    namespace hive {
      using org::esb::hive::Environment;
      bool DatabaseService::_running = false;
      std::string DatabaseService::_base_path;
      Ptr<org::esb::lang::Process> DatabaseService::_dbServer;
      std::map<boost::thread::id, int> DatabaseService::_thread_map;

      DatabaseService::DatabaseService(std::string base_path) {
        _base_path = base_path;
        _running = false;
      }

      void DatabaseService::onMessage(org::esb::signal::Message & msg) {
        if (msg.getProperty<std::string > ("databaseservice") == org::esb::hive::START) {
          start(_base_path);
        }
        if (msg.getProperty<std::string > ("databaseservice") == org::esb::hive::STOP) {
          stop();
        }
      }

      void DatabaseService::start(std::string base_path) {
        _base_path = org::esb::config::Config::get("mhive.base_path");
        //LOGINFO("starting Database Service");
        _running = true;
      }

      bool DatabaseService::databaseExist() {
        bool result = false;
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
        org::esb::io::File dbfile(database_name);
        if (dbfile.exists())
          result = true;
        return result;
      }

      void DatabaseService::createDatabase() {
        return;
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
        db::HiveDb db = getDatabase();
        db.query("CREATE DATABASE " + database_name);

      }

      void DatabaseService::createTables() {
        db::HiveDb db = getDatabase();
        LOGDEBUG("Create Tables");
        db.create();
      }

      void DatabaseService::updateTables() {
        db::HiveDb db = getDatabase();
        if (db.needsUpgrade()) {
          std::cout <<"Upgrade database"<<std::endl;
          //LOGDEBUG("Upgrade database");
          db.upgrade();
        }else{
          std::cout <<"no database upgrade"<<std::endl;
          //LOGDEBUG("no database upgrade");
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
        org::esb::io::File dbfile(database_name);
        if (dbfile.exists())
          dbfile.deleteFile();
        //        db::HiveDb db=getDatabase();
        //        db.query("DROP DATABASE " + database_name);
      }

      void DatabaseService::dropTables() {
        db::HiveDb db = getDatabase();
        db.drop();
      }

      void DatabaseService::loadPresets() {
        LOGDEBUG("LOAD PRESETS");
        _base_path = org::esb::config::Config::get("hive.base_path");
        db::HiveDb db = getDatabase();
        org::esb::io::File dir(_base_path + "/presets");
        org::esb::io::FileList presets = dir.listFiles();

        foreach(org::esb::io::FileList::value_type p, presets) {
          
          org::esb::io::FileInputStream fis(p.get());
          std::string data;
          fis.read(data);
          boost::uuids::uuid uuid = boost::uuids::random_generator()();
          std::string uuidstr = boost::lexical_cast<std::string > (uuid);
          db::Preset preset(db);
          preset.data = data;
          preset.uuid = uuidstr;
          preset.name = p->getFileName();
          preset.update();

        }
      }

      void DatabaseService::stop() {
        //LOGINFO("stopping Database Service");
        return;
        try {
          _dbServer->stop();
        } catch (...) {
        }
        //        if (_running)
        //          mysql_library_end();
        _running = false;
        org::esb::lang::Thread::sleep2(1000);
      }

      void DatabaseService::thread_init() {
        return;
        boost::thread::id id = boost::this_thread::get_id();
        LOGDEBUG("void DatabaseService::thread_init():" << id);
        if (_thread_map.find(id) == _thread_map.end()) {
          _thread_map[id] = 0;
        }
        _thread_map[id]++;
        if (_thread_map[id] == 1) {
          LOGDEBUG("mysql_thread_init()->thread_map count : " << _thread_map[id]);
          //          mysql_thread_init();
        }
      }

      void DatabaseService::thread_end() {
        return;
        boost::thread::id id = boost::this_thread::get_id();
        LOGDEBUG("void DatabaseService::thread_end():" << id)
        _thread_map[id]--;
        if (_thread_map[id] == 0) {
          LOGDEBUG("mysql_thread_end()->thread_map count : " << _thread_map[id]);
          //          mysql_thread_end();
        }
      }

      void DatabaseService::bootstrap() {
        return;
        std::string mysql_bin = org::esb::config::Config::get("MYSQLD_BIN");
        std::string mysql_data = org::esb::config::Config::get("MYSQL_DATA");
        std::string mysql_lang = org::esb::config::Config::get("MYSQL_LANG");
        std::string mysql_boot = org::esb::config::Config::get("MYSQL_BOOT");
        std::list<std::string> args;
        args.push_back(mysql_bin + " --verbose --bootstrap --datadir=" + mysql_data + " --language=" + mysql_lang + " < " + mysql_boot);
#ifdef __WIN32__
        args.push_front("/c");
        //        Process p1("C:\\Windows\\system32\\cmd.exe", args);
        Process p1("cmd.exe", args);
        //        Process p1("D:\\Windows\\system32\\cmd.exe", args);
#else
        args.push_front("-c");
        Process p1("/bin/sh", args);
#endif
        p1.start();
      }

      db::HiveDb & DatabaseService::getDatabase() {
        if (!_running) {
          start();
        }
        static db::HiveDb db=db::HiveDb(Environment::get(Environment::DB_TYPE), Environment::get(Environment::DB_URL));
        return db;
      }
    }
  }
}
