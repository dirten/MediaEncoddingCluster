#include "org/esb/net/Socket.h"
#include "org/esb/io/InputStream.h"
#include "org/esb/io/OutputStream.h"
#include "org/esb/av/FrameOutputStream.h"
#include "org/esb/av/PacketOutputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/CodecInputStream.h"
#include "org/esb/io/FileOutputStream.h"
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
    CodecInputStream codecis(input);
    PacketOutputStream pos(output);
    
    int counter=0;
    int a=0;
     av_register_all();


    string getcodec="get inputcodec";
    output->write((char*)getcodec.c_str(),getcodec.size());

    Codec * codec=codecis.readCodec();
	codec->open();
	string getcmd="get frame";
	string putcmd="put frame";
	unsigned char * test=new unsigned char[10];

    while(!socket->isClosed()){
//	int size=input->read(test,10);
	output->write((char*)getcmd.c_str(),getcmd.size());

	Packet packet=pis.readPacket();
	if(a%1000==0)
	cout << a<<"Packet size:"<<packet.getSize()<<endl;
	
	Frame frame(&packet, codec);
//	cout << counter<<"neues Frame size:"<<frame.getSize()<<endl;

        Frame* rgb=frame.getFrame(PIX_FMT_RGB24);

/*
        char filename[32];
        sprintf(filename,"/tmp/hive/test.%d.ppm",a);

        FileOutputStream *out=new FileOutputStream(filename);
        FrameOutputStream *fout=new FrameOutputStream(out);

        char header[200];
        sprintf(header, "P6\n%d %d\n255\n", rgb->getWidth(), rgb->getHeight());
        fout->write(header, strlen(header));
        fout->writeFrame(rgb);
        delete fout;
        delete out;
        
*/

        delete rgb;


        a++;



    }
}



