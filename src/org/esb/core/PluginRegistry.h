/* 
 * File:   PluginRegistry.h
 * Author: HoelscJ
 *
 * Created on 29. August 2011, 14:46
 */

#ifndef PLUGINREGISTRY_H
#define	PLUGINREGISTRY_H
#include <map>
#include <string>
#include "AppContext.h"
namespace org {
  namespace esb {
    namespace core {
      class Plugin;
      class ServicePlugin;
      class WebservicePlugin;
      class HookPlugin;
      class HookProvider;
      class PluginRegistry {
      public:
        static CORE_EXPORT PluginRegistry * getInstance();
        void CORE_EXPORT registerPlugin(std::string, Plugin*);
        void CORE_EXPORT registerService(std::string, ServicePlugin*);
        void CORE_EXPORT registerWebservice(std::string,std::string, WebservicePlugin*);
        void CORE_EXPORT registerHookPlugin(std::string, HookPlugin*);
        void CORE_EXPORT registerHookProvider(std::string, HookProvider*);
        static AppContext * context;
      private:
        PluginRegistry();
        virtual ~PluginRegistry();
        static PluginRegistry * _instance;
        std::map<std::string, Plugin*> _plugin_map;
        std::map<std::string, Plugin*> _hook_provider_map;
      };
    }
  }
}

#endif	/* PLUGINREGISTRY_H */

