/* 
 * File:   PluginRegistry.cpp
 * Author: HoelscJ
 * 
 * Created on 29. August 2011, 14:46
 */

#include "PluginRegistry.h"
#include "Plugin.h"
#include "ServicePlugin.h"
#include "WebservicePlugin.h"
#include "introspec.h"
#include "org/esb/util/Log.h"
#include "org/esb/core/HookProvider.h"
//#include "org/esb/api/ApiWebServer.h"
namespace org {
  namespace esb {
    namespace core {
      AppContext * PluginRegistry::context=NULL;
      PluginRegistry * PluginRegistry::_instance = NULL;

      PluginRegistry * PluginRegistry::getInstance() {
        if (_instance == NULL){
          _instance = new PluginRegistry;          
        }
        if(context==NULL){
          context=new AppContext();
          
          context->env["web.port"]=getenv("web.port")==NULL?"8080":getenv("web.port");
          context->env["web.docroot"]=getenv("web.docroot")==NULL?".":getenv("web.docroot");
          
        }
          
        return _instance;
      }

      void PluginRegistry::registerPlugin(std::string name, Plugin*plugin) {
        LOGDEBUG("register plugin")
        _plugin_map[name]=plugin;
        LOGDEBUG("delete plugin")
        delete plugin;
      }
      
      void PluginRegistry::registerService(std::string name, ServicePlugin*plugin) {
        LOGDEBUG("register Servicename:" <<name)
        _plugin_map[name]=plugin;
        plugin->setContext(context);
        plugin->startService();
        //plugin->stopService();
        //LOGDEBUG("delete service")
        //delete plugin;
      }
      void PluginRegistry::registerWebservice(std::string name,std::string url, WebservicePlugin*plugin) {
        LOGDEBUG("register WebService "<<name<< " on url "<<url);
        _plugin_map[name]=plugin;
        if(_plugin_map.count("apiwebserver")>0){
          //static_cast<org::esb::core::HookProvider*>(_plugin_map["apiwebserver"])->addHook(url,plugin);
        }
        //
        //LOGDEBUG("delete service")
        //delete plugin;
      }
      void PluginRegistry::registerHookPlugin(std::string name, HookPlugin*plugin) {
        LOGDEBUG("register HookPlugin "<<name);
        _plugin_map[name]=plugin;
        if(_plugin_map.count("apiwebserver")>0){
          //static_cast<org::esb::core::HookProvider*>(_plugin_map["apiwebserver"])->addHook(url,plugin);
        }
        //
        //LOGDEBUG("delete service")
        //delete plugin;
      }
      void PluginRegistry::registerHookProvider(std::string name, HookProvider*plugin) {
        LOGDEBUG("register HookPlugin "<<name);
        _plugin_map[name]=plugin;
        if(_plugin_map.count("apiwebserver")>0){
          //static_cast<org::esb::core::HookProvider*>(_plugin_map["apiwebserver"])->addHook(url,plugin);
        }
        //
        //LOGDEBUG("delete service")
        //delete plugin;
      }

      PluginRegistry::PluginRegistry() {
      }

      PluginRegistry::~PluginRegistry() {
      }
    }
    namespace api{
  //    REGISTER_SERVICE("apiwebserver", ApiWebServer)
    }
  }
}
