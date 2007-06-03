#include "FrameOutputStream.h"
#include "org/esb/io/OutputStream.h"

using namespace org::esb::av;

FrameOutputStream::FrameOutputStream(OutputStream * out){
    outStream=out;
}

FrameOutputStream::~FrameOutputStream(){


}

void FrameOutputStream::writeFrame(Frame * frame){
    int width=frame->getWidth()*3;
    int height=frame->getHeight();
    char * buffer=new char[width*height];
    memset(buffer,0,(width)*height);
    for(int y=0; y<height; y++){
        memcpy(buffer+(y*width),frame->getFrame()->data[0]+(y*width),width);
//        fout->write((char *)pFrameRGB->data[0]+y*pFrameRGB->linesize[0], width*3);
    }


//    int size=frame->getFrame()->linesize[0]*frame->getFrame()->linesize[1];
//    uint8_t * buffer=frame->getFrame()->data[0];
    write((char*)buffer, width*height);
}

void FrameOutputStream::write(char * buffer, int length){
    outStream->write(buffer, length);
}
