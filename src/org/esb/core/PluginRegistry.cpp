/* 
 * File:   PluginRegistry.cpp
 * Author: HoelscJ
 * 
 * Created on 29. August 2011, 14:46
 */

#include "org/esb/db/hivedb.hpp"

#include "PluginRegistry.h"
#include "ServicePlugin.h"
#include "introspec.h"
#include "org/esb/core/HookPlugin.h"
#include "org/esb/util/Foreach.h"
#include "org/esb/util/StringUtil.h"
#include "org/esb/io/File.h"
#include "org/esb/config/config.h"
#include "org/esb/db/hivedb.hpp"
#include "PluginContext.h"
#include "Task.h"
//#include "org/esb/api/ApiWebServer.h"
namespace org {
  namespace esb {
    namespace core {

      bool compare_webservice(Ptr<org::esb::io::File> first, Ptr<org::esb::io::File> second) {
        //LOGDEBUG("Compare="<<first->getPath());
        if (first->getPath().find("webservice") != std::string::npos) {
          return true;
        } else {
          return false;
        }
      }
      PluginRegistry * PluginRegistry::_instance = NULL;

      PluginRegistry * PluginRegistry::getInstance() {
        if (_instance == NULL) {
          _instance = new PluginRegistry;
        }

        return _instance;
      }

      OptionsDescription PluginRegistry::getOptionsDescription(std::string name) {
        if (_plug_map.count(name) > 0) {
          return _plug_map[name]->getOptionsDescription();
        }
        return OptionsDescription();
      }

      std::list<std::string> PluginRegistry::getPluginNameList() {
        std::list<std::string> result;
        typedef std::map<std::string, Plugin*> PluginMap;

        foreach(PluginMap::value_type s, _plug_map) {
          result.push_back(s.first);
        }
        return result;
      }

      void PluginRegistry::registerPlugin(std::string name, Plugin*plugin) {
      }

      void PluginRegistry::registerService(std::string name, ServicePlugin*plugin) {
        if (plugin == NULL)return;
        _service_map[name] = plugin;
        _plug_map[name] = plugin;
        //OptionsDescription desc = plugin->getOptionsDescription();
      }

      void CORE_EXPORT PluginRegistry::startServerServices() {
        typedef std::map<std::string, Plugin*> PluginMap;

        foreach(PluginMap::value_type s, _service_map) {
          //LOGDEBUG("ServiceName="<<s.first<<" type="<<((ServicePlugin*) s.second)->getServiceType());
          if (((ServicePlugin*) s.second)->getServiceType() == ServicePlugin::SERVICE_TYPE_SERVER ||
                  ((ServicePlugin*) s.second)->getServiceType() == ServicePlugin::SERVICE_TYPE_ALL)
            ((ServicePlugin*) s.second)->startService();
        }
      }

      void CORE_EXPORT PluginRegistry::startClientServices() {
        typedef std::map<std::string, Plugin*> PluginMap;

        foreach(PluginMap::value_type s, _service_map) {
          if (((ServicePlugin*) s.second)->getServiceType() == ServicePlugin::SERVICE_TYPE_CLIENT ||
                  ((ServicePlugin*) s.second)->getServiceType() == ServicePlugin::SERVICE_TYPE_ALL)
            ((ServicePlugin*) s.second)->startService();
        }

      }

      void CORE_EXPORT PluginRegistry::stopServices() {
        typedef std::map<std::string, Plugin*> PluginMap;

        foreach(PluginMap::value_type s, _service_map) {
          ((ServicePlugin*) s.second)->stopService();
        }
      }

      void PluginRegistry::registerHookPlugin(std::string name, HookPlugin*plugin) {
        LOGDEBUG("register HookPlugin " << name);
        _plug_map[name] = plugin;
      }

      void PluginRegistry::registerHookProvider(std::string name, HookProvider*plugin) {
        _hook_provider_map[name] = plugin;
      }

      void PluginRegistry::registerTaskFactory(std::string name, TaskFactory *factory) {
        _task_factories[name] = factory;
      }
      /*
      void PluginRegistry::registerTaskFactory(std::string name, Ptr<TaskFactory> factory) {
        _task_factories[name] = factory;
      }*/

      void PluginRegistry::close() {
        delete _instance;

      }

      PluginRegistry::PluginRegistry() {
      }

