/*
 * File:   PluginRegistry.h
 * Author: HoelscJ
 *
 * Created on 29. August 2011, 14:46
 */

#ifndef PLUGINREGISTRY_H
#define	PLUGINREGISTRY_H
#include <map>
#include <list>
#include <string>
#include "http/Server.h"
#include "org/esb/util/Log.h"
#include "Plugin.h"
#include "org/esb/lang/SharedObjectLoader.h"
#include "org/esb/lang/Ptr.h"
#include "exports.h"

namespace org {
  namespace esb {
    namespace grid{
      class GridRegistry;
    }
    namespace core {
      class Plugin;
      class PluginContext;
      class ServicePlugin;
      class StorageEngine;
      class WebservicePlugin;
      class HookPlugin;
      class HookProvider;
      class Task;
      class WebHookHandlerFactory;
      /**
        * @TODO: build this class thread save !!!
        */
      class CORE_EXPORT PluginRegistry {
          classlogger("org.esb.core.PluginRegistry")
          public:
          enum PLUGIN_ORDER {
            NONE,
            NAME,
            PRIO
          };

          static  PluginRegistry * getInstance();
          static void close();
          void registerPlugin(std::string, Plugin*);
          void registerService(std::string, ServicePlugin*);
          void registerStorage(std::string, StorageEngine*);
          void registerHookPlugin(std::string, HookPlugin*);
          void registerHookProvider(std::string, HookProvider*);
          //void CORE_EXPORT registerTaskFactory(std::string,Ptr<TaskFactory>);
          void registerTaskFactory(std::string,TaskFactory*);
          void registerWebHookFactory(std::string name, WebHookFactory *factory);
          void startServerServices();
          void startWebService();
          void startServiceByName(std::string name);
          void stopServiceByName(std::string name);
          StorageEngine * getStorageEngine();
          void startStorageByName(std::string name);
          void stopStorageByName(std::string name);
          void startClientServices();
          void stopServices();
          void load(std::string);
          void initPlugins();
          bool isRunning(std::string plugin_name);
          OptionsDescription getOptionsDescription();
          OptionsDescription getOptionsDescription(std::string);
          std::list<std::string> getPluginNameList();

          std::list<std::string> getTaskNameList();
          OptionsDescription getTaskOptionsDescription(std::string);
          std::string getConfigData(std::string key);
          Ptr<Task> createTask(std::string name,std::map<std::string,std::string> cfg);

        private:
          PluginRegistry();
          virtual ~PluginRegistry();
          static PluginRegistry * _instance;
          void loadFile(std::string);
          void initPluginContext(Plugin*, PluginContext*);

          WebHookHandlerFactory * _webhook_handler_factory;
          struct PluginData{
              std::string name;
              Plugin * plugin;
              PluginContext * context;
          };

          std::map<std::string, Plugin*> _plug_map;
          std::map<std::string, ServicePlugin*> _service_map;
          std::map<std::string, StorageEngine*> _storage_map;
          std::map<std::string, HookProvider*> _hook_provider_map;
          std::map<std::string, HookPlugin*> _hook_map;
          std::map<std::string, org::esb::lang::SharedObjectLoader*> _shared_objects;
          std::map<std::string,PluginData> _plugin_data;
          //std::map<std::string,Ptr<TaskFactory> > _task_factories;
          typedef std::map<std::string,TaskFactory *> TaskFactoryMap;
          //typedef std::map<std::string,WebHookFactory *> WebHookFactoryMap;

          TaskFactoryMap _task_factories;
          //WebHookFactoryMap _webhook_factories;

          std::map<std::string,std::string> _config_data;
          org::esb::core::http::Server * server;

          Ptr<org::esb::grid::GridRegistry> _gridRegistry;
          std::list<std::string> _running_plugins;

          //bool compare_service_byname(std::string & lhs, std::string & rhs);
          //bool compare_service_byprio(std::string & lhs, std::string & rhs);
      };
    }
  }
}

#endif	/* PLUGINREGISTRY_H */

