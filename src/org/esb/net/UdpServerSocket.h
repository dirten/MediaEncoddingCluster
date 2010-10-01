/* 
 * File:   UdpServerSocket.h
 * Author: HoelscJ
 *
 * Created on 1. Oktober 2010, 11:53
 */

#ifndef UDPSERVERSOCKET_H
#define	UDPSERVERSOCKET_H
#include "boost/asio.hpp"
namespace org {
  namespace esb {
    namespace net {

      class UdpServerSocket {
      public:
        UdpServerSocket(int port);
//        UdpServerSocket(const UdpServerSocket& orig);
        virtual ~UdpServerSocket();
        void setDataListener();
        void startListen();
      private:
        boost::asio::io_service recv_service_;
        boost::asio::ip::udp::endpoint recv_endpoint_;
        boost::asio::ip::udp::socket recv_socket_;

      };
    }
  }
}

#endif	/* UDPSERVERSOCKET_H */

