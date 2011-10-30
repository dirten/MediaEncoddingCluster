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
    _type = t;
    _queue = new org::esb::util::FileQueue<boost::shared_ptr<ProcessUnit> >(id);
    _max_endpoint_count=4;
  }

  Stream::~Stream() {
  }

  void Stream::enqueue(Ptr<ProcessUnit> unit) {
    _queue->enqueue(unit);
  }

  boost::shared_ptr<ProcessUnit> Stream::dequeue() {
    return _queue->dequeue();
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

  void Stream::addEndpoint(Endpoint e) {
    _endpoints.push_back(e);
  }
  
  //void Stream::removeEndpoint(Endpoint e) {
  //  _endpoints.erase(e);
  //}
  int Stream::getSize(){
    return _queue->size();
  }
  Stream::EndpointList & Stream::getEndpoints() {
    return _endpoints;
  }

  unsigned int Stream::getMaxEndpointCount() {
    return _max_endpoint_count;
  }

  void Stream::setMaxEndpointCount(unsigned int c) {
        _max_endpoint_count=c;
  }

  bool Stream::operator==(const Stream & a)const {
    return _id == a._id;
  }
}
