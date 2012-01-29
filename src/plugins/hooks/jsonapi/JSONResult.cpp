/* 
 * File:   JSONResult.cpp
 * Author: jhoelscher
 * 
 * Created on 29. Januar 2012, 17:57
 */

#include "JSONResult.h"

JSONResult::JSONResult():JSONNode(JSON_NODE) {

}

void JSONResult::setData(JSONNode & data){
  this->push_back(data);
}

void JSONResult::setRequestId(std::string id){
  push_back(JSONNode("requestID",id));
}

void JSONResult::setStatus(std::string status, std::string msg){
  JSONNode s(JSON_NODE);
  s.set_name("status");
  s.push_back(JSONNode("status",status));
  s.push_back(JSONNode("message",msg));
  push_back(s);
}

JSONResult::~JSONResult() {
}

std::string JSONResult::toString() {
  return this->write_formatted();
}

