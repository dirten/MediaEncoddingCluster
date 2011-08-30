/* 
 * File:   ServicePlugin.h
 * Author: HoelscJ
 *
 * Created on 29. August 2011, 15:48
 */

#ifndef SERVICEPLUGIN_H
#define	SERVICEPLUGIN_H
#include "Plugin.h"
namespace org {
  namespace esb {
    namespace core {

      class ServicePlugin: public Plugin{
      public:

        virtual void startService()=0;
        virtual void stopService()=0;
        virtual ~ServicePlugin(){};
      private:

      };
    }
  }
}

#endif	/* SERVICEPLUGIN_H */

