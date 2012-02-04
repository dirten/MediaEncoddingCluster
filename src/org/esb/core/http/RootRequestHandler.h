/* 
 * File:   RootRequestHandler.h
 * Author: HoelscJ
 *
 * Created on 3. Februar 2012, 15:58
 */

#ifndef ROOTREQUESTHANDLER_H
#define	ROOTREQUESTHANDLER_H
#include "./RequestHandler.h"
#include "exports.h"
namespace org {
  namespace esb {
    namespace core {
      namespace http {

        class CORE_HTTP_EXPORT RootRequestHandler : public RequestHandler{
        public:
          RootRequestHandler();
          virtual ~RootRequestHandler();
          void handle(HTTPServerRequest& request, HTTPServerResponse& response);
        private:

        };
      }
    }
  }
}

#endif	/* ROOTREQUESTHANDLER_H */

