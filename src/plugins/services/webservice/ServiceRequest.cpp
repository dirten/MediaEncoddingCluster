/* 
 * File:   ServiceRequest.cpp
 * Author: HoelscJ
 * 
 * Created on 30. August 2011, 11:26
 */

#include "ServiceRequest.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "boost/lexical_cast.hpp"
#include "org/esb/util/Log.h"
#include "org/esb/util/StringTokenizer.h"
namespace org {
  namespace esb {
    namespace api {

      ServiceInputStream::ServiceInputStream(struct mg_connection *conn) {
        _conn = conn;
      }

      ServiceInputStream::~ServiceInputStream() {
      }

      int ServiceInputStream::read(string& str, int max) {
        //int size = available();
        int bytes = 0, recv = 0;
        char buffer[100000];

        while (max > 0 && (bytes = mg_read(_conn, buffer, sizeof (buffer))) > 0) {
          str = str.append(buffer, bytes);
          max -= bytes;
          recv += bytes;
        }
        return recv;
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

      std::string ServiceRequest::getUUID() {
        boost::uuids::uuid uuid = boost::uuids::random_generator()();
        return boost::lexical_cast<std::string > (uuid);
      }

      std::string ServiceRequest::getParameter(std::string key) {
        char iddata[100];
        memset(&iddata, 0, 100);
        if (_request_info->query_string != NULL) {
          mg_get_var(_request_info->query_string, strlen(_request_info->query_string), key.c_str(), iddata, sizeof (iddata));
        }
        return iddata;
      }

      bool ServiceRequest::hasParameter(std::string key) {
        bool result = false;
        if (_request_info->query_string != NULL) {
          org::esb::util::StringTokenizer st(_request_info->query_string, "&");
          while (st.hasMoreTokens()) {
            std::string tk = st.nextToken();
            if (tk == key) {
              result = true;
              break;
            }
          }
        }
        return result;
      }
    }
  }
}

