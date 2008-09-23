#ifndef ORG_ESB_NET_TCPSOCKET_H
#define ORG_ESB_NET_TCPSOCKET_H

#include "org/esb/io/InputStream.h"
#include "org/esb/io/OutputStream.h"

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
using boost::asio::ip::tcp; 

namespace org{
namespace esb{
namespace net{
  class TcpSocket{
    public:  
      TcpSocket(const char * host, int port);
      TcpSocket(boost::shared_ptr<tcp::socket> io_service);
      ~TcpSocket();
      org::esb::io::InputStream * getInputStream();
      org::esb::io::OutputStream * getOutputStream();
      bool isClosed();
      void close();
      void connect();
    private:
		boost::shared_ptr<tcp::socket> _socket;
        boost::asio::io_service _io_service;
		
		org::esb::io::InputStream * _is;
		org::esb::io::OutputStream * _os;
        
        const char * _host;
        int _port;
  };
}}}
#endif

