/* 
 * File:   JSONResult.cpp
 * Author: jhoelscher
 * 
 * Created on 29. Januar 2012, 17:57
 */

#include "JSONResult.h"
#include "org/esb/lang/Exception.h"
JSONResult::JSONResult(std::string uuid):JSONNode(JSON_NODE) {
  _statusAllreadySetted=false;
  _uuid=uuid;
  JSONNode s(JSON_NODE);
  s.set_name("response");
  s.push_back(JSONNode("status","ok"));
  s.push_back(JSONNode("message",""));
  s.push_back(JSONNode("requestID",uuid));
  push_back(s);
}

void JSONResult::setData(JSONNode & data){
  data.set_name("data");
  this->push_back(data);
}

void JSONResult::setRequestId(std::string id){
  push_back(JSONNode("requestID",id));
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

