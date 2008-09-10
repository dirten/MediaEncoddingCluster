#include "TcpSocket.h"



namespace org{
namespace esb{
namespace net{

      TcpSocket::TcpSocket(boost::asio::io_service& io_service){
      
      }
      TcpSocket::TcpSocket(boost::shared_ptr<tcp::socket> io_service){
      
      }
      tcp::socket& TcpSocket::socket(){
//        return socket_;
      }
      void TcpSocket::run(){
      
      }
}}}




