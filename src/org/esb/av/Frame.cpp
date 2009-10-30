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
  framePtr->pts = 0;
  _buffer = NULL; //new uint8_t[1];
  _allocated = false;
  _type = CODEC_TYPE_VIDEO;
  channels = 0;
  sample_rate = 0;
  _width = 0;
  _height = 0;
  _pixFormat = (PixelFormat) 0;
  _size = 0;
  duration = 0;
  _time_base.num = 0;
  _time_base.den = 0;
  _dts = 0;
}

Frame::Frame(uint8_t *buffer) {
  framePtr = boost::shared_ptr<AVFrame > (new AVFrame());
  framePtr->pts = 0;
  _buffer = buffer;
  _allocated = true;
  _width = 0;
  _height = 0;
  _pixFormat = (PixelFormat) 0;
  _isFinished = false;
  _size = 0;
  channels = 0;
  sample_rate = 0;
  duration = 0;
  _time_base.num = 0;
  _time_base.den = 0;
  _dts = 0;
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
  _size = 0;
  _dts=0;
  framePtr = boost::shared_ptr<AVFrame > (new AVFrame());
  framePtr->pts = 0;
  _width = width;
  _height = height;
  _pixFormat = format;
  _allocated = allocate;
  channels = 0;
  sample_rate = 0,
   duration = 0;
  _time_base.num = 0;
  _time_base.den = 0;
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
  if (allocate) {
    int numBytes = avpicture_get_size(format, width, height);
  _size=numBytes;
    _buffer = (uint8_t*)av_malloc(numBytes);
	_allocated=true;

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
  if (_allocated && _buffer) {
    //    cout <<"Delete Frame"<<endl;
    av_free(_buffer);
    _buffer = NULL;
    _allocated = false;
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
	return _size;
//  return avpicture_get_size(getFormat(), getWidth(), getHeight());
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

int Frame::getDuration() {
  return duration;
}

void Frame::setDuration(int d) {
  duration = d;
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

void Frame::setTimeBase(AVRational t) {
  _time_base = t;
}

AVRational Frame::getTimeBase() {
  return _time_base;
}
//P->S:1732:Pts:127800:Dts:124200:Index:0:Dur:3600:Pos:-1:TBase:1/90000:F:0:KF0
std::string Frame::toString() {
  std::ostringstream oss;
  oss << "F->S:" << _size <<
      ":Pts:" << framePtr->pts <<
      ":Dts:" << _dts <<
      ":SmplSz:" << _size <<
      ":W:" << _width <<
      ":H:" << _height <<
      ":PF:" << _pixFormat <<
      ":Ch:" << channels <<
      ":SmplRt:" << sample_rate <<
      ":Dur:" << duration <<
      ":TBase:" << _time_base.num << "/" <<_time_base.den;

  return std::string(oss.str());
}
/*
void Frame::setFrame(AVFrame * frame){
    if(_frame!=NULL)
        av_free(_frame);
    _frame=frame;
}
 */


