#include "FrameInputStream.h"
#include "Frame.h"
#include "org/esb/net/SocketInputStream.cpp"
#include "introspec.h"

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
	
}

int FrameInputStream::read(unsigned char * buffer, int length){
	int buffer_size=0;
	if(instanceOf(*_source, SocketInputStream)){
		buffer_size=available(true);
	}else{
		buffer_size=available();
	}
}

void FrameInputStream::close(){
	
}

