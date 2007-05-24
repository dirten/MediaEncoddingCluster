#include "org/esb/net/Socket.h"
#include "org/esb/io/InputStream.h"
#include "org/esb/io/OutputStream.h"
#include <iostream>
using namespace std;
using namespace org::esb::net;
using namespace org::esb::io;

int main(int argc,char**argv){
    Socket * socket=new Socket("localhost",20001);
    socket->connect();
    InputStream *input=socket->getInputStream();
    if(input == NULL){
	cout << "Kein inputstream"<<endl;
	exit(1);
    }	
    OutputStream *output=socket->getOutputStream();
    if(output == NULL){
	cout << "Kein outputstream"<<endl;
	exit(1);
    }
    int counter=0;
    while(true){
	int size=input->available(true);
	if(size==0){
	    cout << "ZeroByte received"<<endl;
	    continue;
	}
	cout << ++counter<<"neues Frame size:"<<size<<endl;
	unsigned char buffer[size];
	int read=input->read(buffer, size);
	output->write((char*)buffer, size);
    }
}



