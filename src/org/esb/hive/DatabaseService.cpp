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
#include "config.h"
#include "org/esb/signal/Messenger.h"
#include "org/esb/util/Log.h"
#include "org/esb/util/StringTokenizer.h"
#include "org/esb/io/File.h"

#include "org/esb/config/config.h"
#include "HiveException.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/util/StringUtil.h"
#include <iostream>
#include "org/esb/hive/PresetLoader.h"
namespace org {
  namespace esb {
    namespace hive {
      bool DatabaseService::_running = false;
      std::string DatabaseService::_base_path;
      Ptr<org::esb::lang::Process> DatabaseService::_dbServer;
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
        _base_path=org::esb::config::Config::get("mhive.base_path");
        LOGINFO("starting Database Service");
        _running=true;
      }
      bool DatabaseService::databaseExist() {
        bool result=false;
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
        if(dbfile.exists())
          result=true;
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
        db::HiveDb db=getDatabase();
        db.query("CREATE DATABASE " + database_name);

      }

      void DatabaseService::createTables() {
        db::HiveDb db=getDatabase();
        LOGDEBUG("Create Tables");
        db.create();
      }

      void DatabaseService::updateTables() {
        db::HiveDb db=getDatabase();
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
        org::esb::io::File dbfile(database_name);
        if(dbfile.exists())
          dbfile.deleteFile();
//        db::HiveDb db=getDatabase();
//        db.query("DROP DATABASE " + database_name);
      }

      void DatabaseService::dropTables() {
        db::HiveDb db=getDatabase();
        db.drop();
      }

      void DatabaseService::loadPresets() {
        LOGDEBUG("LOAD PRESETS");
        _base_path=org::esb::config::Config::get("hive.base_path");
        db::HiveDb db=getDatabase();

        if(true){
//          db.query("DELETE FROM Config_ ");
          org::esb::io::FileInputStream fis(_base_path+"/sql/config.txt");
          std::string lines;
          fis.read(lines);
          org::esb::util::StringTokenizer t(lines,"\n");
          while(t.hasMoreTokens()){
            std::string line=t.nextToken();
            std::string sql="INSERT OR REPLACE INTO Config_ values (";
            LOGDEBUG(line);
            sql+=org::esb::util::StringUtil::replace(line,":",",");
            sql+=")";
            LOGDEBUG(sql);
            db.query(sql);
          }
        }
        if(false){
//          db.query("DELETE FROM Profile_ ");
          org::esb::io::FileInputStream fis(_base_path+"/sql/profiles.txt");
          std::string lines;
          fis.read(lines);
          org::esb::util::StringTokenizer t(lines,"\n");
          while(t.hasMoreTokens()){
            std::string line=t.nextToken();
            std::string sql="INSERT OR REPLACE INTO Profile_ values (";
            LOGDEBUG(line);
            sql+=org::esb::util::StringUtil::replace(line,":",",");
            sql+=")";
            LOGDEBUG(sql);
            db.query(sql);
          }
        }
        if(true){
          {
            org::esb::hive::PresetReader reader(_base_path+"/res/presets/x264-hq.preset");
            LOGDEBUG(reader.toString());
            org::esb::hive::PresetLoader loader(reader);
            loader.load();
          }
          {
            org::esb::hive::PresetReader reader(_base_path+"/res/presets/x264-ipod320.preset");
            LOGDEBUG(reader.toString());
            org::esb::hive::PresetLoader loader(reader);
            loader.load();
          }
          {
            org::esb::hive::PresetReader reader(_base_path+"/res/presets/x264-ipod640.preset");
            LOGDEBUG(reader.toString());
            org::esb::hive::PresetLoader loader(reader);
            loader.load();
          }
        }

        if(true){
          org::esb::io::FileInputStream fis(_base_path+"/sql/codec.txt");
          std::string lines;
          fis.read(lines);
          org::esb::util::StringTokenizer t(lines,"\n");
          while(t.hasMoreTokens()){
            std::string line=t.nextToken();
            std::string sql="INSERT OR REPLACE INTO CodecPreset_ values (";
            LOGDEBUG(line);
            sql+=org::esb::util::StringUtil::replace(line,"#",",");
            sql+=")";
            LOGDEBUG(sql);
            db.query(sql);
          }
        }


        return;
        db::ProfileGroup g(db);
        g.name="root";
        g.update();

        db::ProfileGroup avi(db);
        avi.name="Avi";
        avi.update();

        g.childrens().link(avi);

        /**
         * linking Profiles to ProfileGroup
         */
        avi.profiles().link(litesql::select<db::Profile>(db,db::Profile::Id==5).one());
        avi.profiles().link(litesql::select<db::Profile>(db,db::Profile::Id==6).one());

        db::ProfileGroup mpeg(db);
        mpeg.name="MPeg";
        mpeg.update();
        g.childrens().link(mpeg);

        db::ProfileGroup mp4(db);
        mp4.name="MP4";
        mp4.update();
        g.childrens().link(mp4);
        mp4.profiles().link(litesql::select<db::Profile>(db,db::Profile::Id==1).one());
        mp4.profiles().link(litesql::select<db::Profile>(db,db::Profile::Id==2).one());
        mp4.profiles().link(litesql::select<db::Profile>(db,db::Profile::Id==3).one());
        
        db::ProfileGroup web(db);
        web.name="Web";
        web.update();
        g.childrens().link(web);
        
        db::ProfileGroup flash(db);
        flash.name="Flash";
        flash.update();
        web.childrens().link(flash);


      }

      void DatabaseService::stop() {
        return;
        LOGINFO("stopping Database Service");
        try{
        _dbServer->stop();
        }catch(...){}
//        if (_running)
//          mysql_library_end();
        _running = false;
        org::esb::lang::Thread::sleep2(1000);
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
//          mysql_thread_init();
        }
      }

      void DatabaseService::thread_end() {
          boost::thread::id id=boost::this_thread::get_id();
          LOGDEBUG("void DatabaseService::thread_end():"<<id)
          _thread_map[id]--;
        if(_thread_map[id]==0){
          LOGDEBUG("mysql_thread_end()->thread_map count : "<<_thread_map[id]);
//          mysql_thread_end();
        }
      }
       void DatabaseService::bootstrap() {
         return;
        std::string mysql_bin=org::esb::config::Config::get("MYSQLD_BIN");
        std::string mysql_data=org::esb::config::Config::get("MYSQL_DATA");
        std::string mysql_lang=org::esb::config::Config::get("MYSQL_LANG");
        std::string mysql_boot=org::esb::config::Config::get("MYSQL_BOOT");
        std::list<std::string> args;
        args.push_back(mysql_bin+" --verbose --bootstrap --datadir="+mysql_data+" --language="+mysql_lang+" < "+mysql_boot);
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
      db::HiveDb DatabaseService::getDatabase() {
        if(!_running){
          start();
        }
        return db::HiveDb(DEFAULT_DATABASE, org::esb::config::Config::getProperty("db.url"));
      }
      
    }
  }
}
