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
        static PluginRegistry * getInstance();
        void registerPlugin(std::string, Plugin*);
        void registerService(std::string, ServicePlugin*);
        void registerWebservice(std::string,std::string, WebservicePlugin*);
        void registerHookPlugin(std::string, HookPlugin*);
        void registerHookProvider(std::string, HookProvider*);
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

