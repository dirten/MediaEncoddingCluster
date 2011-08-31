/* 
 * File:   ServiceResponse.h
 * Author: HoelscJ
 *
 * Created on 30. August 2011, 11:27
 */

#ifndef SERVICERESPONSE_H
#define	SERVICERESPONSE_H
#include "org/esb/core/Response.h"

namespace org {
  namespace esb {
    namespace api {

      class ServiceResponse: public org::esb::core::Response {
      public:
        ServiceResponse();
        virtual ~ServiceResponse();
      private:

      };
    }
  }
}
#endif	/* SERVICERESPONSE_H */

