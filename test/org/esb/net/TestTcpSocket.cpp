
#include "org/esb/net/TcpSocket.h"
#include "org/esb/net/TcpServerSocket.h"
#include <boost/thread.hpp>

#include "org/esb/util/Log.h"
using namespace org::esb::net;

void server_socket(){
  TcpServerSocket * server=new TcpServerSocket(20200);
  TcpSocket * client=server->accept();
  std::string s;
  client->getInputStream()->read(s);
  client->getOutputStream()->write(s);
  delete client;
  delete server;
}

void testSimpleEcho(){
  boost::thread t(&server_socket);
  org::esb::net::TcpSocket c("localhost", 20200);
  c.connect();
  std::string w="bla fasel";
  c.getOutputStream()->write(w);
  std::string r;
  c.getInputStream()->read(r);
  assert(r==w);
}
void testBigData(){
  int data_size=1024*1024*20;
  char * data=new char[data_size];
  memset(data,0,data_size);

  boost::thread t(&server_socket);
  org::esb::net::TcpSocket c("localhost", 20200);
  c.connect();
//  std::string w="bla fasel";
  c.getOutputStream()->write(data, data_size);
  std::string r;
  c.getInputStream()->read(r);
  std::cout << "StreamLength:"<<r.size()<<endl;
  delete []data;
//  assert(r==w);

}
int main(){
  testSimpleEcho();
  testBigData();
}
