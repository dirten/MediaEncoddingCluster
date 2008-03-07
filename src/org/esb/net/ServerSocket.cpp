#include <iostream>
#include <errno.h>
#include "org/esb/net/ServerSocket.h"
#include "org/esb/lang/Exception.h"
using namespace std;
using namespace
  org::esb::net;
using namespace
  org::esb::lang;

/******************************************************************************/
ServerSocket::~ServerSocket ()
{
  this->close ();
}


/******************************************************************************/
ServerSocket::ServerSocket ()
{
  ServerSocket (0);
}

/******************************************************************************/
ServerSocket::ServerSocket (int port)
{
  this->hostname = "localhost";
  this->port = port;
  this->server_socketFd = 0;
  memset (&this->socketaddr, 0, sizeof (this->socketaddr));
}


/******************************************************************************/
void
ServerSocket::setHostname (char *name)
{
  this->hostname = name;
}

/******************************************************************************/
void
ServerSocket::setPort (int p)
{
  this->port = p;
}

/******************************************************************************/
char *
ServerSocket::getHostname ()
{
  return this->hostname;
}

/******************************************************************************/
int
ServerSocket::getPort ()
{
  return this->port;
}


/******************************************************************************/
void
ServerSocket::init ()
{
  this->server_socketFd =::socket (AF_INET, SOCK_STREAM, 0);
  socketaddr.sin_family = AF_INET;
  socketaddr.sin_addr.s_addr = htonl (INADDR_ANY);
  socketaddr.sin_port = htons (this->port);
}

/******************************************************************************/
int ServerSocket::bind()
{
  this->init ();
  if(::bind(this->server_socketFd,(struct sockaddr*)&this->socketaddr, sizeof(this->socketaddr))<0)
  {
    perror("bind");
    close();
    string error="ServerSocket:bind - ";
    error+=strerror(errno);
//    throw Exception(__FILE__, __LINE__, error.c_str() );
  }
  int err=::listen(server_socketFd,1024);

  if(err){
	perror("listen");
	return err;
  }
  return 0;
}

/******************************************************************************/
Socket *
ServerSocket::accept ()
{
//  if(server_socketFd>0){
  sockaddr_in client;
  socklen_t clilen=sizeof(sockaddr_in);
  memset(&client, 0, sizeof(client));
  int client_socketFd=::accept(this->server_socketFd,(struct sockaddr*)&client,&clilen);
  if(client_socketFd<0)
  {
    perror("accept");
    this->close();
    return NULL;
  }
  return new Socket(client_socketFd);
}

/******************************************************************************/
void
ServerSocket::close ()
{
    if(server_socketFd>0){
//    ::shutdown( this->server_socketFd, SHUT_RDWR );
    #if defined(WIN32)
	::closesocket(server_socketFd);    
    #else
	::close(server_socketFd);
    #endif
    server_socketFd=-1;
    }
}
