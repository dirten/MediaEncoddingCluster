/* 
 * File:   WebserverPlugin.h
 * Author: HoelscJ
 *
 * Created on 30. August 2011, 11:17
 */

#ifndef WEBSERVERPLUGIN_H
#define	WEBSERVERPLUGIN_H
#include "HookPlugin.h"

namespace org {
  namespace esb {
    namespace api{
      class ServiceRequest;
      class ServiceResponse;
    }
    namespace core {
      class WebservicePlugin: public HookPlugin{
      public:
        virtual void handle(org::esb::api::ServiceRequest *, org::esb::api::ServiceResponse *)=0;
        
        virtual ~WebservicePlugin(){};
      private:

      };
    }
  }
}



#endif	/* WEBSERVERPLUGIN_H */

