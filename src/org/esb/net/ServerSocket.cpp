#include <iostream>
#include "org/esb/net/ServerSocket.h"
#include "org/esb/lang/Exception.h"
using namespace std;
using namespace org::esb::net;
using namespace org::esb::lang;

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
  memset(&this->socketaddr, 0, sizeof(this->socketaddr));
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
    close();
    throw new Exception(__FILE__, __LINE__, "ServerSocket:bind -" );
//    exit(1);
  }
  listen(server_socketFd,1024);
}

/******************************************************************************/
Socket* ServerSocket::accept()
{
  sockaddr_in client;
  socklen_t clilen=0;
  memset(&client, 0, sizeof(client));
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
    #if defined(WIN32)
	::closesocket(server_socketFd);    
    #else
	::close(server_socketFd);
    #endif
}
