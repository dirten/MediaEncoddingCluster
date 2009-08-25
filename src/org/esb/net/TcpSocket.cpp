#include "TcpSocket.h"

#include "TcpSocketInputStream.cpp"
#include "TcpSocketOutputStream.cpp"

#include "org/esb/util/Decimal.h"
namespace org {
  namespace esb {
    namespace net {
		boost::mutex net_io_mutex;
      TcpSocket::TcpSocket(const char * host, int port) {
        _connected = false;
        _host = host;
       _port = port;
//        boost::asio::io_service _io_service;

        _socket = boost::shared_ptr<tcp::socket > (new tcp::socket(_io_service));
      }

      TcpSocket::TcpSocket(boost::shared_ptr<tcp::socket> socket) :
      _socket(socket),
      _is(new TcpSocketInputStream(socket,net_io_mutex)),
      _os(new TcpSocketOutputStream(socket,net_io_mutex)) {
		  _connected = true;
      }

      TcpSocket::~TcpSocket() {
//        delete _is;
//        delete _os;
      }

      org::esb::io::InputStream * TcpSocket::getInputStream() {
        return _is;
      }

      org::esb::io::OutputStream * TcpSocket::getOutputStream() {
        return _os;
      }

      bool TcpSocket::isClosed() {
        return !_socket->is_open();
      }

      bool TcpSocket::isConnected() {
        return _connected;
      }

      void TcpSocket::close() {
        _socket->close();
        _connected = false;
      }

      void TcpSocket::connect() {

        tcp::resolver resolver(_io_service);
        tcp::resolver::query query(tcp::v4(), _host, org::esb::util::Decimal(_port).toString().c_str());
        tcp::resolver::iterator iterator = resolver.resolve(query);

        _socket->connect(*iterator);

        _is = new TcpSocketInputStream(_socket,net_io_mutex);
        _os = new TcpSocketOutputStream(_socket,net_io_mutex);
        _connected = true;
      }
      std::string TcpSocket::getRemoteIpAddress(){
        return _socket->remote_endpoint().address().to_string();
      }

    }
  }
}
