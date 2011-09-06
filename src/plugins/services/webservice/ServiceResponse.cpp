/* 
 * File:   ServiceResponse.cpp
 * Author: HoelscJ
 * 
 * Created on 30. August 2011, 11:27
 */

#include "ServiceResponse.h"
#include "mongoose.h"
namespace org {
  namespace esb {
    namespace api {

      ServiceOutputStream::ServiceOutputStream(mg_connection *conn) : _conn(conn) {

      }

      ServiceOutputStream::~ServiceOutputStream() {
        _sent_status = false;
      }

      int ServiceOutputStream::write(std::string data) {
        /*allways when i write out something, assume that a status of 200 is sent*/
        if (!_sent_status) {
          static const char *reply_start =
                  "HTTP/1.1 200 OK\r\n"
                  "Cache: no-cache\r\n"
                  "Content-Type: text/plain; charset=utf-8\r\n"
                  "\r\n";
          mg_printf(_conn, "%s", reply_start);
        }
        mg_write(_conn, data.c_str(), data.length());
      }

      ServiceResponse::ServiceResponse(mg_connection *conn, const mg_request_info *request_info) {
        _conn = conn;
        _request_info = request_info;

        _outputstream = new ServiceOutputStream(conn);
        _status = 0;
      }

      ServiceResponse::~ServiceResponse() {
        delete _outputstream;
      }

      ServiceOutputStream * ServiceResponse::getOutputStream() {
        return _outputstream;
      }

      void ServiceResponse::setStatus(int st) {
        _status = st;
      }
    }
  }
}
