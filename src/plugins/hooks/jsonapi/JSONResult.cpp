/* 
 * File:   JSONResult.cpp
 * Author: jhoelscher
 * 
 * Created on 29. Januar 2012, 17:57
 */

#include "JSONResult.h"
#include "org/esb/lang/Exception.h"
JSONResult::JSONResult(org::esb::core::http::HTTPServerRequest&req):_req(req){
  _statusAllreadySetted=false;
  _uuid=_req.get("requestUUID");
  JSONNode s(JSON_NODE);
  s.set_name("response");
  s.push_back(JSONNode("status","ok"));
  s.push_back(JSONNode("message",""));
  s.push_back(JSONNode("requestID",_uuid));
  push_back(s);
  _req.response().setContentType("application/json");
}

void JSONResult::setData(JSONNode & data){
  data.set_name("data");
  this->push_back(data);
}

void JSONResult::setStatus(Poco::Net::HTTPResponse::HTTPStatus s, std::string msg){
  _req.response().setStatusAndReason(s,msg);
  if(_req.response().HTTP_OK!=s){
    setStatus("error", msg);    
  }else
    setStatus("ok", msg);
}

void JSONResult::setStatus(std::string status, std::string msg){
  if(_statusAllreadySetted){
    throw org::esb::lang::Exception("Status in JSONResult allready setted");
  }
  JSONNode s(JSON_NODE);
  s.set_name("response");
  s.push_back(JSONNode("status",status));
  s.push_back(JSONNode("message",msg));
  s.push_back(JSONNode("requestID",_uuid));
  (*this)["response"].swap(s);
  _statusAllreadySetted=true;
}

JSONResult::~JSONResult() {

}

std::string JSONResult::toString() {
  return this->write_formatted();
}

