/* 
 * File:   Stream.cpp
 * Author: HoelscJ
 * 
 * Created on 28. Oktober 2011, 10:05
 */

#include "Stream.h"
namespace partitionservice {

  Stream::Stream() {
  }

  Stream::Stream(std::string id, TYPE t) {
    _id = id;
    _type=t;
  }

  Stream::~Stream() {
  }

  void Stream::enqueue(Ptr<ProcessUnit>) {

  }

  void Stream::setType(TYPE t) {
    _type = t;
  }

  Stream::TYPE Stream::getType() {
    return _type;
  }

  std::string Stream::getId() {
    return _id;
  }

  Stream::EndpointList Stream::getEndpoints() {
    return _endpoints;
  }

  bool Stream::operator==(const Stream & a)const {
    return _id == a._id;
  }
}
