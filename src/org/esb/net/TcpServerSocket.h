#ifndef ORG_ESB_NET_TCPSERVERSOCKET_H
#define ORG_ESB_NET_TCPSERVERSOCKET_H

#include <boost/asio.hpp>
using boost::asio::ip::tcp; 
namespace org{
namespace esb{
namespace net{
  class TcpSocket;
  class TcpServerSocket{
    public:
      TcpServerSocket(boost::asio::io_service& io_service, short port);
      TcpSocket * accept();
    private:
      boost::asio::io_service& io_service_;
      tcp::acceptor acceptor_;
      void handle_accept(TcpSocket* new_session,const boost::system::error_code& error);

  };
}}}
#endif

