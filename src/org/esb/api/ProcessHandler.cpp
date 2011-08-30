/* 
 * File:   ProcessHandler.cpp
 * Author: HoelscJ
 * 
 * Created on 13. Juli 2011, 11:13
 */

#include "ProcessHandler.h"
#include "org/esb/util/Log.h"
namespace org {
  namespace esb {
    namespace api {

      ProcessHandler::ProcessHandler() {
      }

      ProcessHandler::~ProcessHandler() {
      }

      void ProcessHandler::handle(org::esb::api::ServiceRequest *, org::esb::api::ServiceResponse *) {
        LOGDEBUG("handle url");
      }

      void ProcessHandler::setContext(org::esb::core::AppContext*) {

      }
    }
  }
}
