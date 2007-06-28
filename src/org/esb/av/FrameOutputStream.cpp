#include "FrameOutputStream.h"
#include "org/esb/io/OutputStream.h"
#include <iostream>
using namespace std;
using namespace org::esb::av;

 
 
FrameOutputStream::FrameOutputStream(OutputStream * out){
    outStream=out;
}

FrameOutputStream::~FrameOutputStream(){


}

void FrameOutputStream::writeFrame(Frame * frame){
	char format[5];
	char width[5];
	char height[5];
	memset(format,0,5);
	memset(width,0,5);
	memset(height,0,5);
	sprintf(format,"%04d", frame->getFormat());
	sprintf(width,"%04d", frame->getWidth());
	sprintf(height,"%04d", frame->getHeight());
    write(format, 4);
    write(width, 4);
    write(height, 4);
    write((char*)frame->getData(), frame->getSize());
}

void FrameOutputStream::write(char * buffer, int length){
    outStream->write(buffer, length);
}
void FrameOutputStream::write(vector<unsigned char>&buffer){
    outStream->write(buffer);
}
