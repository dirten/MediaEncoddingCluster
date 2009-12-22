#include "TcpSocket.h"

#include "TcpSocketInputStream.cpp"
#include "TcpSocketOutputStream.cpp"

#include "org/esb/util/Decimal.h"
namespace org {
  namespace esb {
    namespace net {

      TcpSocket::TcpSocket(const char * host, int port) {
        _connected = false;
        _host = host;
        _port = port;
        //        boost::asio::io_service _io_service;
        _is = NULL;
        _os = NULL;
        _socket = boost::shared_ptr<tcp::socket > (new tcp::socket(_io_service));
      }

      TcpSocket::TcpSocket(boost::shared_ptr<tcp::socket> socket) :
      _socket(socket),
      _is(new TcpSocketInputStream(socket, net_io_mutex)),
      _os(new TcpSocketOutputStream(socket, net_io_mutex)) {
        _connected = true;
      }

      TcpSocket::~TcpSocket() {
        close();
        if (_is)
          delete _is;
        if (_os)
          delete _os;
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
        if (_socket.get() && _socket->is_open()) {
          try {
            //            _socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both);
          }          catch (exception & ex) {
            logerror("while shutdown socket:" << ex.what());
          }
          _socket->close();
        }
        _connected = false;
      }

      void TcpSocket::connect() {
        if (!_connected) {
          tcp::resolver resolver(_io_service);
          tcp::resolver::query query(_host, org::esb::util::Decimal(_port).toString().c_str());
          tcp::resolver::iterator iterator = resolver.resolve(query);
          tcp::resolver::iterator end;
          //		  asio::error e=boost::asio::error::host_not_found;
          boost::system::error_code e = boost::asio::error::host_not_found;
          while (e && iterator != end) {
//            logdebug("EndPoint to connect to:" << iterator->endpoint());
            _socket->close();
            _socket->connect(*iterator++, e);
//            logdebug("Socket Message:" << e.message());
//            logdebug("Socket status:" << e);

          }
          if (e) {
            _socket->close();
            throw boost::system::system_error(e);
          }

          //          _socket->connect(*iterator);

          _is = new TcpSocketInputStream(_socket, net_io_mutex);
          _os = new TcpSocketOutputStream(_socket, net_io_mutex);
          _connected = true;
        }
      }

      std::string TcpSocket::getRemoteIpAddress() {
        return _socket->remote_endpoint().address().to_string();
      }

      boost::asio::ip::tcp::endpoint TcpSocket::getRemoteEndpoint() {
        return _socket->remote_endpoint();
      }
    }
  }
}
