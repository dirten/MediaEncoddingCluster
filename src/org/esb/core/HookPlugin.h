/* 
 * File:   HookPlugin.h
 * Author: HoelscJ
 *
 * Created on 30. August 2011, 16:46
 */

#ifndef HOOKPLUGIN_H
#define	HOOKPLUGIN_H
#include "Plugin.h"
#include "PluginRegistry.h"
#include "HookNotificationCenter.h"
#include "Request.h"
#include "Response.h"

namespace org {
  namespace esb {
    namespace core {

      class HookPlugin : public Plugin {
      public:
        virtual ~HookPlugin() {
        };
      private:

      };
    }
  }
}
#define REGISTER_HOOK(name,instance, function, prio) \
	class Register##instance##prio \
	        { \
			instance * element##instance; \
	                public: \
	                        Register##instance##prio() \
	                        { \
				element##instance=new instance(); \
                                std::string tmpname=#instance; \
                                org::esb::core::PluginRegistry::getInstance()->registerHookPlugin(tmpname, element##instance); \
                                org::esb::core::HookNotificationCenter::getInstance()->addObserver(name,boost::bind(&function, element##instance,_1,_2),prio); \
	                        } \
	                        ~Register##instance##prio() \
	                        { \
				delete element##instance; \
	                        } \
	        } Register##instance##Instance##prio; 


#endif	/* HOOKPLUGIN_H */

