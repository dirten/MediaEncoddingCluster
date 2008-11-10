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

      }

      TcpSocket * TcpServerSocket::accept() {
        boost::shared_ptr<tcp::socket> sock(new tcp::socket(_io_service));
        acceptor_.accept(*sock);
        
        return new TcpSocket(sock);;
      }

            /*
          void TcpServerSocket::handle_accept(TcpSocket* new_session,const boost::system::error_code& error){
      
          }
       */
      void TcpServerSocket::bind() {

      }

      void TcpServerSocket::close() {
        acceptor_.close();
      }
    }
  }
}


