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
//#include "mongoose.h"
struct mg_connection;
struct mg_request_info;
namespace org {
  namespace esb {
    namespace api {

      class WEBSERVICE_EXPORT ServiceOutputStream {
      public:
        ServiceOutputStream(mg_connection *conn);
        ~ServiceOutputStream();
        int write(std::string data);
      private:
        mg_connection *_conn;
        bool _sent_status;
      };

      class WEBSERVICE_EXPORT ServiceResponse : public org::esb::core::Response {
      public:
        ServiceResponse(mg_connection *conn, const mg_request_info *request_info);
        ~ServiceResponse();
        ServiceOutputStream * getOutputStream();
        void setStatus(int);
      private:
        ServiceOutputStream * _outputstream;
        mg_connection *_conn;
        const mg_request_info *_request_info;
        friend class ApiWebServer;
        int _status;
      };
    }
  }
}
#endif	/* SERVICERESPONSE_H */

