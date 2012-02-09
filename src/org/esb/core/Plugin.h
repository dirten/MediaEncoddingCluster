/* 
 * File:   Plugin.h
 * Author: HoelscJ
 *
 * Created on 29. August 2011, 14:49
 */

#ifndef ORG_ESB_CORE_PLUGIN_H
#define	ORG_ESB_CORE_PLUGIN_H

#include "boost/program_options.hpp"
#include "org/esb/lang/Ptr.h"
#include "exports.h"
#define ENGINE_VERSION 1

namespace org {
  namespace esb {
    namespace core {
      class PluginContext;
      class Task;
      class WebHookPlugin;
      
      typedef boost::program_options::options_description OptionsDescription;
      //typedef boost::program_options::value Value;

      class Plugin {
      public:

        void CORE_EXPORT setContext(Ptr<PluginContext> ac) {
          _ctx = ac;
        }

        Ptr<org::esb::core::PluginContext> CORE_EXPORT getContext() {
          return _ctx;
        }

        virtual CORE_EXPORT ~Plugin() {
        };

        virtual OptionsDescription CORE_EXPORT getOptionsDescription() {
          return OptionsDescription();
        };

        virtual void CORE_EXPORT init() {
        };
        //virtual std::map<std::string,std::string> getProperties();
      private:
        Ptr<org::esb::core::PluginContext>_ctx;
      };

      template<typename Interface>
      class Factory {
      public:
        virtual Ptr<Interface> create() = 0;
        virtual OptionsDescription getOptionsDescription()=0;
        int getEngineVersion(){return ENGINE_VERSION;}
      };
      typedef Factory<Task> TaskFactory;

      template<typename Interface>
      class WebHook  {
      public:
        virtual Interface * create() = 0;
        virtual OptionsDescription getOptionsDescription()=0;
        int getEngineVersion(){return ENGINE_VERSION;}
        virtual std::string getUrl() = 0;
        virtual std::string getMethod() = 0;
      };

      typedef WebHook<WebHookPlugin> WebHookFactory;

    }
  }
}

#endif	/* PLUGIN_H */

