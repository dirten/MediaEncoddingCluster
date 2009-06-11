#include "Packet.h"
#include "Frame.h"
#include <assert.h>
#include <iostream>
using namespace std;
using namespace org::esb::av;

Frame::Frame() {
  //    cout << "Create Frame()"<<endl;
  _isFinished = false;
  framePtr = boost::shared_ptr<AVFrame > (new AVFrame());
  framePtr->quality = 100;
  framePtr->key_frame = 1;
  _buffer = NULL; //new uint8_t[1];
  _allocated=false;
  _type = CODEC_TYPE_VIDEO;
  channels = 0;
  sample_rate = 0;
  _width = 0;
  _height = 0;
  _pixFormat=(PixelFormat)0;

}
Frame::Frame(uint8_t *buffer) {
   framePtr = boost::shared_ptr<AVFrame > (new AVFrame());
  _buffer=buffer;
  _allocated=true;
  _width = 0;
  _height = 0;
  _pixFormat=(PixelFormat)0;
  _isFinished=false;

}
/*
Frame::Frame(Packet * packet, Codec * codec){
    assert(packet);
    assert(codec);
    
    key_frame=1;
    _frameFinished=0;
    _packet=packet->packet;
//    _codecContext=codec;
    _width=_codecContext->width;
    _height=_codecContext->height;
    _pixFormat=_codecContext->pix_fmt;
    int bytesRemaining=_packet->size,  bytesDecoded=0;
    uint8_t * rawData=packet->packet->data;
    _buffer=new uint8_t[1];
    while(bytesRemaining > 0)
    {
      bytesDecoded=avcodec_decode_video(_codecContext, this, &_frameFinished, rawData, bytesRemaining);
      if(bytesDecoded < 0)
      {
        fprintf(stderr, "Error while decoding frame\n");
        break;
      }

      bytesRemaining-=bytesDecoded;
      rawData+=bytesDecoded;
      if(_frameFinished){
            break;
      }
    }
    
    pts=packet->packet->pts;
    
}
 */
/*
Frame::Frame(const Frame & frame) {
  logdebug("Create Frame(const Frame & frame)");
  quality = 100;
  _width = frame._width;
  _height = frame._height;
  _pixFormat = frame._pixFormat;
  _type = frame._type;
  if (frame._type == CODEC_TYPE_VIDEO) {
    avcodec_get_frame_defaults(this);
    int numBytes = avpicture_get_size(_pixFormat, _width, _height);
    _buffer = new uint8_t[numBytes];
    memset(_buffer, 0, numBytes);
    avpicture_fill((AVPicture*)this, _buffer, _pixFormat, _width, _height);
    av_picture_copy((AVPicture*)this, (AVPicture*) & frame, _pixFormat, _width, _height);
  } else {
    _size = frame._size;
    _buffer = new uint8_t[_size];
    memcpy(_buffer, frame._buffer, _size);
    channels = frame.channels;
    sample_rate = frame.sample_rate;
    ;
  }
  pts = frame.pts;
  _dts = frame._dts;
  stream_index = frame.stream_index;
  duration = frame.duration;
  pos = frame.pos;
}
 */
//Packet Packet::operator=(Packet & p){

/*
Frame Frame::operator=(Frame & frame) {
  logdebug("Create Frame::operator=(Frame & frame)");
  quality = 100;
  _width = frame._width;
  _height = frame._height;
  _pixFormat = frame._pixFormat;
  //    pts=frame.pts;
  avcodec_get_frame_defaults(this);
  int numBytes = avpicture_get_size(_pixFormat, _width, _height);
  _buffer = new uint8_t[numBytes];
  memset(_buffer, 0, numBytes);
  avpicture_fill((AVPicture*)this, _buffer, _pixFormat, _width, _height);
  av_picture_copy((AVPicture*)this, (AVPicture*) & frame, _pixFormat, _width, _height);
  pts = frame.pts;
  _dts = frame._dts;
  duration = frame.duration;
  stream_index = frame.stream_index;
  pos = frame.pos;
  return *this;
}
 */


