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
#ifndef ORG_ESB_CORE_PLUGIN_H
#define	ORG_ESB_CORE_PLUGIN_H

namespace org {
  namespace esb {
    namespace core {

      class Plugin {
      public:

        void setContext(AppContext * ac) {
          _ctx = ac;
        }

        org::esb::core::AppContext*getContext() {
          return _ctx;
        }

        virtual ~Plugin() {
        };
        //virtual std::map<std::string,std::string> getProperties();
      private:
        org::esb::core::AppContext*_ctx;
      };

      template<typename Interface>
      class Factory {
        virtual Interface * create();
      };

      class WebService {
        void doRequest(Request *, Response*);
      };
      typedef Factory<WebService> WebServiceFactory;
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
                        org::esb::core::ServicePlugin* element##type; \
	                public: \
	                        Register##type() \
	                        { \
                                element##type=new type(); \
	                        org::esb::core::PluginRegistry::getInstance()->registerService(std::string(name), element##type); \
	                        } \
                                ~Register##type(){ \
                                } \
	        } Register##type##Instance; 

#define REGISTER_HOOK(name,instance, function, prio) \
	class Register##instance##id \
	        { \
			instance * element##instance; \
	                public: \
	                        Register##instance##id() \
	                        { \
				element##instance=new instance(); \
                                org::esb::core::HookNotificationCenter::getInstance()->addObserver(name,boost::bind(&function, element##instance,_1,_2),prio); \
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

