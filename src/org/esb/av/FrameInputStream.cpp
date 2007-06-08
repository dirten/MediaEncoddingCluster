#include "FrameInputStream.h"
#include "Frame.h"
#include "org/esb/net/SocketInputStream.cpp"
#include "introspec.h"
#include "avformat.h"

using namespace org::esb::av;
using namespace org::esb::net;

FrameInputStream::FrameInputStream(InputStream * source){
	_source=source;
}

FrameInputStream::~FrameInputStream(){
	
}

int FrameInputStream::available(bool isBlocking){
	return _source->available(isBlocking);
}

Frame * FrameInputStream::readFrame(){
	char format[5];
	char width[5];
	char height[5];
	memset(format,0,5);
	memset(width,0,5);
	memset(height,0,5);

	read((unsigned char*)format,4);
	read((unsigned char*)width,4);
	read((unsigned char*)height,4);
	int size=avpicture_get_size(atoi(format), atoi(width),atoi(height));
	char buffer[size];
	int remaining=size, counter=0, bytes=0;
	while(remaining > 0){
		bytes=read((unsigned char*)buffer+counter, remaining);
		if(bytes<0){
			cerr << "Fehler beim lesen des Frames"<<endl;
		}
		cout << "remaining:"<<remaining<<endl;
		
		remaining-=bytes;
		counter+=bytes;
	}
	cerr << "Bytes Readed="<<counter<<endl;
	Frame * frame=new Frame(atoi(format), atoi(width), atoi(height), (unsigned char*)buffer);
	return frame;
}

int FrameInputStream::read(unsigned char * buffer, int length){
	return _source->read(buffer, length);
}

void FrameInputStream::close(){
	
}

