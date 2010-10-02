/* 
 * File:   GridNodeServer.h
 * Author: HoelscJ
 *
 * Created on 1. Oktober 2010, 13:31
 */

#ifndef GRIDNODESERVER_H
#define	GRIDNODESERVER_H
#include "boost/asio.hpp"
#include "org/esb/grid/protocol/ProtocolDispatcher.h"
#include "org/esb/util/Log.h"
namespace org {
  namespace esb {
    namespace grid {

      class GridNodeServer {
        classlogger("org.esb.grid.GridNodeServer");
      public:
        GridNodeServer();
        virtual ~GridNodeServer();
      private:
        boost::asio::io_service recv_service_;
        boost::asio::ip::udp::endpoint recv_endpoint_;
        boost::asio::ip::udp::socket recv_socket_;
        void handle_receive(const boost::system::error_code& error, size_t bytes_recvd);
        enum {
          max_length = 50000
        };
        char data_[max_length];
        ProtocolDispatcher _dispatcher;
      };
    }
  }
}

#endif	/* GRIDNODESERVER_H */

