/* 
 * File:   HTTPRequest.h
 * Author: HoelscJ
 *
 * Created on 24. Januar 2012, 10:47
 */

#ifndef HTTPREQUEST_H
#define	HTTPREQUEST_H
#include "Poco/Net/HTTPRequest.h"
namespace org {
  namespace esb {
    namespace core {
      namespace http {

        class HTTPRequest : public Poco::Net::HTTPRequest {
        public:
          HTTPRequest();
          virtual ~HTTPRequest();
        private:

        };
      }
    }
  }
}
#endif	/* HTTPREQUEST_H */

