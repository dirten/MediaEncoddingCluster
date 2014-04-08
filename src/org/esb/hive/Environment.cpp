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
#include "org/esb/util/StringUtil.h"

using org::esb::util::StringUtil;

namespace org {
  namespace esb {
    namespace hive {
      std::map<std::string, std::string> Environment::_environmentMap;
      std::vector<std::string> Environment::_argumentMap;
      std::map<std::string, boost::any>  Environment::_env;

      const std::string Environment::BASE_PATH="hive.base_path";
      const std::string Environment::USER_HOME="hive.user_path";
      const std::string Environment::PLUGIN_PATH="hive.plugin_path";
      const std::string Environment::EXE_PATH="hive.exec_path";
      const std::string Environment::EXE_NAME="hive.exec_name";
      const std::string Environment::SYSTEM="hive.arch";
      const std::string Environment::DB_URL="db.url";
      const std::string Environment::DB_TYPE="db.type";


      Environment::Environment() {
      }

      void Environment::close() {
        _environmentMap.clear();
        _argumentMap.clear();
        org::esb::config::Config::close();
      }

      std::vector<std::string> Environment::getArguments(){
        return _argumentMap;
      }

      bool Environment::contains(string key)
      {
        if(_env.count(key)==0){
          return false;
        }
        return true;
      }

      boost::any Environment::getAnyType(std::string key) {
        if(_env.count(key)==0){
          return boost::any();
        }else{
          return _env[key];
        }
      }

      std::string Environment::get(std::string key, std::string def){
        return get<std::string>(key, def);
      }

      void Environment::set(std::string key, const char * value){
        set(key, std::string(value));
      }

      void Environment::set(std::string key, boost::any value){
        _env[key]=value;
        if (value.type() == typeid (int)) {
          config::Config::setProperty(key, StringUtil::toString(boost::any_cast< int >(value)));
        } else if (value.type() == typeid (double)) {
          config::Config::setProperty(key, StringUtil::toString(boost::any_cast< double >(value)));
        } else if (value.type() == typeid (bool)) {
          config::Config::setProperty(key, StringUtil::toString(boost::any_cast< bool >(value)));
        } else if (value.type() == typeid (std::vector<std::string>)) {
          /*NOP, Config class could not handle list properties and will be removed in the near future*/
        } else {
          config::Config::setProperty(key, boost::any_cast< std::string >(value));
        }
      }

      void Environment::build(int argc, char ** argv) {

        for(int a = 0;a<argc;a++){
          std::string arg=argv[a];
          _argumentMap.push_back(arg);
        }
        /*building the absolute path to the executable*/
        org::esb::io::File ftmp(argv[0]);
        chdir(ftmp.getFilePath().c_str());
        char buf[1000];
        getcwd(buf, 1000);

        org::esb::io::File f(buf);
        //std::string bpath = org::esb::io::File(f.getParent()).getParent();
        std::string bpath = f.getParent();
        std::cout << "bpath:"<<bpath<<std::endl;
        //std::cout << "bpath2:"<<bpath2<<std::endl;
        std::cout << "cwd:"<<buf<<std::endl;
#ifdef __WIN32__
        std::string upath = get("APPDATA") + "/mhive";
        set(SYSTEM, "win");
#elif defined __APPLE__
        std::string upath = get("HOME") + "/.mhive";
        set(SYSTEM, "Darwin");
#elif defined __LINUX__
        std::string upath = get("HOME") + "/.mhive";
        set(SYSTEM, std::string("Linux"));
#else
#error "plattform not supported"
#endif

        /*override the user path when this environment variable is set*/
        upath = get("MHIVE_DATA_PATH", upath);

        set(USER_HOME, upath);

        set(BASE_PATH, bpath);
        set(PLUGIN_PATH, get("MHIVE_PLUGIN_DIR", bpath+"/plugin" ));

        set(EXE_PATH, f.getFilePath());
        set(EXE_NAME, f.getFileName());

        set(DB_URL, "database=" + upath + "/data/hive.db");
        set(DB_TYPE, std::string("sqlite3"));

        set("web.docroot", bpath + "/web");
        set("hive.config_path", upath + "/conf");
        set("hive.update_path", upath + "/update");
        set("hive.dump_path", upath + "/dmp");
        set("hive.tmp_path", upath + "/tmp");
        set("hive.data_path", upath + "/data");
        set("preset.path", bpath + "/presets");
        set("log.path", upath + "/logs");
#ifdef __WIN32__
        set("PATH", get("PATH") + ";" + bpath + "/plugins");
        set("PATH", get("PATH") + ";" + bpath + "/lib");
#elif defined __APPLE__
        set("DYLD_LIBRARY_PATH", get("DYLD_LIBRARY_PATH") + ":" + bpath + "/plugins");
        set("DYLD_LIBRARY_PATH", get("DYLD_LIBRARY_PATH") + ":" + bpath + "/lib");
#elif defined __LINUX__
        set("LD_LIBRARY_PATH", get("LD_LIBRARY_PATH") + ":" + bpath + "/plugins");
        set("LD_LIBRARY_PATH", get("LD_LIBRARY_PATH") + ":" + bpath + "/lib");
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

        org::esb::io::File updatedir(get("hive.update_path"));
        if (!updatedir.exists())
          updatedir.mkdir();

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
        //org::esb::hive::DatabaseService::stop();

      }

      Environment::~Environment() {
      }
      }
    }
  }
