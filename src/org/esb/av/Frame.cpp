#include "Packet.h"
#include "Frame.h"
#include "avformat.h"
#include <assert.h>
#include <iostream>
using namespace std;
using namespace org::esb::av;
Frame::Frame(Packet * packet, Codec * codec){
    assert(packet);
    assert(codec);

    pts=AV_NOPTS_VALUE;
    key_frame=1;

    _packet=packet;
    _codecContext=codec->getCodecContext();
    _width=_codecContext->width;
    _height=_codecContext->height;
    _pixFormat=_codecContext->pix_fmt;
    int bytesRemaining=_packet->size, frameFinished=0, bytesDecoded=0;
    uint8_t * rawData=packet->data;
    _buffer=new uint8_t[1];
    while(bytesRemaining > 0)
    {
      bytesDecoded=avcodec_decode_video(_codecContext, this, &frameFinished, rawData, bytesRemaining);
      if(bytesDecoded < 0)
      {
        fprintf(stderr, "Error while decoding frame\n");
        break;
      }

      bytesRemaining-=bytesDecoded;
      rawData+=bytesDecoded;
      if(frameFinished){
	    break;
      }
    }    
}

Frame::Frame(int format, int width, int height){
    pts=AV_NOPTS_VALUE;
    key_frame=1;
    /*
    int numBytes=avpicture_get_size(PIX_FMT_RGB24, width,height);
    uint8_t * buffer=new uint8_t[numBytes];
    // Assign appropriate parts of buffer to image planes in pFrameRGB
    avpicture_fill((AVPicture *)this, buffer, format, width, height);
    
    */
}

Frame::Frame(Frame * source, int format){
    pts=AV_NOPTS_VALUE;
    key_frame=1;
    _width=source->getWidth();
    _height=source->getHeight();
    _pixFormat=format;
    int numBytes=avpicture_get_size(format, source->getWidth(),source->getHeight());
    _buffer=new uint8_t[numBytes];
    avpicture_fill((AVPicture *)this, _buffer, format, source->getWidth(),source->getHeight());
    img_convert((AVPicture *)this, format, (AVPicture*)source, source->getFormat(), source->getWidth(),source->getHeight());

}

Frame::~Frame(){

    delete []_buffer;

}

AVPacket * Frame::getPacket(){
    return _packet;

}

uint8_t * Frame::getData(){
    return data[0];

}

int Frame::getFormat(){
    return _pixFormat;
}

int Frame::getSize(){
    return avpicture_get_size(getFormat(), getWidth(),getHeight());
}

Frame * Frame::getFrame(int format){
    Frame * result=this;
    if(format > 0){
        result=new Frame(this, format);
    }
    return result;
}

int Frame::getHeight(){
    return _height;
}

int Frame::getWidth(){
    return _width;
}
/*
void Frame::setFrame(AVFrame * frame){
    if(_frame!=NULL)
        av_free(_frame);
    _frame=frame;
}
*/