/* 
 * File:   HTTPResponse.h
 * Author: HoelscJ
 *
 * Created on 24. Januar 2012, 10:47
 */

#ifndef HTTPRESPONSE_H
#define	HTTPRESPONSE_H
#include "Poco/Net/HTTPResponse.h"
namespace org {
  namespace esb {
    namespace core {
      namespace http {

        class HTTPResponse : public Poco::Net::HTTPResponse {
        public:
          HTTPResponse();
          virtual ~HTTPResponse();
        private:

        };
      }
    }
  }
}

#endif	/* HTTPRESPONSE_H */

