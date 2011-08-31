/* 
 * File:   ProcessHandler.h
 * Author: HoelscJ
 *
 * Created on 13. Juli 2011, 11:13
 */

#ifndef PROCESSHANDLER_H
#define	PROCESSHANDLER_H
#include "org/esb/core/WebservicePlugin.h"
#include "org/esb/core/AppContext.h"

namespace org {
  namespace esb {
    namespace api {

      class ProcessHandler: public org::esb::core::WebservicePlugin {
      public:
        ProcessHandler();
        virtual ~ProcessHandler();
        void handle(org::esb::api::ServiceRequest *, org::esb::api::ServiceResponse *);
        void hook(void*a,void*b){};
        void setContext(org::esb::core::AppContext*);
      private:

      }processHandler;
      //REGISTER_HOOK("webserver.url.hook",processHandler,ProcessHandler::hook)
    }
  }
}
#endif	/* PROCESSHANDLER_H */

