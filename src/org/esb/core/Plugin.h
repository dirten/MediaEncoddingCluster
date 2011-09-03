/* 
 * File:   Plugin.h
 * Author: HoelscJ
 *
 * Created on 29. August 2011, 14:49
 */
#include "PluginRegistry.h"
#include "HookNotificationCenter.h"
#include "AppContext.h"
#include <iostream>
#include <map>
#include <boost/bind.hpp>
#ifndef PLUGIN_H
#define	PLUGIN_H

namespace org {
  namespace esb {
    namespace core {
      class Plugin {
      public:
        void setContext(AppContext * ac){_ctx=ac;}
        org::esb::core::AppContext*getContext(){return _ctx;}
        virtual ~Plugin(){};
        //virtual std::map<std::string,std::string> getProperties();
      private:
        org::esb::core::AppContext*_ctx;
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

#define REGISTER_HOOK(name,instance, function, id) \
	class Register##instance##id \
	        { \
	                public: \
	                        Register##instance##id() \
	                        { \
                                org::esb::core::HookNotificationCenter::getInstance()->addObserver(name,boost::bind(&function, &instance,_1,_2)); \
	                        } \
	        } Register##instance##Instance##id; 
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

