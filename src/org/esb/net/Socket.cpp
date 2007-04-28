#include <iostream>
#include "Socket.h"
using namespace std;
using namespace org::esb::net;
/******************************************************************************/
Socket::Socket(int sock)
{
  this->connectFd=sock;
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
  this->connectFd=0;
  bzero(&this->socketaddr,sizeof(socketaddr));
}

/******************************************************************************/
Socket::Socket(char * hostname, int portnumber)
{
  this->hostname=hostname;
  this->port=portnumber;
  this->socketFd=0;
  this->connectFd=0;
  bzero(&socketaddr,sizeof(socketaddr));
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
  return port;
}

/******************************************************************************/
int Socket::write(SocketData * data)
{
  return this->write((unsigned char *)data->data, data->data_length);
}

/******************************************************************************/
int Socket::write(const unsigned char * buffer, int len)
{
  int remaining=len, byteCounter=0;;
  int sendOpts = SOCKET_NOSIGNAL;
  char * length=new char[64];
  sprintf(length,"%d", len);
  ::send(this->connectFd,length,64,sendOpts);
  while(remaining>0)
  {
    int bytes=::send(this->connectFd,buffer,remaining,sendOpts);
    //  cout << "Bytes Sendet:"<<bytes<<endl;
    byteCounter+=bytes;
    if(bytes<0)
    {
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
  ::read(this->connectFd,bytes_str,64);
  int bytes=atoi(bytes_str);
  int counter=0;
  char recvBuffer[8192];
  int offset=0;
  unsigned int rest=bytes;
  int all=0;
  char*frame=new char[bytes];
  while(all<bytes)
  {
    int maxrecv=rest>sizeof(recvBuffer)?sizeof(recvBuffer):rest;
    counter=::read(this->connectFd,recvBuffer,maxrecv);
//    fflush(NULL);
    /*Connection is dead*/
    cout <<"ByteCounter"<<counter<<endl;
    if(counter<0){
	cout <<"ByteCounter"<<counter<<endl;
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
  socketFd=::socket(AF_INET,SOCK_STREAM,0);
  socketaddr.sin_family=AF_INET;
  socketaddr.sin_addr.s_addr=htonl(INADDR_ANY);
  socketaddr.sin_port=htons(this->port);
}

/******************************************************************************/

/******************************************************************************/
void Socket::connect()
{
  this->init();
  inet_pton(AF_INET,hostname,&socketaddr.sin_addr);
  ::connect(socketFd,(struct sockaddr*)&socketaddr,sizeof(socketaddr));
  connectFd=socketFd;
}

/******************************************************************************/
void Socket::close()
{
  ::close(socketFd);
  ::close(connectFd);
}
/******************************************************************************/
bool Socket::isClosed()
{
    cout << "ConnectFD:"<<connectFd<<endl;
    return (connectFd<=0);
}
