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
      ServiceOutputStream::ServiceOutputStream(mg_connection *conn):_conn(conn){
        
      }
      ServiceOutputStream::~ServiceOutputStream(){
        
      }
      
      int ServiceOutputStream::write(std::string data){
        mg_write(_conn, data.c_str(), data.length());
      }
      
      ServiceResponse::ServiceResponse(mg_connection *conn, const mg_request_info *request_info) {
         _conn = conn;
        _request_info = request_info;

        _outputstream=new ServiceOutputStream(conn);
        _status=0;
      }

      ServiceResponse::~ServiceResponse() {
        delete _outputstream;
      }
      
      ServiceOutputStream * ServiceResponse::getOutputStream(){
        return _outputstream;
      }
      
      void ServiceResponse::setStatus(int st){
        _status=st;
      }
    }
  }
}
