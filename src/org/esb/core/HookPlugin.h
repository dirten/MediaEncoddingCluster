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


#endif	/* HOOKPLUGIN_H */

