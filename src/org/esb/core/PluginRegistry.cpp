/* 
 * File:   PluginRegistry.cpp
 * Author: HoelscJ
 * 
 * Created on 29. August 2011, 14:46
 */

#include "PluginRegistry.h"
#include "ServicePlugin.h"
#include "introspec.h"
#include "org/esb/core/HookPlugin.h"
#include "org/esb/util/Foreach.h"
#include "org/esb/io/File.h"
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
      AppContext * PluginRegistry::context = NULL;
      PluginRegistry * PluginRegistry::_instance = NULL;

      PluginRegistry * PluginRegistry::getInstance() {
        if (_instance == NULL) {
          _instance = new PluginRegistry;
        }
        if (context == NULL) {
          context = new AppContext();

          context->env["web.port"] = getenv("web.port") == NULL ? "8080" : getenv("web.port");
          context->env["web.docroot"] = getenv("web.docroot") == NULL ? "." : getenv("web.docroot");

        }

        return _instance;
      }

      OptionsDescription PluginRegistry::getOptionsDescription(std::string name) {
        if(_plugin_map.count(name)>0){
          return _plugin_map[name]->getOptionsDescription();
        }
        return OptionsDescription();
      }

      std::list<std::string> PluginRegistry::getPluginNameList() {
        std::list<std::string> result;
        typedef std::map<std::string, Plugin*> PluginMap;
        foreach(PluginMap::value_type s, _plugin_map) {
          result.push_back(s.first);
        }
        return result;
      }

      void PluginRegistry::registerPlugin(std::string name, Plugin*plugin) {
      }

      void PluginRegistry::registerService(std::string name, ServicePlugin*plugin) {
        if (plugin == NULL)return;
        _plugin_map[name] = plugin;
        //OptionsDescription desc = plugin->getOptionsDescription();
        
        
      }

      void CORE_EXPORT PluginRegistry::startServices() {
        typedef std::map<std::string, Plugin*> PluginMap;

        foreach(PluginMap::value_type s, _plugin_map) {
          LOGDEBUG("Start Service:" << s.first);
          ((ServicePlugin*) s.second)->setContext(context);
          ((ServicePlugin*) s.second)->startService();
        }

      }

      void CORE_EXPORT PluginRegistry::stopServices() {
        typedef std::map<std::string, Plugin*> PluginMap;

        foreach(PluginMap::value_type s, _plugin_map) {
          ((ServicePlugin*) s.second)->stopService();
        }
      }

      void PluginRegistry::registerHookPlugin(std::string name, HookPlugin*plugin) {
        //LOGDEBUG("register HookPlugin "<<name);
        //_plugin_map[name] = plugin;
        if (_plugin_map.count("apiwebserver") > 0) {
          //static_cast<org::esb::core::HookProvider*>(_plugin_map["apiwebserver"])->addHook(url,plugin);
        }
        //
        //LOGDEBUG("delete service")
        //delete plugin;
      }

      void PluginRegistry::registerHookProvider(std::string name, HookProvider*plugin) {
        //LOGDEBUG("register HookProvider "<<name);
        _hook_provider_map[name] = plugin;
      }

      void PluginRegistry::close() {
        delete _instance;
        delete context;
      }

      PluginRegistry::PluginRegistry() {
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
        LOGDEBUG("PluginRegistry::~PluginRegistry()")
                typedef std::map<std::string, org::esb::lang::SharedObjectLoader*> SharedObjectMap;

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
