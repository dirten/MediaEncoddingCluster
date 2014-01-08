#include "TcpSocket.h"

#include "TcpSocketInputStream.cpp"
#include "TcpSocketOutputStream.cpp"

#include "org/esb/util/StringUtil.h"
#include "org/esb/util/Log.h"
namespace org {
  namespace esb {
    namespace net {

      TcpSocket::TcpSocket(std::string host, int port) {
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
        if(!isConnected())
          throw SocketException("Socket is not connected");
        return _is;
      }

      org::esb::io::OutputStream * TcpSocket::getOutputStream() {
        if(!isConnected())
          throw SocketException("Socket is not connected");
        return _os;
      }

      bool TcpSocket::isClosed() {
        return !_socket->is_open();
      }

      bool TcpSocket::isConnected() {
        return _connected&&_socket->is_open();
      }

      void TcpSocket::close() {
        if (_connected&&_socket.get() && _socket->is_open()) {
          boost::system::error_code ec;
          try {
            _socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
            _socket->close(ec);
          }catch (exception & ex) {
            LOGERROR("while shutdown socket:" << ex.what());
          }
        }
        _connected = false;
      }

      void TcpSocket::connect() {
        if (!_connected) {
          tcp::resolver resolver(_io_service);
          tcp::resolver::query query(_host, org::esb::util::StringUtil::toString(_port).c_str());
          tcp::resolver::iterator iterator = resolver.resolve(query);
          tcp::resolver::iterator end;
          //		  asio::error e=boost::asio::error::host_not_found;
          boost::system::error_code e = boost::asio::error::host_not_found;
          while (e && iterator != end) {
            LOGDEBUG("EndPoint to connect to:" << iterator->endpoint());
            _socket->close();
            _socket->connect(*iterator++, e);
            LOGDEBUG("Socket Message:" << e.message());
            LOGDEBUG("Socket status:" << e);
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
