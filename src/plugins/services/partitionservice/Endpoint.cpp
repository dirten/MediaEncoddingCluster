/* 
 * File:   Endpoint.cpp
 * Author: HoelscJ
 * 
 * Created on 28. Oktober 2011, 11:35
 */

#include "Endpoint.h"
namespace partitionservice {

  Endpoint::Endpoint(boost::asio::ip::tcp::endpoint ep) {
    _ep = ep;
  }

  Endpoint::~Endpoint() {
  }

  bool Endpoint::operator==(const Endpoint & a)const {
    return _ep == a._ep;
  }

  bool Endpoint::operator<(const Endpoint&a)const {
    return _ep < a._ep;
  }

  bool Endpoint::operator>(const Endpoint&a)const {
    return _ep > a._ep;

  }

}
