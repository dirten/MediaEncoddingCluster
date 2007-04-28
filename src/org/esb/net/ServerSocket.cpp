#include <iostream>
#include "org/esb/net/ServerSocket.h"
using namespace std;
using namespace org::esb::net;

/******************************************************************************/
ServerSocket::~ServerSocket()
{
  this->close();
}


/******************************************************************************/
ServerSocket::ServerSocket()
{
    ServerSocket(0);
}
/******************************************************************************/
ServerSocket::ServerSocket(int port)
{
  this->hostname="localhost";
  this->port=port;
  this->server_socketFd=0;
//  this->client_socketFd=0;
  bzero(&this->socketaddr,sizeof(this->socketaddr));
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
  return this->port;
}


/******************************************************************************/
void ServerSocket::init()
{
  this->server_socketFd=::socket(AF_INET,SOCK_STREAM,0);
  socketaddr.sin_family=AF_INET;
  socketaddr.sin_addr.s_addr=htonl(INADDR_ANY);
  socketaddr.sin_port=htons(this->port);
}

/******************************************************************************/
void ServerSocket::bind()
{
  this->init();
  if(::bind(this->server_socketFd,(struct sockaddr*)&this->socketaddr, sizeof(this->socketaddr))<0)
  {
    perror("Bind");
    ::close(server_socketFd);
    exit(1);
  }
  listen(server_socketFd,1024);
}

/******************************************************************************/
Socket* ServerSocket::accept()
{
  sockaddr_in client;
  socklen_t clilen=0;
  bzero(&client, sizeof(client));
  int client_socketFd=::accept(this->server_socketFd,(struct sockaddr*)&client,&clilen);
  if(client_socketFd<0)
  {
    this->close();
  }
  return new Socket(client_socketFd);
}

/******************************************************************************/
void ServerSocket::close()
{
  ::close(this->server_socketFd);
//  ::close(this->client_socketFd);
}
