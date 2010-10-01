/* 
 * File:   GridNodeConsole.h
 * Author: HoelscJ
 *
 * Created on 1. Oktober 2010, 13:50
 */

#ifndef GRIDNODECONSOLE_H
#define	GRIDNODECONSOLE_H
#include "boost/asio.hpp"
namespace org {
  namespace esb {
    namespace grid {

      class GridNodeConsole {
      public:
        GridNodeConsole();
        virtual ~GridNodeConsole();
      private:
        boost::asio::io_service send_service_;

        boost::asio::ip::udp::endpoint send_endpoint_;

        boost::asio::ip::udp::socket send_socket_;
        void handle_send(const boost::system::error_code& error,std::size_t bytes_transferred);

      };
    }
  }
}

#endif	/* GRIDNODECONSOLE_H */

