#ifndef ORG_ESB_NET_TCPSOCKET_H
#define ORG_ESB_NET_TCPSOCKET_H

#include <boost/thread.hpp>

#include "org/esb/io/InputStream.h"
#include "org/esb/io/OutputStream.h"

#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
//using boost::asio::ip::tcp;

namespace org {
    namespace esb {
        namespace net {

            class TcpSocket {
            public:
                TcpSocket(const char * host, int port);
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

                const char * _host;
                int _port;
                bool _connected;
            };
        }
    }
}
#endif

