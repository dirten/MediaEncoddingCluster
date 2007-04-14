#include <iostream>
#include "socket.h"
#include "config.h"
using namespace std;

Socket::Socket(int sock){
    this->connectFd=sock;
    Socket();
//    cout<<"socket"<<sock<<endl;
}
Socket::~Socket(){
}

Socket::Socket(){
    this->hostname="localhost";
    this->port=0;
    this->socketFd=0;
    this->connectFd=0;
    bzero(&socketaddr,sizeof(socketaddr));    
}
void Socket::setHostname(char*name){
    this->hostname=name;
}
void Socket::setPort(int p){
    this->port=p;
}
char * Socket::getHostname(){
    return this->hostname;
}
int Socket::getPort(){
    return port;
}

struct SocketData{
    unsigned char*data;
    int data_length;
}data;
bool Socket::send(unsigned char *in){
//    cout<<"Socket::send(char*in)"<<endl;
//    cout<<connectFd<<endl;
    int width=0, height=0;
//    data.data=new unsigned char[100];
    memcpy(&data,"test",4);
//    width=atoi(EsbConfig::getConfig("width"));
//    height=atoi(EsbConfig::getConfig("height"));
//    char*bytes=new char[64];
//    sprintf(bytes,"%d",(width*height*3));
//    int rest=width*height*3;
//    int offset=0, byte=0;
//    int wri=write(this->connectFd,bytes,64);
//    cout<<"writed length of "<<bytes<<endl;
    int rest=sizeof((struct SocketData)data);
    cout << "StructSize"<<sizeof((struct SocketData)data)<<endl;
    while(rest>0){
	int byte=write(this->connectFd,&data,sizeof((struct SocketData)data));
//	int byte=fwrite(&data,1,10,this->connectFd);
//	offset+=byte;
	rest-=byte;
	cout<<"sended "<<byte<<" bytes"<<endl;
    }
    cout<<"frame sendet"<<endl;
    return true;
}
char* Socket::Recv(){
    char*bytes_str=new char[64];
    read(this->connectFd,bytes_str,64);
    int bytes=atoi(bytes_str);
    cout<<"try to recv "<<bytes<<" bytes"<<endl;
    int counter=0;
    char recvBuffer[8192];
    int offset=0;
    int rest=bytes;
    int all=0;  

    char*frame=new char[bytes];
    while(all<bytes){
	int maxrecv=rest>sizeof(recvBuffer)?sizeof(recvBuffer):rest;
	counter=read(this->socketFd,recvBuffer,maxrecv);
	if(counter<0)return false;
	memcpy(frame+offset,recvBuffer,counter);
	offset+=counter;
	rest-=counter;
	all+=counter;
	cout<<counter<<" received"<<endl;
    }
    cout<<"received "<<strlen(frame)<<" bytes"<<endl;
    return frame;
}

void Socket::init(){
    socketFd=socket(AF_INET,SOCK_STREAM,0);
    socketaddr.sin_family=AF_INET;
    socketaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    socketaddr.sin_port=htons(this->port);
}
void Socket::Listen(){
    this->init();
    if(bind(socketFd,(struct sockaddr*)&socketaddr, sizeof(socketaddr))<0){
	perror("Bind");
	close(socketFd);
	exit(1);
    }
    listen(socketFd,1024);
}
int Socket::Accept(){
    sockaddr_in client;
    socklen_t clilen=0;
    bzero(&client, sizeof(client));
//    cout<<"listening on port:"<<port<<endl;
    connectFd=accept(socketFd,(struct sockaddr*)&client,&clilen);
    if(connectFd<0){
	close(connectFd);
	close(socketFd);
	exit(1);
    }
    return connectFd;
}

void Socket::Connect(){
    this->init();
    inet_pton(AF_INET,hostname,&socketaddr.sin_addr);
    connect(socketFd,(struct sockaddr*)&socketaddr,sizeof(socketaddr));
    connectFd=socketFd;    
}

void Socket::Close(){
    close(socketFd);
    close(connectFd);
}
