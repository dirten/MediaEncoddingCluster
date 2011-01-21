#ifndef ORG_ESB_NET_TCPSOCKET_H
#define ORG_ESB_NET_TCPSOCKET_H

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "org/esb/io/InputStream.h"
#include "org/esb/io/OutputStream.h"

#include <boost/shared_ptr.hpp>
//using boost::asio::ip::tcp;
#include "org/esb/util/Log.h"
namespace org {
    namespace esb {
        namespace net {

            class TcpSocket {
              classlogger("org.esb.net.TcpSocket")
            public:
                TcpSocket(std::string host, int port);
                TcpSocket(boost::shared_ptr<boost::asio::ip::tcp::socket> io_service);
                ~TcpSocket();
                org::esb::io::InputStream * getInputStream();
                org::esb::io::OutputStream * getOutputStream();
                bool isClosed();
                bool isConnected();
                void close();
                void connect();
                std::string getRemoteIpAddress();
                boost::asio::ip::tcp::endpoint getRemoteEndpoint();
            private:
                boost::asio::io_service _io_service;
                boost::shared_ptr<boost::asio::ip::tcp::socket> _socket;
                boost::mutex net_io_mutex;

                org::esb::io::InputStream * _is;
                org::esb::io::OutputStream * _os;

                std::string _host;
                int _port;
                bool _connected;
            };
        }
    }
}
#endif

