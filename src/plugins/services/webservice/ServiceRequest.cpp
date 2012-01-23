/* 
 * File:   ServiceRequest.cpp
 * Author: HoelscJ
 * 
 * Created on 30. August 2011, 11:26
 */

#include "ServiceRequest.h"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "boost/lexical_cast.hpp"
#include "org/esb/util/Log.h"
#include "org/esb/util/UUID.h"
#include "org/esb/util/StringTokenizer.h"


using namespace std;
namespace org {
  namespace esb {
    namespace api {

      ServiceInputStream::ServiceInputStream(struct mg_connection *conn) {
        _conn = conn;
      }

      ServiceInputStream::~ServiceInputStream() {
      }

      int ServiceInputStream::read(string& str, int max) {
        int bytes = 0, recv = 0;
        size_t size = min(10000, max);
        char * buffer = new char[size];

        while (max > 0 && (bytes = mg_read(_conn, buffer, size)) > 0) {
          str = str.append(buffer, bytes);
          max -= bytes;
          recv += bytes;
        }
        //LOGDEBUG("received data size="<<recv);
        return recv;
      }

      int ServiceInputStream::read(string & str, boost::function<bool (std::string data) > func, int size) {
        std::string tmp;
        while (read(tmp, size)) {
          if (func(tmp)) {
            str.append(tmp);
          } else {
            break;
          }
          tmp.clear();
        }
        return str.length();
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
        if(_uuid.length()==0){
          _uuid=org::esb::util::PUUID();
        }
        return _uuid;//boost::lexical_cast<std::string > (uuid);
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
        //LOGDEBUG("Query String"<<_request_info->query_string);
        if (_request_info->query_string != NULL) {
          org::esb::util::StringTokenizer st(_request_info->query_string, "&");
          while (st.hasMoreTokens()) {
            std::string tk = st.nextToken();
            //LOGDEBUG("token:"<<tk);
            if (tk.find(key) == 0 ) {
              result = true;
              break;
            }
          }
        }
        return result;
      }

      std::string ServiceRequest::getHeader(std::string name) {
        std::string result;
        for (int a = 0; a < _request_info->num_headers; a++) {
          if(strcmp(_request_info->http_headers[a].name,name.c_str())==0){
            result=_request_info->http_headers[a].value;
            break;
          }
        }
        return result;

      }

      bool ServiceRequest::hasHeader(std::string name) {
        bool result=false;
        //LOGDEBUG("HeaderCount="<<_request_info->num_headers);
        for (int a = 0; a < _request_info->num_headers; a++) {
          //LOGDEBUG("header "<<_request_info->http_headers[a].name<<" = "<<_request_info->http_headers[a].value);
          if(strcmp(_request_info->http_headers[a].name,name.c_str())==0){
            result=true;
            break;
          }
        }
        return result;
      }

    }
  }
}

