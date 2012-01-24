/* 
 * File:   RequestHandlerFactory.h
 * Author: HoelscJ
 *
 * Created on 24. Januar 2012, 11:05
 */

#ifndef REQUESTHANDLERFACTORY_H
#define	REQUESTHANDLERFACTORY_H
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "RequestHandler.h"
#include "HTTPRequest.h"
namespace org{
  namespace esb{
    namespace core{
      namespace http{
        class RequestHandlerFactory: public Poco::Net::HTTPRequestHandlerFactory{
          void addRequestHandler(RequestHandler&);
          RequestHandler * createRequestHandler(HTTPRequest&);
        };
      }
    }
  }
}


#endif	/* REQUESTHANDLERFACTORY_H */

