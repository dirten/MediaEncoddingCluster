#include "FrameInputStream.h"
#include "Frame.h"
#include "org/esb/net/SocketInputStream.cpp"
#include "introspec.h"
#include "AV.h"

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
	if(atoi(width)==0||atoi(height)==0)return NULL;
	int size=avpicture_get_size(atoi(format), atoi(width),atoi(height));
	char buffer[size];
	int remaining=size, counter=0, bytes=0;
	while(remaining > 0){
		bytes=0;
		bytes=this->read((unsigned char*)buffer+counter, remaining);
		if(bytes<=0){
			return NULL;
		}		
		remaining-=bytes;
		counter+=bytes;
	}
	Frame * frame=new Frame(atoi(format), atoi(width), atoi(height), (unsigned char*)buffer);
	return frame;
}

int FrameInputStream::read(){
//	return _source->read(buffer, length);
	return 0;
}
int FrameInputStream::read(unsigned char * buffer, int length){
	return _source->read(buffer, length);
}

int FrameInputStream::read(vector<unsigned char>&buffer){
	return _source->read(buffer);
}

void FrameInputStream::close(){
	
}

