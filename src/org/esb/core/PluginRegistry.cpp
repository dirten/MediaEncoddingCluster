/* 
 * File:   PluginRegistry.cpp
 * Author: HoelscJ
 *
 * Created on 29. August 2011, 14:46
 */

#include "org/esb/db/hivedb.hpp"
#include "org/esb/grid/GridRegistry.h"

#include "PluginRegistry.h"
#include "ServicePlugin.h"
#include "StorageEngine.h"
#include "org/esb/lang/Introspec.h"
#include "org/esb/core/HookPlugin.h"
#include "org/esb/util/Foreach.h"
#include "org/esb/util/StringUtil.h"
#include "org/esb/io/File.h"
#include "org/esb/config/config.h"
#include "org/esb/db/hivedb.hpp"
#include "PluginContext.h"
#include "Task.h"
#include "Poco/Net/HTTPStreamFactory.h"
#include "Poco/Net/FTPStreamFactory.h"
#include "WebHookHandlerFactory.h"
#include "org/esb/hive/Environment.h"

#include <string.h>
//#include "org/esb/api/ApiWebServer.h"


namespace org {
  namespace esb {
    namespace core {
      using org::esb::hive::Environment;
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

      org::esb::core::OptionsDescription PluginRegistry::getOptionsDescription()
      {
        org::esb::core::OptionsDescription result("registry");
        std::string storage_engines="";
        typedef std::map<std::string, StorageEngine*> PluginMap;

        foreach(PluginMap::value_type s, _storage_map) {
          storage_engines+=s.first+", ";
        }
        storage_engines="which storage engine to use("+storage_engines+")";
        result.add_options()
            ("storage.engine", boost::program_options::value<string >()->default_value("sqlite3"), storage_engines.c_str())
            //("storage.hosts", boost::program_options::value< std::vector<string> >()->multitoken(), "ip:port tuples for the other redundant storage")
            //("storage.port", boost::program_options::value< int >()->default_value(20202), "port number for own redundant storage")
            ;
        return result;
      }


      std::list<std::string> PluginRegistry::getPluginNameList() {
        std::list<std::string> result;
        typedef std::map<std::string, Plugin*> PluginMap;
        foreach(PluginMap::value_type s, _plug_map) {
          result.push_back(s.first);
        }
        result.push_back("registry");
        return result;
      }

      void PluginRegistry::registerPlugin(std::string name, Plugin*plugin) {
        /*NOP*/
      }

      void PluginRegistry::registerService(std::string name, ServicePlugin*plugin) {
        if (plugin == NULL)return;
        _service_map[name] = plugin;
        _plug_map[name] = plugin;
      }

      void PluginRegistry::registerStorage(std::string name, StorageEngine*storage){
        _storage_map[name]=storage;
        _plug_map[name] = storage;
      }

      void PluginRegistry::startStorageByName(std::string name)
      {
        typedef std::map<std::string, StorageEngine*> PluginMap;

        foreach(PluginMap::value_type s, _storage_map) {
          if (s.first==name){
            s.second->startup();
            break;
          }
        }
      }

      void PluginRegistry::stopStorageByName(std::string name)
      {
        typedef std::map<std::string, StorageEngine*> PluginMap;
        foreach(PluginMap::value_type s, _storage_map) {
          if (s.first==name){
            s.second->shutdown();
            break;
          }
        }
      }


      void PluginRegistry::startServerServices() {
        startWebService();
        typedef std::map<std::string, Plugin*> PluginMap;

        foreach(PluginMap::value_type s, _service_map) {
          if (
              ((ServicePlugin*) s.second)->getServiceType() == ServicePlugin::SERVICE_TYPE_SERVER ||
              ((ServicePlugin*) s.second)->getServiceType() == ServicePlugin::SERVICE_TYPE_ALL){
            startServiceByName(s.first);
          }
        }
      }

      void PluginRegistry::startClientServices() {
        typedef std::map<std::string, Plugin*> PluginMap;

        foreach(PluginMap::value_type s, _service_map) {
          if (
              ((ServicePlugin*) s.second)->getServiceType() == ServicePlugin::SERVICE_TYPE_CLIENT ||
              ((ServicePlugin*) s.second)->getServiceType() == ServicePlugin::SERVICE_TYPE_ALL){
            startServiceByName(s.first);
          }
        }
      }

