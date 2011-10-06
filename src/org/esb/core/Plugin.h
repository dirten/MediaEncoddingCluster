/* 
 * File:   Plugin.h
 * Author: HoelscJ
 *
 * Created on 29. August 2011, 14:49
 */

#ifndef ORG_ESB_CORE_PLUGIN_H
#define	ORG_ESB_CORE_PLUGIN_H

#include "boost/program_options.hpp"

namespace org {
  namespace esb {
    namespace core {
      class PluginContext;
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
    }
  }
}

#endif	/* PLUGIN_H */

