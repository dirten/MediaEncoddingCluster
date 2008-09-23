#include "TcpSocket.h"

#include "TcpSocketInputStream.cpp"
#include "TcpSocketOutputStream.cpp"

#include "org/esb/util/Decimal.h"
namespace org{
namespace esb{
namespace net{
      
	  TcpSocket::TcpSocket(const char * host, int port){
	    _host=host;
	    _port=port;
	    _socket=boost::shared_ptr<tcp::socket>(new tcp::socket(_io_service));
	  }
      
	  TcpSocket::TcpSocket(boost::shared_ptr<tcp::socket> socket):
	    _socket(socket),
	    _is(new TcpSocketInputStream(socket)),
	    _os(new TcpSocketOutputStream(socket)){
      }

      TcpSocket::~TcpSocket(){
        delete _is;
        delete _os;
      }
      
      org::esb::io::InputStream * TcpSocket::getInputStream(){
        return _is;
      }

      org::esb::io::OutputStream * TcpSocket::getOutputStream(){
        return _os;
      }
      bool TcpSocket::isClosed(){
        return !_socket->is_open();
      }
      void TcpSocket::close(){
        _socket->close();
      }

      void TcpSocket::connect(){

	    tcp::resolver resolver(_io_service);
	    tcp::resolver::query query(tcp::v4(), _host, org::esb::util::Decimal(_port).toString().c_str());
	    tcp::resolver::iterator iterator = resolver.resolve(query);

        _socket->connect(*iterator);
        
        _is=new TcpSocketInputStream(_socket);
        _os=new TcpSocketOutputStream(_socket);
      }
}}}
