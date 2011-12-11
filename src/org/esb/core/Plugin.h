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
namespace org {
  namespace esb {
    namespace core {
      class PluginContext;
      class Task;
      typedef boost::program_options::options_description OptionsDescription;
      //typedef boost::program_options::value Value;

      class CORE_EXPORT Plugin {
      public:

        void setContext(Ptr<PluginContext> ac) {
          _ctx = ac;
        }

        Ptr<org::esb::core::PluginContext>getContext() {
          return _ctx;
        }

        virtual ~Plugin() {
        };

        virtual OptionsDescription getOptionsDescription() {
          return OptionsDescription();
        };

        virtual void init() {
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
      };
      typedef Factory<Task> TaskFactory;

    }
  }
}

#endif	/* PLUGIN_H */

