/* 
 * File:   ServiceResponse.cpp
 * Author: HoelscJ
 * 
 * Created on 30. August 2011, 11:27
 */

#include "ServiceResponse.h"
#include "mongoose.h"
#include "org/esb/util/StringUtil.h"
namespace org {
  namespace esb {
    namespace api {

      ServiceOutputStream::ServiceOutputStream(mg_connection *conn, ServiceResponse*res) : _conn(conn), _response(res) {
        _sent_header=false;
      }

      ServiceOutputStream::~ServiceOutputStream() {
        _sent_header = false;
      }

      int ServiceOutputStream::write(std::string data) {
        /*allways when i write out something, assume that a status of 200 is sent*/
        if (!_sent_header) {
          std::string status = org::esb::util::StringUtil::toString(_response->getStatus());
          std::string header;
          header += "HTTP/1.1 ";
          header += status + "\r\n";
          header += "Cache: no-cache\r\n";
          header += "Content-Type: ";
          header += _response->getMimetype() + "; charset=utf-8\r\n";
          header += "\r\n";
          header += "";

          mg_printf(_conn, "%s", header.c_str());
          _sent_header = true;
        }
        return mg_write(_conn, data.c_str(), data.length());
      }

      ServiceResponse::ServiceResponse(mg_connection *conn, const mg_request_info *request_info) {
        _conn = conn;
        _request_info = request_info;

        _outputstream = new ServiceOutputStream(conn, this);
        _status = NONE;
        _mime = "text/plain";
      }

      ServiceResponse::~ServiceResponse() {
        delete _outputstream;
      }

      ServiceOutputStream * ServiceResponse::getOutputStream() {
        return _outputstream;
      }

      void ServiceResponse::setStatus(Status st) {
        _status = st;
      }

      ServiceResponse::Status ServiceResponse::getStatus() {
        return _status;
      }

      void ServiceResponse::setMimetype(std::string type) {
        _mime = type;
      }

      std::string ServiceResponse::getMimetype() {
        return _mime;
      }

      void ServiceResponse::flush() {
        _outputstream->write("");
      }
    }
  }
}
