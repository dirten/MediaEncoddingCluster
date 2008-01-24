#include <iostream>
#include "Socket.h"
#include "SocketInputStream.cpp"
#include "SocketOutputStream.cpp"
using namespace std;
using namespace org::esb::net;
using namespace org::esb::lang;
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
    this->close();
    if(this->inputStream!=NULL){
	delete this->inputStream;
	this->inputStream=NULL;
    }
    if(this->outputStream!=NULL){
	delete this->outputStream;
	this->outputStream=NULL;
    }

}


/******************************************************************************/
Socket::Socket()
{
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
  inet_pton(AF_INET,this->hostname,&this->socketaddr.sin_addr);
  ::connect(this->socketFd,(struct sockaddr*)&this->socketaddr,sizeof(this->socketaddr));
  
}

/******************************************************************************/
void Socket::close()
{
    if(!isClosed()){
    /*
    if(this->inputStream!=NULL){
	delete this->inputStream;
	this->inputStream=NULL;
    }
    if(this->outputStream!=NULL){
	delete this->outputStream;
	this->outputStream=NULL;
    }*/
    if(!isClosed()){
//	::shutdown( this->socketFd, SHUT_RDWR );
    #if defined(WIN32)
	::closesocket(this->socketFd);    
    #else
	::close(this->socketFd);
    #endif
    }
  this->socketFd=-1;
  }
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

