
#include "org/esb/net/TcpSocket.h"
int main(){
//  boost::asio::io_service io_service;


  org::esb::net::TcpSocket c("localhost", 3306);
  c.connect();
}
