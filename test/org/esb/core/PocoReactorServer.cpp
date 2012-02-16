#include "Poco/Net/Net.h"
#include "Poco/Net/SocketNotification.h"
#include "Poco/Net/SocketReactor.h"

#include "Poco/Net/SocketConnector.h"
#include "Poco/Net/SocketAcceptor.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Observer.h"

#include <iostream>
#include <sstream>
#include <assert.h>
#include "org/esb/lang/Thread.h"
using Poco::Net::SocketReactor;
using Poco::Net::SocketConnector;
using Poco::Net::SocketAcceptor;
using Poco::Net::StreamSocket;
using Poco::Net::ServerSocket;
using Poco::Net::SocketAddress;
using Poco::Net::SocketNotification;
using Poco::Net::ReadableNotification;
using Poco::Net::WritableNotification;
using Poco::Net::TimeoutNotification;
using Poco::Net::ShutdownNotification;
using Poco::Observer;

class EchoServiceHandler
{
public:
  EchoServiceHandler(StreamSocket& socket, SocketReactor& reactor):
    _socket(socket),
    _reactor(reactor)
  {
    _reactor.addEventHandler(_socket, Observer<EchoServiceHandler, ReadableNotification>(*this, &EchoServiceHandler::onReadable));
    _reactor.addEventHandler(_socket, Observer<EchoServiceHandler, WritableNotification>(*this, &EchoServiceHandler::onWritable));
  }

  ~EchoServiceHandler()
  {
    _reactor.removeEventHandler(_socket, Observer<EchoServiceHandler, ReadableNotification>(*this, &EchoServiceHandler::onReadable));
    _reactor.removeEventHandler(_socket, Observer<EchoServiceHandler, WritableNotification>(*this, &EchoServiceHandler::onWritable));
  }

  void onReadable(ReadableNotification* pNf)
  {
    pNf->release();
    char buffer[8];
    int n = _socket.receiveBytes(buffer, sizeof(buffer));
    if (n > 0)
      {
        _socket.sendBytes(buffer, n);
      }
    else
      {
        _socket.shutdownSend();
        delete this;
      }
  }
  void onWritable(WritableNotification* pNf)
  {

  }

private:
  StreamSocket   _socket;
  SocketReactor& _reactor;
};






class ClientServiceHandler
{
public:
  ClientServiceHandler(StreamSocket& socket, SocketReactor& reactor):
    _socket(socket),
    _reactor(reactor)
  {
    std::cout << "ClientServiceHandler(StreamSocket& socket, SocketReactor& reactor):"<<std::endl;
    _timeout = false;
    _reactor.addEventHandler(_socket, Observer<ClientServiceHandler, ReadableNotification>(*this, &ClientServiceHandler::onReadable));
    _reactor.addEventHandler(_socket, Observer<ClientServiceHandler, WritableNotification>(*this, &ClientServiceHandler::onWritable));
    _reactor.addEventHandler(_socket, Observer<ClientServiceHandler, TimeoutNotification>(*this, &ClientServiceHandler::onTimeout));
  }

  ~ClientServiceHandler()
  {
  }

  void onReadable(ReadableNotification* pNf)
  {
    pNf->release();
    char buffer[32];
    int n = _socket.receiveBytes(buffer, sizeof(buffer));
    if (n > 0)
      {
        _str.write(buffer, n);
      }
    else
      {
        _reactor.removeEventHandler(_socket, Observer<ClientServiceHandler, ReadableNotification>(*this, &ClientServiceHandler::onReadable));
        _reactor.stop();
        _data = _str.str();
        delete this;
      }
  }

  void onWritable(WritableNotification* pNf)
  {
    std::cout << "void onWritable(WritableNotification* pNf)"<<std::endl;
    pNf->release();
    _reactor.removeEventHandler(_socket, Observer<ClientServiceHandler, WritableNotification>(*this, &ClientServiceHandler::onWritable));
    std::string data(1024, 'x');
    _socket.sendBytes(data.data(), (int) data.length());
    _socket.shutdownSend();
  }

  void onTimeout(TimeoutNotification* pNf)
  {
    pNf->release();
    _timeout = true;
    if (_closeOnTimeout)
      {
        _reactor.stop();
        delete this;
      }
  }

  static std::string data()
  {
    return _data;
  }

  static bool timeout()
  {
    return _timeout;
  }

  static bool getCloseOnTimeout()
  {
    return _closeOnTimeout;
  }

  static void setCloseOnTimeout(bool flag)
  {
    _closeOnTimeout = flag;
  }

private:
  StreamSocket       _socket;
  SocketReactor&     _reactor;
  std::stringstream  _str;
  static std::string _data;
  static bool        _timeout;
  static bool        _closeOnTimeout;
};


std::string ClientServiceHandler::_data;
bool ClientServiceHandler::_timeout = false;
bool ClientServiceHandler::_closeOnTimeout = false;







int main(int argc, char** argv){
  SocketAddress ssa;
  ServerSocket ss(ssa);
  SocketReactor reactor;
  SocketAcceptor<EchoServiceHandler> acceptor(ss, reactor);


  SocketReactor reactor2;
  SocketAddress sa("localhost", ss.address().port());
  SocketConnector<ClientServiceHandler> connector(sa, reactor2);
  reactor2.run();
  reactor.run();
  std::string data(ClientServiceHandler::data());
  assert (data.size() == 1024);
  std::cout << data<<std::endl;
  data=(ClientServiceHandler::data());
  assert (data.size() == 1024);
  std::cout << data<<std::endl;
  org::esb::lang::Thread::sleep2(10*1000);
  reactor.stop();


}


