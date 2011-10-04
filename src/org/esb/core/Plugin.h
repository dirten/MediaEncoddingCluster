/* 
 * File:   Plugin.h
 * Author: HoelscJ
 *
 * Created on 29. August 2011, 14:49
 */
#include "PluginRegistry.h"
#include "HookNotificationCenter.h"
#include "PluginContext.h"

#include <iostream>
#include <map>
#include <boost/bind.hpp>
#include "boost/program_options.hpp"

#ifndef ORG_ESB_CORE_PLUGIN_H
#define	ORG_ESB_CORE_PLUGIN_H

namespace org {
  namespace esb {
    namespace core {
      typedef boost::program_options::options_description OptionsDescription;
      //typedef boost::program_options::value Value;


      class Plugin {
      public:

        void setContext(PluginContext * ac) {
          _ctx = ac;
        }

        org::esb::core::PluginContext*getContext() {
          return _ctx;
        }

        virtual ~Plugin() {
        };

        virtual OptionsDescription getOptionsDescription(){
          return OptionsDescription();
        };
        virtual void init(){};
        //virtual std::map<std::string,std::string> getProperties();
      private:
        org::esb::core::PluginContext*_ctx;
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

