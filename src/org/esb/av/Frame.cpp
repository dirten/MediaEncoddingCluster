#include "Packet.h"
#include "Frame.h"
#include <assert.h>
#include <iostream>
using namespace std;
using namespace org::esb::av;
void DeleteFrame(AVFrame* frame) {
  avcodec_free_frame(&frame);
  //av_frame_unref(frame);
  //av_frame_free(&frame);
  //av_free(frame);
}
Frame::Frame() {
  //    cout << "Create Frame()"<<endl;
  _isFinished = false;
  framePtr = boost::shared_ptr<AVFrame > (av_frame_alloc(), DeleteFrame);
  framePtr->quality = 100;
  framePtr->key_frame = 1;
  framePtr->pts = 0;
  _buffer = NULL; //new uint8_t[1];
  _allocated = false;
  _type = AVMEDIA_TYPE_VIDEO;
  channels = 0;
  sample_rate = 0;
  _width = 0;
  _height = 0;
  _pixFormat = (PixelFormat) 0;
  _size = 0;
  duration = 0;
  _time_base.num = 0;
  _time_base.den = 0;
  //_dts = 0;
  _frames = 1;
  memset(&_pixel_aspect_ratio, 0, sizeof (_pixel_aspect_ratio));
  memset(&_storage_aspect_ratio, 0, sizeof (_pixel_aspect_ratio));
  memset(&_display_aspect_ratio, 0, sizeof (_pixel_aspect_ratio));
}

Frame::Frame(uint8_t *buffer, int64_t size) {
  framePtr = boost::shared_ptr<AVFrame > (new AVFrame());
  _size = size;
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
  //_dts = 0;
  _frames = 1;
  memset(&_pixel_aspect_ratio, 0, sizeof (_pixel_aspect_ratio));
  memset(&_storage_aspect_ratio, 0, sizeof (_pixel_aspect_ratio));
  memset(&_display_aspect_ratio, 0, sizeof (_pixel_aspect_ratio));

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
  LOGDEBUG("Frame::Frame(const Frame & frame)")
  copyFromFrame(frame);
}

//Packet Packet::operator=(Packet & p){

