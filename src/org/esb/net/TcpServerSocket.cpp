#include "TcpServerSocket.h"
#include "TcpSocket.h"

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include "org/esb/util/Log.h"
namespace org {
  namespace esb {
    namespace net {

      //      typedef boost::shared_ptr<tcp::socket> socket_ptr;

      TcpServerSocket::~TcpServerSocket() {
        acceptor_.close();
      }

      TcpServerSocket::TcpServerSocket(short port)
      : acceptor_(_io_service, tcp::endpoint(tcp::v4(), port)) {
        boost::asio::socket_base::enable_connection_aborted option(true);
        acceptor_.set_option(option);

        _inShutdown = false;
      }

      TcpSocket * TcpServerSocket::accept() {
        if (_inShutdown)return NULL;
        boost::shared_ptr<tcp::socket> sock(new tcp::socket(_io_service));
        boost::system::error_code ec;

        acceptor_.accept(*sock, ec);
        if (ec) {
          ec.message();
          logerror("Error while accepting Socket("<<ec.message()<<")");
          return NULL;
        }

        boost::asio::socket_base::keep_alive option(true);
        //		sock->set_option(option);
        return new TcpSocket(sock);
      }

      void TcpServerSocket::bind() {

      }

      void TcpServerSocket::close() {
        _io_service.stop();
//        if (acceptor_.is_open()) {
//          acceptor_.cancel();
//          acceptor_.close();
//          logdebug("TcpServerSocket closed");
//        }
        _inShutdown = true;

      }
    }
  }
}


