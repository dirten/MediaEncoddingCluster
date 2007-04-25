#include <iostream>
#include "org/esb/net/ServerSocket.h"
using namespace std;
using namespace org::esb::net;
/******************************************************************************/
ServerSocket::ServerSocket(int sock)
{
  this->connectFd=sock;
  ServerSocket();
}

/******************************************************************************/
ServerSocket::~ServerSocket()
{
  this->close();
}


/******************************************************************************/
ServerSocket::ServerSocket()
{
  this->hostname="localhost";
  this->port=0;
  this->socketFd=0;
  this->connectFd=0;
  bzero(&this->socketaddr,sizeof(socketaddr));
}


/******************************************************************************/
void ServerSocket::setHostname(char*name)
{
  this->hostname=name;
}

/******************************************************************************/
void ServerSocket::setPort(int p)
{
  this->port=p;
}

/******************************************************************************/
char * ServerSocket::getHostname()
{
  return this->hostname;
}

/******************************************************************************/
int ServerSocket::getPort()
{
  return port;
}


/******************************************************************************/
void ServerSocket::init()
{
  socketFd=::socket(AF_INET,SOCK_STREAM,0);
  socketaddr.sin_family=AF_INET;
  socketaddr.sin_addr.s_addr=htonl(INADDR_ANY);
  socketaddr.sin_port=htons(this->port);
}

/******************************************************************************/
void ServerSocket::bind()
{
  this->init();
  if(::bind(socketFd,(struct sockaddr*)&socketaddr, sizeof(socketaddr))<0)
  {
    perror("Bind");
    ::close(socketFd);
    exit(1);
  }
  listen(socketFd,1024);
}

/******************************************************************************/
Socket* ServerSocket::accept()
{
  sockaddr_in client;
  socklen_t clilen=0;
  bzero(&client, sizeof(client));
  connectFd=::accept(socketFd,(struct sockaddr*)&client,&clilen);
  if(connectFd<0)
  {
    ::close(connectFd);
    ::close(socketFd);
    //exit(1);
  }
  return new Socket();
}

/******************************************************************************/
void ServerSocket::close()
{
  ::close(socketFd);
  ::close(connectFd);
}