Frame::Frame(PixelFormat format, int width, int height, bool allocate) {
//  logdebug("Create Frame(int format, int width, int height)");
  _isFinished = false;
  framePtr = boost::shared_ptr<AVFrame > (new AVFrame());
  _width = width;
  _height = height;
  _pixFormat = format;
  _allocated=allocate;
  /*
    quality = 100;
    channels = 0;
    sample_rate = 0;
    _width = width;
    _height = height;
    _pixFormat = format;

    pts = AV_NOPTS_VALUE;
   */
  //  avcodec_get_frame_defaults(this);
  if(allocate){
    int numBytes = avpicture_get_size(format, width, height);
    _buffer = new uint8_t[numBytes];
    memset(_buffer, 0, numBytes);
    // Assign appropriate parts of buffer to image planes
    avpicture_fill((AVPicture*) framePtr.get(), _buffer, format, width, height);
  }
}
/*
Frame::Frame(int format, int width, int height, unsigned char * data){
    pts=AV_NOPTS_VALUE;
    key_frame=1;
    _width=width;
    _height=height;
    _pixFormat=format;
    int numBytes=avpicture_get_size(format, _width,_height);
    _buffer=new uint8_t[numBytes];
    memcpy(_buffer, data, numBytes);
    avpicture_fill((AVPicture *)this, _buffer, format, _width,_height);
//    img_convert((AVPicture *)this, format, (AVPicture*)source, source->getFormat(), source->getWidth(),source->getHeight());
}
 */

/*
Frame::Frame(Frame * source, int format){
    pts=AV_NOPTS_VALUE;
    key_frame=1;



    _width=source->getWidth();
    _height=source->getHeight();
    _pixFormat=format;
    int numBytes=avpicture_get_size(_pixFormat, _width,_height);
    _buffer=new uint8_t[numBytes];
    memset(_buffer, 0, numBytes);
    avpicture_fill((AVPicture *)this, _buffer, _pixFormat, _width,_height);
//    if(source->_frameFinished){
//	img_convert((AVPicture *)this, _pixFormat, (AVPicture*)source, source->getFormat(), source->getWidth(),source->getHeight());
//    }
    pts=source->pts;
}
 */

Frame::~Frame() {
  if (_allocated&&_buffer) {
    //    cout <<"Delete Frame"<<endl;
    delete []_buffer;
    _buffer = NULL;
  }
}

AVPacket * Frame::getPacket() {
  return _packet;

}

AVFrame * Frame::getAVFrame() {
  return framePtr.get();
}

uint8_t * Frame::getData() {
  return framePtr->data[0];
}

PixelFormat Frame::getFormat() {
  return _pixFormat;
}

int Frame::getSize() {
  return avpicture_get_size(getFormat(), getWidth(), getHeight());
}

void Frame::setFinished(bool f) {
  _isFinished = f;
}

bool Frame::isFinished() {
  return _isFinished;
}

/*
Frame Frame::getFrame(int format){
    return Frame(this, format);
}
 */
int Frame::getHeight() {
  return _height;
}

int Frame::getWidth() {
  return _width;
}

int64_t Frame::getPts() {
  return framePtr->pts;
}

int64_t Frame::getDts() {
  return _dts;
}

void Frame::setPts(int64_t pts) {
  framePtr->pts = pts;
}

void Frame::setDts(int64_t dts) {
  _dts = dts;
}

void Frame::toString() {
  if (getHeight() > 0 && getWidth() > 0)
    logdebug("Frame->Size:" << getSize());
  logdebug("Frame->Width:" << getWidth());
  logdebug("Frame->Height:" << getHeight());
  logdebug("Frame->PixelFormat:" << getFormat());
  logdebug("Frame->Pts:" << getPts());
  logdebug("Frame->Dts:" << getDts());
  logdebug("Frame->Channels:" << channels);
  logdebug("Frame->SampleRate:" << sample_rate);
}
/*
void Frame::setFrame(AVFrame * frame){
    if(_frame!=NULL)
        av_free(_frame);
    _frame=frame;
}
 */


