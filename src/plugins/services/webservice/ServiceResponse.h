/* 
 * File:   ServiceResponse.h
 * Author: HoelscJ
 *
 * Created on 30. August 2011, 11:27
 */

#ifndef SERVICERESPONSE_H
#define	SERVICERESPONSE_H
#include "org/esb/core/Response.h"
#include "exports.h"
#include <string>
#include <map>
//#include "mongoose.h"
struct mg_connection;
struct mg_request_info;
namespace org {
  namespace esb {
    namespace api {
      class ServiceResponse;
      class WEBSERVICE_EXPORT ServiceOutputStream {
      public:
        ServiceOutputStream(mg_connection *conn,ServiceResponse*);
        ~ServiceOutputStream();
        int write(std::string data);
        void setHeader(std::string key, std::string value);
      private:
        mg_connection *_conn;
        bool _sent_header;
        ServiceResponse*_response;
        typedef std::map<std::string, std::string> Header;
        Header _header;
      };
            
      class WEBSERVICE_EXPORT ServiceResponse : public org::esb::core::Response {
      public:
        enum Status {
          NONE=0,
          OK = 200,
          BAD_REQUEST = 400,
          UNAUTHORIZED = 401,
          FORBIDDEN = 403,
          NOT_FOUND = 404
        };
        ServiceResponse(mg_connection *conn, const mg_request_info *request_info);
        ~ServiceResponse();
        ServiceOutputStream * getOutputStream();
        void setStatus(Status);
        Status getStatus();
        void setMimetype(std::string type);
        std::string getMimetype();
        void flush();
      private:
        ServiceOutputStream * _outputstream;
        mg_connection *_conn;
        const mg_request_info *_request_info;
        friend class ApiWebServer;
        Status _status;
        std::string _mime;
      };
    }
  }
}
#endif	/* SERVICERESPONSE_H */