      void PluginRegistry::startWebService(){
        server=new org::esb::core::http::Server(atoi(Environment::get("webport").c_str()));
        server->setRequestHandlerFactory(_webhook_handler_factory);
        server->start();
      }

      void PluginRegistry::startServiceByName(std::string name){
        bool found=false;
        typedef std::map<std::string, Plugin*> PluginMap;

        foreach(PluginMap::value_type s, _service_map) {
          if (s.first== name){
            ((ServicePlugin*) s.second)->startService();
            found=true;
            _running_plugins.push_back(name);
            break;
          }
        }
        if(!found){
          LOGERROR("pluging with name "<<name<<" not found");
        }
      }

      void PluginRegistry::stopServiceByName(std::string name){
        bool found=false;
        typedef std::map<std::string, Plugin*> PluginMap;

        foreach(PluginMap::value_type s, _service_map) {
          if (s.first== name){
            ((ServicePlugin*) s.second)->stopService();
            found=true;
            _running_plugins.remove(name);
            break;
          }
        }
        if(!found){
          LOGERROR("pluging with name "<<name<<" not found");
        }
      }

      bool PluginRegistry::isRunning(std::string plugin_name){
        bool result=false;
        foreach(std::string name, _running_plugins){
          if(name==plugin_name){
            result=true;
            break;
          }
        }
        return result;
      }

      void PluginRegistry::stopServices() {
        typedef std::map<std::string, Plugin*> PluginMap;

        foreach(PluginMap::value_type s, _service_map) {
          stopServiceByName(s.first);
        }
        if(server)
          server->stop();
        delete server;
        server=NULL;
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

      void PluginRegistry::registerWebHookFactory(std::string name, WebHookFactory *factory) {
        LOGDEBUG("register WebHookPlugin " << name);
        _webhook_handler_factory->registerHandlerFactory(factory);
      }

      std::list<std::string> PluginRegistry::getTaskNameList() {
        std::list<std::string> result;
        foreach(TaskFactoryMap::value_type s, _task_factories) {
          result.push_back(s.first);
        }
        return result;
      }

      OptionsDescription PluginRegistry::getTaskOptionsDescription(std::string name) {
        if (_task_factories.count(name) > 0) {
          return _task_factories[name]->getOptionsDescription();
        }
        return OptionsDescription();
      }

      /*
      void PluginRegistry::registerTaskFactory(std::string name, Ptr<TaskFactory> factory) {
        _task_factories[name] = factory;
      }*/

      void PluginRegistry::close() {
        delete _instance;
      }

      PluginRegistry::PluginRegistry() {
        Poco::Net::HTTPStreamFactory::registerFactory();
        Poco::Net::FTPStreamFactory::registerFactory();
        server=NULL;
        _webhook_handler_factory=new WebHookHandlerFactory();

        /*
         * this is not the right place for initialization
         * this generates unnecessary dependencies to the grid library
         */
        org::esb::grid::GridRegistry::init();
      }

      Ptr<Task>PluginRegistry::createTask(std::string name, std::map<std::string, std::string> cfg) {
        Ptr<Task>result;
        if (_task_factories.count(name) > 0) {
          result = _task_factories[name]->create();
          result->setContext(new PluginContext(getStorageEngine()));
          
          /*fill up PluginContext with default Options*/
          OptionsDescription desc = result->getOptionsDescription();
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
            } else if (data.type() == typeid (std::string)){
              def = org::esb::util::StringUtil::toString(boost::any_cast<std::string > (data));
            }
            result->getContext()->env[value->long_name()] = def;
            result->getContext()->setProperty<std::string>(value->long_name(), def);
          }

          typedef std::map<std::string, std::string> stringmap;
          foreach(stringmap::value_type config, cfg) {
            result->getContext()->env[config.first] = config.second;
            result->getContext()->setProperty<std::string>(config.first, config.second);
            LOGDEBUG("Setting plugin Context : " << config.first << "=" << config.second);
          }
        }
        return result;
      }

