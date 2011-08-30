/* 
 * File:   Plugin.h
 * Author: HoelscJ
 *
 * Created on 29. August 2011, 14:49
 */
#include "PluginRegistry.h"
#include "AppContext.h"
#include <iostream>
#include <map>
#ifndef PLUGIN_H
#define	PLUGIN_H

namespace org {
  namespace esb {
    namespace core {
      class Plugin {
      public:
        virtual void setContext(AppContext * )=0;
        virtual ~Plugin(){};
        //virtual std::map<std::string,std::string> getProperties();

      };
    }
  }
}
#define REGISTER_PLUGIN(name,type) \
	class Register##type \
	        { \
	                public: \
	                        Register##type() \
	                        { \
	                        org::esb::core::PluginRegistry::getInstance()->registerPlugin(std::string(name), (org::esb::core::Plugin*)new type()); \
	                        } \
	        } Register##type##Instance; 
	
#define REGISTER_SERVICE(name,type) \
	class Register##type \
	        { \
	                public: \
	                        Register##type() \
	                        { \
	                        org::esb::core::PluginRegistry::getInstance()->registerService(std::string(name), (org::esb::core::ServicePlugin*)new type()); \
	                        } \
	        } Register##type##Instance; 

#define REGISTER_HOOK(name,url,type) \
	class Register##type \
	        { \
	                public: \
	                        Register##type() \
	                        { \
	                        org::esb::core::PluginRegistry::getInstance()->registerHookPlugin(std::string(name), (org::esb::core::HookPlugin*)new type()); \
	                        } \
	        } Register##type##Instance; 
#define REGISTER_HOOK_PROVIDER(name,type) \
	class Register##type \
	        { \
	                public: \
	                        Register##type() \
	                        { \
	                        org::esb::core::PluginRegistry::getInstance()->registerHookProvider(std::string(name), (org::esb::core::HookProvider*)new type()); \
	                        } \
	        } Register##type##Instance; 
	
#endif	/* PLUGIN_H */

