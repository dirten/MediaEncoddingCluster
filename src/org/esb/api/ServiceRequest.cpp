/* 
 * File:   ServiceRequest.cpp
 * Author: HoelscJ
 * 
 * Created on 30. August 2011, 11:26
 */

#include "ServiceRequest.h"
namespace org {
  namespace esb {
    namespace api {

      ServiceInputStream::ServiceInputStream(struct mg_connection *conn) {
        _conn = conn;
      }

      ServiceInputStream::~ServiceInputStream() {
      }

      int ServiceInputStream::read(string& str) {
        //int size = available();
        int bytes = 0, max = 1500, recv = 0;
        char buffer[100000];
        while ((bytes = mg_read(_conn, buffer, sizeof (buffer))) > 0) {
          str = str.append(buffer, bytes);
          max -= bytes;
          recv += bytes;
        }
      }

      int ServiceInputStream::read(unsigned char* buffer, int length) {
        return mg_read(_conn, buffer, length);
      }

      long long int ServiceInputStream::available(bool isBlocking) {
        return 0; //_conn->content_len - _conn->consumed_content;
      }

      ServiceRequest::ServiceRequest(struct mg_connection *conn, const struct mg_request_info *request_info) {
        _conn = conn;
        _request_info = request_info;
        _inputstream = new ServiceInputStream(conn);

      }

      ServiceRequest::~ServiceRequest() {
        delete _inputstream;
      }

      std::string ServiceRequest::getMethod() {
        return _request_info->request_method;
      }

      std::string ServiceRequest::getRequestURI() {
        return _request_info->uri;
      }

      ServiceInputStream * ServiceRequest::getInputstream() {
        return _inputstream;
      }

      std::string ServiceRequest::getRemoteIP() {
        return address(_conn);
      }

      int ServiceRequest::getRemotePort() {
        return _request_info->remote_port;
      }


    }
  }
}

