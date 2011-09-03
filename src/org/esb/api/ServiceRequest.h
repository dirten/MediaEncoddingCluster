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

      class API_EXPORT ServiceInputStream {
      public:
        ServiceInputStream( mg_connection *conn);
        //ServiceInputStream();
        ~ServiceInputStream();
        int read(string & str);
        int read(unsigned char * buffer, int length);

        int read(vector<unsigned char>&buffer) {};

        int read() {};
        long long int available(bool isBlocking = false);
      private:
        mg_connection *_conn;

      };

      class API_EXPORT ServiceRequest : public org::esb::core::Request {
      public:
        ServiceRequest(mg_connection *conn, const mg_request_info *request_info);
        virtual ~ServiceRequest();
        std::string getMethod();
        std::string getRequestURI();
        ServiceInputStream * getInputstream();
        std::string getRemoteIP();
        int getRemotePort();
      private:
        ServiceInputStream * _inputstream;
        mg_connection *_conn;
        const mg_request_info *_request_info;
      };
    }
  }
}
#endif	/* SERVICEREQUEST_H */

