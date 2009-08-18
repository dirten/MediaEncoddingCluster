#include "TcpServerSocket.h"
#include "TcpSocket.h"

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
namespace org {
  namespace esb {
    namespace net {

      //      typedef boost::shared_ptr<tcp::socket> socket_ptr;

      TcpServerSocket::~TcpServerSocket() {
        acceptor_.close();
      }

      TcpServerSocket::TcpServerSocket(short port)
      : acceptor_(_io_service, tcp::endpoint(tcp::v4(), port)) {
        _inShutdown = false;
      }

      TcpSocket * TcpServerSocket::accept() {
        if (_inShutdown)return NULL;
        boost::shared_ptr<tcp::socket> sock(new tcp::socket(_io_service));
        acceptor_.accept(*sock);
        boost::asio::socket_base::keep_alive option(true);
        //		sock->set_option(option);
        return new TcpSocket(sock);
      }

      void TcpServerSocket::bind() {

      }

      void TcpServerSocket::close() {
        if (acceptor_.is_open())
          acceptor_.close();
        _inShutdown = true;

      }
    }
  }
}


