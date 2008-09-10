#ifndef ORG_ESB_NET_TCPSOCKET_H
#define ORG_ESB_NET_TCPSOCKET_H

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
using boost::asio::ip::tcp; 

namespace org{
namespace esb{
namespace net{
  class TcpSocket{
    public:  
      TcpSocket(boost::asio::io_service& io_service);
      TcpSocket(boost::shared_ptr<tcp::socket> io_service);
//      tcp::socket& socket();
      void run();
    private:
		boost::shared_ptr<tcp::socket> _io_service;
//      tcp::socket socket_;
  };
}}}
#endif

