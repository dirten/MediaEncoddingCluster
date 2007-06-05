#include "FrameOutputStream.h"
#include "org/esb/io/OutputStream.h"

using namespace org::esb::av;

FrameOutputStream::FrameOutputStream(OutputStream * out){
    outStream=out;
}

FrameOutputStream::~FrameOutputStream(){


}

void FrameOutputStream::writeFrame(Frame * frame){
    write((char*)frame->getData(), frame->getSize());
}

void FrameOutputStream::write(char * buffer, int length){
    outStream->write(buffer, length);
}
