/* 
 * File:   HookPlugin.h
 * Author: HoelscJ
 *
 * Created on 30. August 2011, 16:46
 */

#ifndef HOOKPLUGIN_H
#define	HOOKPLUGIN_H
#include "Plugin.h"
#include "Request.h"
#include "Response.h"

namespace org {
  namespace esb {
    namespace core {

      class HookPlugin: public Plugin{
      public:
        //virtual void hook()=0;
        virtual ~HookPlugin(){};
      private:

      };
    }
  }
}
#define REGISTER_HOOK(name,instance, function, prio) \
	class Register##instance##id \
	        { \
			instance * element##instance; \
	                public: \
	                        Register##instance##id() \
	                        { \
				element##instance=new instance(); \
                                org::esb::core::PluginRegistry::getInstance()->registerHookPlugin(std::string(name), element##instance); \
                                org::esb::core::HookNotificationCenter::getInstance()->addObserver(name,boost::bind(&function, element##instance,_1,_2),prio); \
	                        } \
	        } Register##instance##Instance##id; 


#endif	/* HOOKPLUGIN_H */

