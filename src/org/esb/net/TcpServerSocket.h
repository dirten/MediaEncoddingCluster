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
      TcpServerSocket(short port);
      ~TcpServerSocket();
      void bind();
      void close();
      TcpSocket * accept();
    private:
      TcpServerSocket();
//      boost::asio::io_service& io_service_;
      boost::asio::io_service _io_service;
      boost::asio::ip::tcp::acceptor acceptor_;
//      boost::asio::ip::tcp::socket socket_;
      void handle_accept(TcpSocket* new_session,const boost::system::error_code& error);
      bool _inShutdown;
  };
}}}
#endif

