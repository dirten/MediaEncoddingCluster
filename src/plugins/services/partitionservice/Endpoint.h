/* 
 * File:   Endpoint.h
 * Author: HoelscJ
 *
 * Created on 28. Oktober 2011, 11:35
 */

#ifndef ENDPOINT_H
#define	ENDPOINT_H

#include <boost/asio/ip/tcp.hpp>
#include "exports.h"

namespace partitionservice {

  class PARTSERVICE_EXPORT Endpoint {
  public:
    Endpoint(boost::asio::ip::tcp::endpoint);
    virtual ~Endpoint();
    bool operator==(const Endpoint&a)const;
    bool operator<(const Endpoint&a)const;
    bool operator>(const Endpoint&a)const;
  private:
    boost::asio::ip::tcp::endpoint _ep;

  };
}
#endif	/* ENDPOINT_H */

