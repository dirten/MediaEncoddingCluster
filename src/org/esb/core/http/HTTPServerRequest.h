/* 
 * File:   HTTPServerRequest.h
 * Author: HoelscJ
 *
 * Created on 26. Januar 2012, 13:17
 */

#ifndef HTTPSERVERREQUEST_H
#define	HTTPSERVERREQUEST_H
#include "Poco/Net/HTTPServerRequest.h"
#include "exports.h"
namespace org {
  namespace esb {
    namespace core {
      namespace http {

        class CORE_HTTP_EXPORT HTTPServerRequest : public Poco::Net::HTTPServerRequest {
        public:
          HTTPServerRequest();
          virtual ~HTTPServerRequest();
          
        private:

        };
      }
    }
  }
}
#endif	/* HTTPSERVERREQUEST_H */

