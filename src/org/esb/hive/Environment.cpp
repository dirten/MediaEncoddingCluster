/* 
 * File:   Environment.cpp
 * Author: HoelscJ
 * 
 * Created on 2. November 2011, 13:22
 */

#include "Environment.h"
#include "org/esb/io/File.h"
#include "org/esb/config/config.h"
#include "org/esb/util/Foreach.h"
#include "org/esb/util/UUID.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/libjson/libjson.h"
#include "DatabaseService.h"
namespace org {
  namespace esb {
    namespace hive {

      Environment::Environment() {
      }

      void Environment::build(int argc, char ** argv) {
        org::esb::io::File f(argv[0]);
        std::string bpath = org::esb::io::File(f.getParent()).getParent();

#ifdef __WIN32__
        std::string upath = config::Config::get("APPDATA") + "/mhive";
#elif defined __APPLE__
        std::string upath = config::Config::get("HOME") + "/.mhive";
#elif defined __LINUX__
        std::string upath = config::Config::get("HOME") + "/.mhive";
#else
        //#error "plattform not supported"
#endif

        config::Config::setProperty("hive.user_path", upath);
        config::Config::setProperty("web.docroot", bpath + "/web");
        config::Config::setProperty("hive.config_path", upath + "/conf");
        config::Config::setProperty("hive.dump_path", upath + "/dmp");
        config::Config::setProperty("hive.tmp_path", upath + "/tmp");
        config::Config::setProperty("hive.data_path", upath + "/data");
        config::Config::setProperty("preset.path", bpath + "/presets");
        config::Config::setProperty("log.path", upath + "/logs");
        config::Config::setProperty("db.url", "database=" + upath + "/data/hive.db");
#ifdef __WIN32__
        config::Config::setProperty("PATH", config::Config::get("PATH") + ";" + bpath + "/plugins");
#elif defined __APPLE__
        config::Config::setProperty("DYLD_LIBRARY_PATH", config::Config::get("DYLD_LIBRARY_PATH") + ":" + bpath + "/plugins");
#elif defined __LINUX__
        config::Config::setProperty("LD_LIBRARY_PATH", config::Config::get("LD_LIBRARY_PATH") + ":" + bpath + "/plugins");
#else
        //#error "plattform not supported"
#endif
        //LOGDEBUG("LIBRARY_PATH="<<config::Config::get("DYLD_LIBRARY_PATH"));
        /*
          std::string logpath=std::string("log.path=").append(bpath).append("/logs");
          char * pa=new char[logpath.length()+1];//const_cast<char*>(logpath.c_str());

          memset(pa,0,logpath.length()+1);
          memcpy(pa,logpath.c_str(),logpath.length());
          putenv(pa);*/
        //std::cout << "logpath"<<pa<<std::endl;
        //std::cout << "logpathenv" << getenv("log.path") << std::endl;
        //config::Config::setProperty("authentication", "true");
        org::esb::io::File u2path(config::Config::get("hive.user_path"));
        if (!u2path.exists())
          u2path.mkdir();

        org::esb::io::File dpath(config::Config::get("hive.dump_path"));
        if (!dpath.exists())
          dpath.mkdir();

        org::esb::io::File tpath(config::Config::get("hive.tmp_path"));
        if (!tpath.exists())
          tpath.mkdir();

        org::esb::io::File datadir(config::Config::get("hive.data_path"));
        if (!datadir.exists())
          datadir.mkdir();

        org::esb::io::File confdir(config::Config::get("hive.config_path"));
        if (!confdir.exists())
          confdir.mkdir();

        org::esb::io::File logdir(config::Config::get("log.path"));
        if (!logdir.exists())
          logdir.mkdir();


        org::esb::hive::DatabaseService::start(config::Config::getProperty("hive.base_path"));
        if (!DatabaseService::databaseExist()) {
          DatabaseService::createDatabase();
          DatabaseService::createTables();
          DatabaseService::updateTables();
          //DatabaseService::loadPresets();
          {
            db::HiveDb db = org::esb::hive::DatabaseService::getDatabase();
            org::esb::io::File dir(config::Config::get("hive.base_path") + "/presets");
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
            conf["hive.port"] = config::Config::getProperty("hive.port"); //StringUtil::toString(vm["hiveport"].as<int> ());
            conf["web.port"] = config::Config::getProperty("web.port"); //StringUtil::toString(vm["webport"].as<int> ());

            conf["hive.start"] = "true";
            conf["web.start"] = "true";
            conf["hive.autoscan"] = "true";
            conf["hive.scaninterval"] = "30";
            std::string webroot = std::string(config::Config::getProperty("hive.base_path"));
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
