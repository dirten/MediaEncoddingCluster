/* 
 * File:   HookProvider.h
 * Author: HoelscJ
 *
 * Created on 30. August 2011, 16:47
 */

#ifndef HOOKPROVIDER_H
#define	HOOKPROVIDER_H
#include "Plugin.h"
#include "HookPlugin.h"
namespace org {
  namespace esb {
    namespace core {

      class HookProvider:public Plugin{
      public:
        virtual void addHook(std::string hookname, HookPlugin*)=0;
        virtual ~HookProvider(){};
      private:

      };
    }
  }
}


#endif	/* HOOKPROVIDER_H */

