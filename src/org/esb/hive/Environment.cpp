/* 
 * File:   Environment.cpp
 * Author: HoelscJ
 * 
 * Created on 2. November 2011, 13:22
 */

#include "DatabaseService.h"
#include "Environment.h"
#include "org/esb/io/File.h"
#include "org/esb/config/config.h"
#include "org/esb/util/Foreach.h"
#include "org/esb/util/UUID.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/libjson/libjson.h"

namespace org {
  namespace esb {
    namespace hive {
      std::map<std::string, std::string> Environment::_environmentMap;

      Environment::Environment() {
      }

      std::string Environment::get(std::string key, std::string def){
        if(_environmentMap.find(key)==_environmentMap.end()){
          return getenv(key.c_str())!=NULL?getenv(key.c_str()):def;
        }else{
          return _environmentMap[key];
        }
      }

      void Environment::set(std::string key, std::string value){
        _environmentMap[key]=value;
        config::Config::setProperty(key, value);
      }

      void Environment::build(int argc, char ** argv) {
        org::esb::io::File f(argv[0]);
        std::string bpath = org::esb::io::File(f.getParent()).getParent();
        
#ifdef __WIN32__
        std::string upath = get("APPDATA") + "/mhive";
#elif defined __APPLE__
        std::string upath = get("HOME") + "/.mhive";
#elif defined __LINUX__
        std::string upath = get("HOME") + "/.mhive";
#else
        #error "plattform not supported"
#endif
        /*override the user path when this environment variable is set*/
        upath = get("MHIVE_DATA_PATH", upath);

        set("hive.user_path", upath);
        set("hive.exec_path", f.getFilePath());
        set("web.docroot", bpath + "/web");
        set("hive.config_path", upath + "/conf");
        set("hive.dump_path", upath + "/dmp");
        set("hive.tmp_path", upath + "/tmp");
        set("hive.data_path", upath + "/data");
        set("preset.path", bpath + "/presets");
        set("log.path", upath + "/logs");
        set("db.url", "database=" + upath + "/data/hive.db");
#ifdef __WIN32__
        set("PATH", get("PATH") + ";" + bpath + "/plugins");
#elif defined __APPLE__
        set("DYLD_LIBRARY_PATH", get("DYLD_LIBRARY_PATH") + ":" + bpath + "/plugins");
#elif defined __LINUX__
        set("LD_LIBRARY_PATH", get("LD_LIBRARY_PATH") + ":" + bpath + "/plugins");
#else
        #error "plattform not supported"
#endif

        org::esb::io::File u2path(get("hive.user_path"));
        if (!u2path.exists())
          u2path.mkdir();

        org::esb::io::File dpath(get("hive.dump_path"));
        if (!dpath.exists())
          dpath.mkdir();

        org::esb::io::File tpath(get("hive.tmp_path"));
        if (!tpath.exists())
          tpath.mkdir();

        org::esb::io::File datadir(get("hive.data_path"));
        if (!datadir.exists())
          datadir.mkdir();

        org::esb::io::File confdir(get("hive.config_path"));
        if (!confdir.exists())
          confdir.mkdir();

        org::esb::io::File logdir(get("log.path"));
        if (!logdir.exists())
          logdir.mkdir();


        org::esb::hive::DatabaseService::start(get("hive.base_path"));
        if (!DatabaseService::databaseExist()) {
          DatabaseService::createDatabase();
          DatabaseService::createTables();
          DatabaseService::updateTables();
          //DatabaseService::loadPresets();
          {
            db::HiveDb db = org::esb::hive::DatabaseService::getDatabase();
            org::esb::io::File dir(get("hive.base_path") + "/presets");
            org::esb::io::FileList presets = dir.listFiles();

            foreach(org::esb::io::FileList::value_type p, presets) {
              org::esb::io::FileInputStream fis(p.get());
              std::string data;
              fis.read(data);
              try {
                JSONNode d = libjson::parse(data);
                db::Preset preset(db);
                preset.data = data;
                preset.uuid = (std::string)org::esb::util::PUUID();
                preset.name = d["name"].as_string();
                preset.update();
              } catch (...) {
              }
            }

            std::map<std::string, std::string> conf;
            conf["hive.mode"] = "server";
            conf["hive.port"] = get("hive.port"); //StringUtil::toString(vm["hiveport"].as<int> ());
            conf["web.port"] = get("web.port"); //StringUtil::toString(vm["webport"].as<int> ());

            conf["hive.start"] = "true";
            conf["web.start"] = "true";
            conf["hive.autoscan"] = "true";
            conf["hive.scaninterval"] = "30";
            std::string webroot = std::string(get("hive.base_path"));
            webroot.append("/web");
            conf["web.docroot"] = webroot;

            std::map<std::string, std::string>::iterator it = conf.begin();
            db.begin();
            for (; it != conf.end(); it++) {
              db::Config cfg(db);
              cfg.configkey = it->first;
              cfg.configval = it->second;
              cfg.update();
              LOGDEBUG("key=" << it->first << " val=" << it->second);
            }
            db.commit();
          }
        } else {
          DatabaseService::updateTables();
        }
        /**stopping the internal database service*/
        org::esb::hive::DatabaseService::stop();

      }

      Environment::~Environment() {
      }
    }
  }
}
