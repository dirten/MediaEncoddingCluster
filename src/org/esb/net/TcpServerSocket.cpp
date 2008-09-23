#include "TcpServerSocket.h"
#include "TcpSocket.h"

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
namespace org{
namespace esb{
namespace net{

//      typedef boost::shared_ptr<tcp::socket> socket_ptr;


      TcpServerSocket::TcpServerSocket(short port)
      : acceptor_(_io_service, tcp::endpoint(tcp::v4(), port)){
      
      }
      
      TcpSocket * TcpServerSocket::accept(){
      
        boost::shared_ptr<tcp::socket> sock(new tcp::socket(_io_service));
        acceptor_.accept(*sock);
        TcpSocket * socket=new TcpSocket(sock);
//        boost::thread t(boost::bind(&TcpSocket::run,socket));
        /*
        TcpSocket* new_session = new TcpSocket(io_service_);
        acceptor_.async_accept(new_session->socket(),
                    boost::bind(&TcpServerSocket::handle_accept, this, new_session, boost::asio::placeholders::error));

        */
        return socket;
      }
/*
    void TcpServerSocket::handle_accept(TcpSocket* new_session,const boost::system::error_code& error){
      
    }
*/
    void TcpServerSocket::bind(){
      
    }
    void TcpServerSocket::close(){
      
    }

}}}


