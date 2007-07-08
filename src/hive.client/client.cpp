#include "org/esb/net/Socket.h"
#include "org/esb/io/InputStream.h"
#include "org/esb/io/OutputStream.h"
#include "org/esb/av/PacketOutputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/Frame.h"
#include <iostream>
#include "avformat.h"
using namespace std;
using namespace org::esb::net;
using namespace org::esb::io;
using namespace org::esb::av;

int main(int argc,char**argv){
    Socket * socket=new Socket("localhost",20000);
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
    
    PacketInputStream pis(input);
    PacketOutputStream pos(output);
    
    int counter=0;
     av_register_all();
    Codec codec(CODEC_ID_MPEG4);
    while(!socket->isClosed()){
	string getcmd="get frame";
	string putcmd="put frame";
	unsigned char * test=new unsigned char[10];
//	int size=input->read(test,10);
	output->write((char*)getcmd.c_str(),getcmd.size());
	Packet packet=pis.readPacket();
	Frame frame(&packet, &codec);
	
	
	cout << ++counter<<"neues Frame size:"<<frame.getSize()<<endl;
//	cout << ++counter<<"neues Frame size:"<<size<<"Data:"<<test<<endl;
    }
}



