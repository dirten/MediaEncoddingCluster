/* 
 * File:   HTTPServerResponse.h
 * Author: HoelscJ
 *
 * Created on 26. Januar 2012, 13:34
 */

#ifndef HTTPSERVERRESPONSE_H
#define	HTTPSERVERRESPONSE_H
#include "Poco/Net/HTTPServerResponse.h"
namespace org {
  namespace esb {
    namespace core {
      namespace http {

        class HTTPServerResponse: public Poco::Net::HTTPServerResponse {
        public:
          HTTPServerResponse();
          virtual ~HTTPServerResponse();
        private:

        };
      }
    }
  }
}
#endif	/* HTTPSERVERRESPONSE_H */

