/* 
 * File:   RequestHandler.h
 * Author: HoelscJ
 *
 * Created on 24. Januar 2012, 11:07
 */

#ifndef REQUESTHANDLER_H
#define	REQUESTHANDLER_H
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "HTTPServerRequest.h"
#include "HTTPServerResponse.h"
#include "exports.h"
namespace org{
  namespace esb{
    namespace core{
      namespace http{
        class CORE_HTTP_EXPORT RequestHandler: public Poco::Net::HTTPRequestHandler{
        public:
            virtual ~RequestHandler(){}
          virtual void handle(HTTPServerRequest& request, HTTPServerResponse& response)=0;
          void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response){
            HTTPServerRequest * request1=static_cast<HTTPServerRequest *>(&request);
            HTTPServerResponse * response1=static_cast<HTTPServerResponse *>(&response);
            handle(*request1,* response1);
          }
        };
      }
    }
  }
}



#endif	/* REQUESTHANDLER_H */

