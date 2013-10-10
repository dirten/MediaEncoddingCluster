/* 
 * File:   RequestHandlerFactory.h
 * Author: HoelscJ
 *
 * Created on 24. Januar 2012, 11:05
 */

#ifndef REQUESTHANDLERFACTORY_H
#define	REQUESTHANDLERFACTORY_H
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "RequestHandler.h"
#include "HTTPServerRequest.h"
#include "exports.h"
namespace org {
  namespace esb {
    namespace core {
      namespace http {

        class CORE_HTTP_EXPORT RequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory {
          void addRequestHandler(RequestHandler&);
        public:
          virtual RequestHandler * createHandler(HTTPServerRequest&) = 0;
          virtual ~RequestHandlerFactory(){}
          Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest&req) {
            Poco::Net::HTTPServerRequest * ptr = const_cast<Poco::Net::HTTPServerRequest *> (&req);
            HTTPServerRequest* httpPtr = static_cast<HTTPServerRequest*> (ptr);
            return createHandler(*httpPtr);
          }
        };
      }
    }
  }
}


#endif	/* REQUESTHANDLERFACTORY_H */

