
#include <iostream>
//#include "config.h"
#include "org/esb/socket/socket.h"
#include "org/esb/config/config.h"
#include <fcntl.h>
#include <iostream>
#include <fstream>
using namespace std;
using namespace org::esb::socket;
using namespace org::esb::config;
class server{
    public:
	server(){
	    
	    fstream FileBin("/tmp/frame.container/1/frame210.data.ppm",ios::in|ios::out|ios::binary);
	    FileBin.seekg(0,ios::end);
	    unsigned long filesize=streamoff(FileBin.tellg());
	    FileBin.seekg(0,ios::beg);
	    string strBuffer="";
	    char buffer[filesize];
	    FileBin.read(buffer, filesize);
//	    cout <<"Buffer:"<< buffer << endl;
	    printf("FileSize:%d->%d", strlen(buffer), filesize);
	    

/*
//	    FILE * f=fopen("/tmp/frame.container/1/frame1.data.ppm","r+b");
	    int f=open("/tmp/frame.container/1/frame100.fstream.ppm",O_RDONLY);
	    if(!(f)){
		cout << "Datei nicht geöffnet"<<endl;
		return;
	    }
	    unsigned char *tmpbuffer=new unsigned char[4000000];
//	    memset(tmpbuffer,0,4000000);
	    int *buffer=new int[1024];
//	    unsigned char c;
//	    for(int i=0;c=fgetc(f);i++){
//	    for(int i=0;fread(buffer,1,255,f)!=0;i++){
	    int byteCount=0;
	    int count=0;
//	    while(count=read(f,buffer,255)){
	    string bla;
 	    for(int i=0;(count=read(f,buffer,1024));i++){
//		cout << count<<endl;;
//		bla.insert(byteCount, buffer);
		memcpy(tmpbuffer+byteCount,buffer,count);
		byteCount+=count;
//		fread(buffer,1,1,f);
//		bla.insert(byteCount, buffer);
		bla+=(char*)buffer;
//		tmpbuffer+=count;
//		cout<<buffer;
//		tmpbuffer[i]=c;
	    }
	    cout<<"ByteCount"<<byteCount<<endl;
	    cout<<"Plan:"<<bla.size()<<endl;
//	    cout << "Buffer:"<<bla<<endl;
	    int filesize=byteCount;
//	    buffer=bla.c_str();
*/
	    Config::init("test.cfg");
	    Socket *socket=new Socket();
//	    socket->setHostname("localhost");
	    socket->setPort(10000);
	    socket->Listen();
	for(;;){
	    cout<<"Waiting for connection"<<endl;
	    socket->Accept();
	    cout<<"conection accepted"<<endl;
//	    string s="bla fasel";
	    SocketData * data=new SocketData();
	    data->data=buffer;
	    data->data_length=filesize;
	    socket->write(data);
	    cout<<"sendet"<<endl;
	}
    }
};
int main(int argc,char**argv){
    server * s=new server();
//    delete s;
}
