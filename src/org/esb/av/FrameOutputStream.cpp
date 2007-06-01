#include "FrameOutputStream.h"
#include "org/esb/io/OutputStream.h"

using namespace org::esb::av;

FrameOutputStream::FrameOutputStream(OutputStream * out){
    outStream=out;

}

FrameOutputStream::~FrameOutputStream(){


}

void FrameOutputStream::writeFrame(Frame * frame){
    int size=frame->getFrame()->linesize[0]*frame->getFrame()->linesize[1];
    uint8_t * buffer=frame->getFrame()->data[0];
    write((char*)buffer, size);

}

void FrameOutputStream::write(char * buffer, int length){
    outStream->write(buffer, length);
}
