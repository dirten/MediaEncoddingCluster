/* 
 * File:   UdpServerSocket.cpp
 * Author: HoelscJ
 * 
 * Created on 1. Oktober 2010, 11:53
 */

#include "UdpServerSocket.h"
namespace org {
  namespace esb {
    namespace net {

      UdpServerSocket::UdpServerSocket(int port):
            recv_socket_(recv_service_) {
      }

      UdpServerSocket::~UdpServerSocket() {
      }
      void UdpServerSocket::startListen() {

      }

      void UdpServerSocket::setDataListener() {
      }
    }
  }
}
