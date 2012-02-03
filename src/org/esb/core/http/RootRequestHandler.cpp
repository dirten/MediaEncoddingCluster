/* 
 * File:   RootRequestHandler.cpp
 * Author: HoelscJ
 * 
 * Created on 3. Februar 2012, 15:58
 */

#include "RootRequestHandler.h"
namespace org {
  namespace esb {
    namespace core {
      namespace http {

        RootRequestHandler::RootRequestHandler() {
        }

        RootRequestHandler::~RootRequestHandler() {
        }
        void RootRequestHandler::handle(HTTPServerRequest& request, HTTPServerResponse& response){
          //response.setContentType("application/json");
          std::ostream& ostr = response.send();
        }
      }
    }
  }
}
