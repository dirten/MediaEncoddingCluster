/* 
 * File:   Partition.cpp
 * Author: HoelscJ
 * 
 * Created on 28. Oktober 2011, 10:04
 */

#include "Partition.h"
#include "org/esb/util/Foreach.h"
namespace partitionservice {

  Partition::Partition(std::string id) {
    _id = id;
  }

  Partition::~Partition() {
  }

  void Partition::addStream(Stream s) {
    _streams.push_back(s);
  }

  bool Partition::containStream(std::string id) {
    bool result = false;

    foreach(Stream s, _streams) {
      if (s.getId() == id)
        result = true;
    }
    return result;

  }

  Stream Partition::getStream(std::string id) {
    Stream result;

    foreach(Stream s, _streams) {
      if (s.getId() == id)
        result = s;
    }
    return result;
  }

  std::string Partition::getId() {
    return _id;
  }

  void Partition::addEndpoint(Endpoint e) {
    _endpoints.push_back(e);
  }

  void Partition::removeEndpoint(Endpoint e) {
    _endpoints.remove(e);
  }


  Partition::EndpointList Partition::getEndpoints() {
    return _endpoints;
  }
}
