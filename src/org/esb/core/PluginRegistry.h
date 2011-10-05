/* 
 * File:   PluginRegistry.h
 * Author: HoelscJ
 *
 * Created on 29. August 2011, 14:46
 */

#ifndef PLUGINREGISTRY_H
#define	PLUGINREGISTRY_H
#include <map>
#include <set>
#include <string>
#include "Plugin.h"
#include "AppContext.h"
#include "org/esb/lang/SharedObjectLoader.h"
#include "org/esb/util/Log.h"
#include "PluginContext.h"
namespace org {
  namespace esb {
    namespace core {
      class Plugin;
      class ServicePlugin;
      class WebservicePlugin;
      class HookPlugin;
      class HookProvider;
      class PluginRegistry {
        classlogger("org.esb.core.PluginRegistry")
      public:
        static CORE_EXPORT PluginRegistry * getInstance();
        static CORE_EXPORT void close();
        void CORE_EXPORT registerPlugin(std::string, Plugin*);
        void CORE_EXPORT registerService(std::string, ServicePlugin*);
        void CORE_EXPORT registerHookPlugin(std::string, HookPlugin*);
        void CORE_EXPORT registerHookProvider(std::string, HookProvider*);
        void CORE_EXPORT startServices();
        void CORE_EXPORT stopServices();
        void CORE_EXPORT load(std::string);
        void CORE_EXPORT initPlugins();
        
        OptionsDescription CORE_EXPORT getOptionsDescription(std::string);
        std::list<std::string> CORE_EXPORT getPluginNameList();
        
        
      private:
        PluginRegistry();
        virtual ~PluginRegistry();
        static PluginRegistry * _instance;
        void loadFile(std::string);
        struct PluginData{
          std::string name;
          Plugin * plugin;
          PluginContext * context;
        };

        std::map<std::string, Plugin*> _plug_map;
        std::map<std::string, ServicePlugin*> _service_map;
        std::map<std::string, HookProvider*> _hook_provider_map;
        std::map<std::string, HookPlugin*> _hook_map;
        std::map<std::string, org::esb::lang::SharedObjectLoader*> _shared_objects;
        std::map<std::string,PluginData> _plugin_data;
      };
    }
  }
}

#endif	/* PLUGINREGISTRY_H */

