#include <iostream>
#include "Socket.h"
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
  this->close();
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
int Socket::write(SocketData * data)
{
  return this->write((unsigned char *)data->data, data->data_length);
}

/******************************************************************************/
int Socket::write(const unsigned char * buffer, int len)
{
  int remaining=len, byteCounter=0, sendOpts = SOCKET_NOSIGNAL;
  char * length=new char[64];
  sprintf(length,"%d", len);
  if((::send(this->socketFd,length,64,sendOpts))<0){
    this->close();
  }
  while(remaining>0)
  {
    int bytes=::send(this->socketFd,buffer,remaining,sendOpts);
    byteCounter+=bytes;
    if(bytes<0)
    {
      this->close();
      return bytes;
    }
    buffer+=bytes;
    remaining-=bytes;
  }
  return byteCounter;
}

/******************************************************************************/
SocketData* Socket::read()
{
  char*bytes_str=new char[64];
  if((::read(this->socketFd,bytes_str,64))<0){
    this->close();
  }
  int bytes=atoi(bytes_str),counter=0,offset=0,all=0;
  unsigned int rest=bytes;
  char recvBuffer[8192];
  
  char*frame=new char[bytes];
  while(all<bytes)
  {
    int maxrecv=rest>sizeof(recvBuffer)?sizeof(recvBuffer):rest;
    counter=::read(this->socketFd,recvBuffer,maxrecv);
    /*Connection is dead*/
    if(counter<0){
	this->close();
	return false;
    }
    memcpy(frame+offset,recvBuffer,counter);
    offset+=counter;
    rest-=counter;
    all+=counter;
  }
  SocketData * packet=new SocketData();
  packet->data=frame;
  packet->data_length=all;
  return packet;
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
}
/******************************************************************************/
bool Socket::isClosed()
{
    return (this->socketFd<=0);
}
