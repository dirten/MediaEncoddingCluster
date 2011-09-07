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
#include "exports.h"
#include "mongoose.h"

namespace org {
  namespace esb {
    namespace api {

      class WEBSERVICE_EXPORT ServiceInputStream {
      public:
        ServiceInputStream( mg_connection *conn);
        //ServiceInputStream();
        ~ServiceInputStream();
        int read(string & str, int max=1500);
        int read(unsigned char * buffer, int length);
        int read(vector<unsigned char>&buffer) {return 0;};
        int read() {return 0;};
        long long int available(bool isBlocking = false);
      private:
        mg_connection *_conn;

      };

      class WEBSERVICE_EXPORT ServiceRequest : public org::esb::core::Request {
      public:
        ServiceRequest(mg_connection *conn, const mg_request_info *request_info);
        virtual ~ServiceRequest();
        std::string getMethod();
        std::string getRequestURI();
        ServiceInputStream * getInputstream();
        std::string getRemoteIP();
        int getRemotePort();
        std::string getUUID();
        std::string getParameter(std::string);
        bool hasParameter(std::string);
      private:
        ServiceInputStream * _inputstream;
        mg_connection *_conn;
        const mg_request_info *_request_info;

      };
    }
  }
}
#endif	/* SERVICEREQUEST_H */

