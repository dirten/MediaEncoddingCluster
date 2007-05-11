#include <iostream>
#include "Socket.h"
#include "SocketInputStream.cpp"
#include "SocketOutputStream.cpp"
using namespace std;
using namespace org::esb::net;
/******************************************************************************/
Socket::Socket(int sock)
{
  this->is_closed=false;
  this->socketFd=sock;
//  Socket();
}

/******************************************************************************/
Socket::~Socket()
{
//  this->close();
}


/******************************************************************************/
Socket::Socket()
{
  this->hostname="localhost";
  this->port=0;
  this->socketFd=0;
  bzero(&this->socketaddr,sizeof(this->socketaddr));
}

/******************************************************************************/
Socket::Socket(char * hostname, int portnumber)
{
  this->hostname=hostname;
  this->port=portnumber;
  this->socketFd=0;
  bzero(&this->socketaddr,sizeof(this->socketaddr));
}

/******************************************************************************/
void Socket::setHostname(char*name)
{
  this->hostname=name;
}

/******************************************************************************/
void Socket::setPort(int p)
{
  this->port=p;
}

/******************************************************************************/
char * Socket::getHostname()
{
  return this->hostname;
}

/******************************************************************************/
int Socket::getPort()
{
  return this->port;
}

/******************************************************************************/
void Socket::init()
{
  this->socketFd=::socket(AF_INET,SOCK_STREAM,0);
  socketaddr.sin_family=AF_INET;
  socketaddr.sin_addr.s_addr=htonl(INADDR_ANY);
  socketaddr.sin_port=htons(this->port);
}

/******************************************************************************/

/******************************************************************************/
void Socket::connect()
{
  this->init();
  inet_pton(AF_INET,this->hostname,&this->socketaddr.sin_addr);
  ::connect(this->socketFd,(struct sockaddr*)&this->socketaddr,sizeof(this->socketaddr));
}

/******************************************************************************/
void Socket::close()
{
  ::close(this->socketFd);
  this->socketFd=0;
  delete this;
}
/******************************************************************************/
bool Socket::isClosed()
{
    return (this->socketFd<=0);
}

/******************************************************************************/
InputStream * Socket::getInputStream(){
  if(this->inputStream==NULL)
    this->inputStream=new SocketInputStream(this);
  return this->inputStream;
}
/******************************************************************************/

OutputStream * Socket::getOutputStream(){
  if(this->outputStream==NULL)
    this->outputStream=new SocketOutputStream(this);
  return this->outputStream;
}

/******************************************************************************/
int Socket::getDescriptor(){
  return this->socketFd; 
}