Frame Frame::operator=(Frame & frame) {
  LOGDEBUG("Frame Frame::operator=(Frame & frame)");
  copyFromFrame(frame);
  return *this;
}
*/
Frame::Frame(PixelFormat format, int width, int height, bool allocate) {
  //  logdebug("Create Frame(int format, int width, int height)");
  _isFinished = false;
  _size = 0;
  //_dts = 0;
  framePtr = boost::shared_ptr<AVFrame > (new AVFrame());
  framePtr->pts = 0;
  _width = width;
  _height = height;
  _pixFormat = format;
  _allocated = allocate;
  channels = 0;
  sample_rate = 0, duration = 0;
  _time_base.num = 0;
  _time_base.den = 0;
  _frames = 1;
  _type = AVMEDIA_TYPE_VIDEO;
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
    LOGDEBUG("NumBytes to allocate=" << numBytes);
    _size = numBytes;
    _buffer = (uint8_t*) av_malloc(numBytes);
    _allocated = true;
    if (numBytes > 0) {
      memset(_buffer, 0, numBytes);
      // Assign appropriate parts of buffer to image planes
      avpicture_fill((AVPicture*) framePtr.get(), _buffer, format, width, height);
    }
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
  //cout << "try Delete Frame:" << framePtr.use_count() << " : " << this <<":"<<_allocated<<":"<<(_buffer!=NULL)<< endl;
  if (framePtr.use_count() == 1) {
    //AVFrame * frame=framePtr.get();
    //avcodec_free_frame(&frame);
  }
  if (_allocated && _buffer!=NULL) {
    if (framePtr.use_count() == 1) {
      //cout << "Delete Frame:" << framePtr.use_count() << " : " << this << endl;
      av_free(_buffer);
      _buffer = NULL;
      _allocated = false;
    }
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

int Frame::getSampleCount(){
  return framePtr->nb_samples;
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

void Frame::setWidth(int w) {
  _width = w;
}

void Frame::setHeight(int h) {
  _height = h;
}

int Frame::getDuration() {
  return duration;
}

int Frame::getFrameCount() {
  return _frames;
}

void Frame::setFrameCount(int d) {
  _frames = d;
}

void Frame::setDuration(int d) {
  duration = d;
}

int64_t Frame::getPts() {
  return framePtr->pts;
}
/*
int64_t Frame::getDts() {
  return _dts;
}
*/
void Frame::setPts(int64_t pts) {
  framePtr->pts = pts;
}
/*
void Frame::setDts(int64_t dts) {
  _dts = dts;
}
*/
void Frame::setTimeBase(AVRational t) {
  _time_base = t;
}

void Frame::setTimeBase(int num, int den) {
  _time_base.num = num;
  _time_base.den = den;
}

AVRational Frame::getTimeBase() {
  return _time_base;
}

AVRational Frame::getPixelAspectRatio() {
  return _pixel_aspect_ratio;
}

AVRational Frame::getDisplayAspectRatio() {
  return _display_aspect_ratio;
}

AVRational Frame::getStorageAspectRatio() {
  return _storage_aspect_ratio;
}

void Frame::setPixelAspectRatio(AVRational par) {
  if (par.num < 1)
    par.num = 1;
  if (par.den < 1)
    par.den = 1;
  _pixel_aspect_ratio = par;
}

void Frame::setPixelAspectRatio(int num, int den) {
  _pixel_aspect_ratio.num = num;
  _pixel_aspect_ratio.den = den;
}

void Frame::setDisplayAspectRatio(AVRational r) {
  _display_aspect_ratio = r;
}

void Frame::setDisplayAspectRatio(int num, int den) {
  _display_aspect_ratio.num = num;
  _display_aspect_ratio.den = den;
}

void Frame::setStorageAspectRatio(AVRational r) {
  _storage_aspect_ratio = r;
}

void Frame::setStorageAspectRatio(int num, int den) {
  _storage_aspect_ratio.num = num;
  _storage_aspect_ratio.den = den;
}

void Frame::copyFromFrame(const Frame& frame) {
  //quality = frame.quality;
  _isFinished = frame._isFinished;
  _size = frame._size;
  //_dts = frame._dts;
  framePtr = boost::shared_ptr<AVFrame > (new AVFrame());
  framePtr->pts = frame.framePtr->pts;
  _width = frame._width;
  _height = frame._height;
  _pixFormat = frame._pixFormat;
  _allocated = true;
  channels = frame.channels;
  sample_rate = frame.sample_rate;
  duration = frame.duration;
  _time_base.num = frame._time_base.num;
  _time_base.den = frame._time_base.den;
  _frames = frame._frames;
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
  if (frame._type == AVMEDIA_TYPE_VIDEO) {
    int numBytes = avpicture_get_size(_pixFormat, _width, _height);
    LOGDEBUG("NumBytes to allocate=" << numBytes);
    _size = numBytes;
    _buffer = (uint8_t*) av_malloc(numBytes);
    _allocated = true;
    if (numBytes > 0) {
      memset(_buffer, 0, numBytes);
      // Assign appropriate parts of buffer to image planes
      avpicture_fill((AVPicture*) framePtr.get(), _buffer, _pixFormat, _width, _height);
      av_picture_copy((AVPicture*) framePtr.get(), (AVPicture*) frame.framePtr.get(), _pixFormat, _width, _height);
      
    }
  } else {
    _size = frame._size;
    _buffer = (uint8_t*) av_malloc(_size);
    memcpy(_buffer, frame._buffer, _size);
    channels = frame.channels;
    sample_rate = frame.sample_rate;
  }
  /*
    _width = frame._width;
  _height = frame._height;
  _pixFormat = frame._pixFormat;
  _type = frame._type;
  if (frame._type == AVMEDIA_TYPE_VIDEO) {
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
    
  }
  pts = frame.pts;
  _dts = frame._dts;
  stream_index = frame.stream_index;
  duration = frame.duration;
  pos = frame.pos;
   */
}

//P->S:1732:Pts:127800:Dts:124200:Index:0:Dur:3600:Pos:-1:TBase:1/90000:F:0:KF0

std::string Frame::toString() {

  std::ostringstream oss;
  oss << "F->S:" << 0 << ":Pts:" << framePtr->pts << ":SmplFmt:" << framePtr->format << ":SmplSz:" << framePtr->nb_samples << ":W:" << framePtr->width << ":H:" << framePtr->height << ":PF:" << _pixFormat << ":Ch:" << av_frame_get_channels(framePtr.get()) <<":ChL:"<< av_frame_get_channel_layout(framePtr.get())<< ":SmplRt:" << sample_rate << ":Dur:" << duration << ":FrameCount" << _frames << ":TBase:" << _time_base.num << "/" << _time_base.den << ":q" << framePtr->quality<<":fin:"<<_isFinished;
  oss<<":ppts:"<<framePtr->pkt_pts;
  oss<<":psts:"<<framePtr->pkt_dts;
  oss<<":t:";
  switch (getAVFrame()->pict_type) {
    case AV_PICTURE_TYPE_B:
      oss << ("B");
      break;
    case AV_PICTURE_TYPE_I:
      oss << ("I");
      break;
    case AV_PICTURE_TYPE_P:
      oss << ("P");
      break;
    case AV_PICTURE_TYPE_S:
      oss << ("S");
      break;
    case AV_PICTURE_TYPE_SI:
      oss << ("SI");
      break;
    case AV_PICTURE_TYPE_SP:
      oss << ("SP");
      break;
    case AV_PICTURE_TYPE_BI:
      oss << ("BI");
      break;
    default:
      oss << "U=" << getAVFrame()->pict_type;
      break;

  }

  return oss.str();
}

/*
 void Frame::setFrame(AVFrame * frame){
 if(_frame!=NULL)
 av_free(_frame);
 _frame=frame;
 }
 */
void Frame::dumpHex() {
  printf("hexdump:\n");
  int len, i, j, c;
  int size=_size;//framePtr->nb_samples;
  uint8_t* buffer=_buffer;//framePtr->data[0];
  for (i = 0; i < size; i += 16) {
    len = size - i;
    if (len > 16)
      len = 16;
    printf("%08x ", i);
    for (j = 0; j < 16; j++) {
      if (j < len)
        printf(" %02x", buffer[i + j]);
      else
        printf("   ");
    }
    printf(" ");
    for (j = 0; j < len; j++) {
      c = buffer[i + j];
      if (c < ' ' || c > '~')
        c = '.';
      printf("%c", c);
    }
    printf("\n");
  }

}

