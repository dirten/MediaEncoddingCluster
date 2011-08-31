/* 
 * File:   ServiceRequest.h
 * Author: HoelscJ
 *
 * Created on 30. August 2011, 11:26
 */

#ifndef SERVICEREQUEST_H
#define	SERVICEREQUEST_H
#include "org/esb/io/InputStream.h"
#include "org/esb/core/Request.h"
namespace org {
  namespace esb {
    namespace api {
      class ServiceInputStream: public org::esb::io::InputStream{
      public:
        ServiceInputStream();
      };

      class ServiceRequest: public org::esb::core::Request {
      public:
        ServiceRequest();
        virtual ~ServiceRequest();
        std::string getMethod();
        std::string getRequestURI();
        ServiceInputStream & getInputstream();
      private:

      };
    }
  }
}
#endif	/* SERVICEREQUEST_H */