      void PluginRegistry::initPluginContext(Plugin*plugin, PluginContext*context)
      {
        OptionsDescription desc=plugin->getOptionsDescription();
        typedef boost::shared_ptr<boost::program_options::option_description> option;
        foreach(const option value, desc.options()) {

          /*loading default data into the context*/
          boost::any data;
          value->semantic()->apply_default(data);
          context->setProperty(value->long_name(), data);

          /*overwrite option data with specific data from command option*/
          if(Environment::contains(value->long_name())){
            context->setProperty(value->long_name(), Environment::getAnyType(value->long_name()));
          }

        }
      }

      void PluginRegistry::initPlugins() {
        typedef std::map<std::string, Plugin*> PluginMap;



        foreach(PluginMap::value_type s, _plug_map) {
          LOGDEBUG("Initialize Plugin:" << s.first);
          _plugin_data[s.first].name = s.first;
          _plugin_data[s.first].context = new PluginContext(getStorageEngine());
          _plugin_data[s.first].plugin = s.second;

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
            } else if (data.type() == typeid (std::string)) {
              def = org::esb::util::StringUtil::toString(boost::any_cast<std::string > (data));
            }
            LOGDEBUG("Key=" << value->long_name() << " Default=" << def << " Value=" << org::esb::config::Config::get(value->long_name()));
            _plugin_data[s.first].context->env[value->long_name()] = org::esb::config::Config::get(value->long_name());
            if(org::esb::hive::Environment::_env.count(value->long_name())){
              _plugin_data[s.first].context->_props[value->long_name()] = org::esb::hive::Environment::getAnyType(value->long_name());
              LOGDEBUG("Setting environment for key "<<value->long_name());
            }
            _config_data[value->long_name()] = org::esb::config::Config::get(value->long_name());
            if (org::esb::config::Config::get(value->long_name()) == def) {
              litesql::DataSource<db::Config> confs = litesql::select<db::Config > (_plugin_data[s.first].context->database, db::Config::Configkey == value->long_name());
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
        LOGDEBUG("loading plugin from directory" << file);
        org::esb::io::File plugin_dir(file);
        if(plugin_dir.isDirectory()) {
          org::esb::io::FileList plugin_list = plugin_dir.listFiles();
          foreach(Ptr<org::esb::io::File> f, plugin_list) {
            load(f->getPath());
          }
        }else{
          loadFile(file);
        }
      }

      StorageEngine * PluginRegistry::getStorageEngine(){
        static StorageEngine * engine=NULL;
        if(engine==NULL){
          std::string storage_name=org::esb::hive::Environment::get<std::string>("storage.engine");
          std::cout << "configure storage engine:"<<storage_name<<std::endl;
          typedef std::map<std::string, StorageEngine*> PluginMap;

          foreach(PluginMap::value_type s, _storage_map) {
            if (s.first==storage_name){
              engine=s.second;
              PluginContext * context=new PluginContext();
              engine->setContext(context);
              initPluginContext(engine, context);
              engine->startup();
              break;
            }
          }
        }
        return engine;
      }

      /**
        * Loading the plugin from this path, plugins are self registering types through the following macro's
        * REGISTER_WEB_HOOK(url,method,clazz)
        * REGISTER_TASK(name,instance)
        * REGISTER_SERVICE(name,type)
        * REGISTER_HOOK(name,instance, function, prio) --- this is currently not fully implemented
        */
      void PluginRegistry::loadFile(std::string file) {
        if(strstr(file.c_str(),".dylib")>0||strstr(file.c_str(),".dll")>0||strstr(file.c_str(),".so")>0){
          LOGDEBUG("loading plugin" << file);
          try {
            org::esb::lang::SharedObjectLoader * loader = new org::esb::lang::SharedObjectLoader(file);
            _shared_objects[file] = loader;
          } catch (org::esb::lang::NotFoundException & ex) {
            LOGERROR("failed loading plugin:"<<file);
            LOGERROR(ex.what());
          }
        }
      }

      PluginRegistry::~PluginRegistry() {

        LOGDEBUG("PluginRegistry::~PluginRegistry()");

        typedef std::map<std::string, org::esb::lang::SharedObjectLoader*> SharedObjectMap;

        typedef std::map<std::string, PluginData> PluginDataMap;

        foreach(PluginDataMap::value_type s, _plugin_data) {

          //delete s.second.context->database;
          //delete s.second.context;
        }

        foreach(SharedObjectMap::value_type row, _shared_objects) {
          //delete row.second;
          //row.second=NULL;
        }
      }
      }
    }
  }