      Ptr<Task>PluginRegistry::createTask(std::string name,std::string cfg) {
        Ptr<Task>result=_task_factories[name]->create();
        result->setContext(new PluginContext());
        org::esb::util::StringTokenizer tok(cfg,";");
        while(tok.hasMoreTokens()){
          std::string line=tok.nextToken();
          org::esb::util::StringTokenizer tok2(line,"=");
          if(tok2.countTokens()==2){
            std::string key=tok2.nextToken();
            std::string val=tok2.nextToken();
            LOGDEBUG("Setting plugin Context : "<<key<<"="<<val);
            result->getContext()->env[key]=val;
          }else{
            LOGERROR("line : "<<line);
          }
        }        
        return result;
      }

      void PluginRegistry::initPlugins() {
        typedef std::map<std::string, Plugin*> PluginMap;

        foreach(PluginMap::value_type s, _plug_map) {
          LOGDEBUG("Initialize Plugin:" << s.first);
          _plugin_data[s.first].name = s.first;
          _plugin_data[s.first].context = new PluginContext();
          _plugin_data[s.first].plugin = s.second;
          _plugin_data[s.first].context->database = new db::HiveDb("sqlite3", org::esb::config::Config::get("db.url"));

          /*fill up PluginContext with Options*/
          OptionsDescription desc = s.second->getOptionsDescription();
          typedef boost::shared_ptr<boost::program_options::option_description> option;

          foreach(const option value, desc.options()) {
            boost::any data;
            value->semantic()->apply_default(data);
            std::string def;
            if (data.type() == typeid (int)) {
              def = org::esb::util::StringUtil::toString(boost::any_cast<int>(data));
            } else if (data.type() == typeid (double)) {
              def = org::esb::util::StringUtil::toString(boost::any_cast<double>(data));
            } else if (data.type() == typeid (bool)) {
              def = org::esb::util::StringUtil::toString(boost::any_cast<bool>(data));
            } else {
              def = org::esb::util::StringUtil::toString(boost::any_cast<std::string > (data));
            }
            LOGDEBUG("Key=" << value->long_name() << " Default=" << def << " Value=" << org::esb::config::Config::get(value->long_name()));
            _plugin_data[s.first].context->env[value->long_name()] = org::esb::config::Config::get(value->long_name());
            _config_data[value->long_name()] = org::esb::config::Config::get(value->long_name());
            if (org::esb::config::Config::get(value->long_name()) == def) {
              litesql::DataSource<db::Config> confs = litesql::select<db::Config > (*_plugin_data[s.first].context->database, db::Config::Configkey == value->long_name());
              if (confs.count() > 0) {
                db::Config c = confs.one();
                if (c.configval.value().length() > 0) {
                  _plugin_data[s.first].context->env[value->long_name()] = c.configval.value();
                  _config_data[value->long_name()] = c.configval.value();
                  LOGDEBUG("Load from db Key=" << value->long_name() << "Default=" << def << " Value=" << c.configval.value());
                }
              }
            }
          }
          s.second->setContext(_plugin_data[s.first].context);
          s.second->init();
        }
      }

      std::string PluginRegistry::getConfigData(std::string key) {
        std::string result;
        if (_config_data.count(key) > 0) {
          result = _config_data[key];
        }
        return result;
      }

      void PluginRegistry::load(std::string file) {
        org::esb::io::File plugin_dir(file);

        if (plugin_dir.isDirectory()) {

          org::esb::io::FileList plugin_list = plugin_dir.listFiles();
          plugin_list.sort(compare_webservice);

          foreach(Ptr<org::esb::io::File> f, plugin_list) {
            if (f->isFile())
              load(f->getPath());
          }
        } else if (plugin_dir.isFile()) {

          loadFile(plugin_dir.getPath());
        }

      }

      void PluginRegistry::loadFile(std::string file) {
        LOGDEBUG("loading plugins from " << file);
        //std::cout<<"loading plugins from "<<file<<std::endl;

        try {
          org::esb::lang::SharedObjectLoader * loader = new org::esb::lang::SharedObjectLoader(file);
          _shared_objects[file] = loader;
        } catch (org::esb::lang::NotFoundException & ex) {

        }
      }

      PluginRegistry::~PluginRegistry() {

        LOGDEBUG("PluginRegistry::~PluginRegistry()");

        typedef std::map<std::string, org::esb::lang::SharedObjectLoader*> SharedObjectMap;

        typedef std::map<std::string, PluginData> PluginDataMap;

        foreach(PluginDataMap::value_type s, _plugin_data) {

          delete s.second.context->database;
          delete s.second.context;
        }

        foreach(SharedObjectMap::value_type row, _shared_objects) {
          delete row.second;
        }
      }
    }
    namespace api {
      //    REGISTER_SERVICE("apiwebserver", ApiWebServer)
    }
  }
}
