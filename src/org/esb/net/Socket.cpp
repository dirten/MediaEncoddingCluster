#include <iostream>
#include "Socket.h"
#include "SocketInputStream.cpp"
#include "SocketOutputStream.cpp"
using namespace std;
using namespace org::esb::net;
/******************************************************************************/
Socket::Socket(int sock)
{
//  Socket();
  this->is_closed=false;
  this->socketFd=sock;

  this->inputStream=new SocketInputStream(this);
  this->outputStream=new SocketOutputStream(this);

}

/******************************************************************************/
Socket::~Socket()
{
    delete this->inputStream;
    delete this->outputStream;
}


/******************************************************************************/
Socket::Socket()
{
  cout << "default Constructor from Socket"<<endl;
  this->hostname="localhost";
  this->port=0;
  this->socketFd=0;
  memset(&this->socketaddr, 0, sizeof(this->socketaddr));

  this->inputStream=new SocketInputStream(this);
  this->outputStream=new SocketOutputStream(this);

}

/******************************************************************************/
Socket::Socket(char * hostname, int portnumber)
{
  this->hostname=hostname;
  this->port=portnumber;
  this->socketFd=0;
  memset(&this->socketaddr, 0, sizeof(this->socketaddr));

  this->inputStream=new SocketInputStream(this);
  this->outputStream=new SocketOutputStream(this);
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
//  inet_pton(AF_INET,this->hostname,&this->socketaddr.sin_addr);
  ::connect(this->socketFd,(struct sockaddr*)&this->socketaddr,sizeof(this->socketaddr));
  
}

/******************************************************************************/
void Socket::close()
{
    #if defined(WIN32)
	::closesocket(this->socketFd);    
    #else
	::close(this->socketFd);
    #endif

 // ::close(this->socketFd);
  this->socketFd=0;
}
/******************************************************************************/
bool Socket::isClosed()
{
    return (this->socketFd<=0);
}

/******************************************************************************/
InputStream * Socket::getInputStream(){
  return this->inputStream;
}
/******************************************************************************/

OutputStream * Socket::getOutputStream(){
  return this->outputStream;
}

/******************************************************************************/
int Socket::getDescriptor(){
  return this->socketFd; 
}

