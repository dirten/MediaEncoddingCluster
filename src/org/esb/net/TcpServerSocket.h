#ifndef ORG_ESB_NET_TCPSERVERSOCKET_H
#define ORG_ESB_NET_TCPSERVERSOCKET_H

#include <boost/asio.hpp>
#include "org/esb/util/Log.h"
#include "TcpConnectionListener.h"
using boost::asio::ip::tcp;
namespace org{
namespace esb{
namespace net{
  class TcpSocket;
  class TcpServerSocket{
    classlogger("org.esb.net.TcpServerSocket")
    public:
      TcpServerSocket(short port);
      ~TcpServerSocket();
      void bind();
      void close();
      TcpSocket * accept();
      void setConnectionListener(Ptr<TcpConnectionListener> l);
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

