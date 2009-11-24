#include "TcpServerSocket.h"
#include "TcpSocket.h"

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include "org/esb/util/Log.h"
#include "org/esb/util/Decimal.h"
namespace org {
  namespace esb {
    namespace net {

      //      typedef boost::shared_ptr<tcp::socket> socket_ptr;

      TcpServerSocket::~TcpServerSocket() {
        acceptor_.close();
      }

      TcpServerSocket::TcpServerSocket(short port)
      : _io_service(), acceptor_(_io_service) {
        try {
          boost::asio::ip::tcp::resolver resolver(_io_service);
          std::string intf = "0.0.0.0";
          std::string p = org::esb::util::Decimal(port).toString();
          boost::asio::ip::tcp::resolver::query query(intf, p);
          boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
          acceptor_.open(endpoint.protocol());
          acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
          acceptor_.bind(endpoint);
          acceptor_.listen();
        } catch (exception & ex) {
          logerror("Cannot create server Socket:" << ex.what());
        }
        _inShutdown = false;
      }

      TcpSocket * TcpServerSocket::accept() {
        if (_inShutdown)return NULL;
        boost::shared_ptr<tcp::socket> sock(new tcp::socket(_io_service));
        boost::system::error_code ec;

        acceptor_.accept(*sock, ec);
        if (ec) {
          logerror("Error while accepting Socket(" << ec.message() << ")");
          return NULL;
        }

        boost::asio::socket_base::keep_alive option(true);
        //		sock->set_option(option);
        /**
         * @TODO: check the connection before returning
         */
        TcpSocket * s = NULL;
        if (sock->is_open())
          s = new TcpSocket(sock);

        return s;
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


