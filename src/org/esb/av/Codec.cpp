#include "Codec.h"
#include "Frame.h"
#include <iostream>
#include <assert.h>
#include "org/esb/util/Log.h"
using namespace std;

//void __attribute__ ((constructor)) my_init (void);

void my_init() {
  using namespace org::esb::av;
  //	cout << "test initialised"<<endl;
  //	logdebug("init av package");
  av_register_all();
  avcodec_init();
  avcodec_register_all();
}

namespace org {
  namespace esb {
    namespace av {

      Codec::Codec() {
        _opened = false;
        _codec_id = CODEC_ID_NONE;
        _mode = 0;
        _pix_fmt = (PixelFormat) 0;
        _width = 0;
        _height = 0;
        ////					_time_base=(AVRational){0,0};
        _time_base.num = 0;
        _time_base.den = 0;
        _gop_size = 0;
        _bit_rate = 0;
        _channels = 0;
        _sample_rate = 0;
        _sample_format = (SampleFormat) 0;
      }

      Codec::Codec(const CodecID codecId, int mode) {
        _codec_id = codecId;
        _flags = 0;
        _mode = mode;
        _width = 0;
        _height = 0;
        _bit_rate = 0;
        _gop_size = 0;
        _time_base.num = 0;
        _time_base.den = 0;
        _channels = 0;
        _sample_rate = 0;
        _sample_format = (SampleFormat) 0;
        _pix_fmt = (PixelFormat) 0;
        _opened = false;
      }

      CodecType Codec::getCodecType() {
        return ctx->codec_type;
      }

      char *Codec::getCodecName() {
        return ctx->codec_name;
      }

      CodecID Codec::getCodecId() {
        return _codec_id;
      }

      bool Codec::findCodec(int mode) {
        bool result = true;
        logdebug("try to find " << (mode == DECODER ? "Decoder" : "Encoder") << " with id:" << _codec_id);
        if (mode == DECODER) {
          _codec = avcodec_find_decoder((CodecID) _codec_id);
          if (_codec == NULL) {
            logerror("Decoder not found for id :" << _codec_id);
            result = false;
          }
        } else
          if (mode == ENCODER) {
          _codec = avcodec_find_encoder((CodecID) _codec_id);
          if (_codec == NULL) {
            logerror("Encoder not found for id :" << _codec_id);
            result = false;
          }
        } else {
          logerror("Mode not set for Codec");
        }
        return result;
      }

      void Codec::setParams() {
        ctx->codec_id = (CodecID) _codec_id;
        ctx->codec_type = _codec->type;
        //				ctx->global_quality = 1000000;
        ctx->pix_fmt = _pix_fmt;
        ctx->width = _width;
        ctx->height = _height;
        //				if(_bit_rate>0)
        ctx->bit_rate = _bit_rate;

        ctx->time_base = _time_base;

        //				if(_gop_size>0)
        ctx->gop_size = _gop_size;
        //				if(_sample_rate>0)
        ctx->sample_rate = _sample_rate;
        //				if(_sample_format>0)
        ctx->sample_fmt = _sample_format;
        //				if(_channels>0)
        ctx->channels = _channels;
        if (_codec->id == CODEC_ID_MPEG2VIDEO) {
          ctx->max_b_frames = 2;
          //					  cout << "setting max B Frames 2"<<endl;
        }
        ctx->flags |= _flags;
        //					ctx->start_time=_start_time;
      }

      int Codec::open() {
        if (findCodec(_mode)) {
          ctx = avcodec_alloc_context();
          setParams();
          if (_codec->capabilities & CODEC_CAP_TRUNCATED) {
            //			        	ctx->flags |= CODEC_FLAG_TRUNCATED;
            //					    cout <<"CodecCapTruncated"<<endl;
          }
          //				    ctx->flags |=CODEC_FLAG_LOW_DELAY;
          try {
            if (avcodec_open(ctx, _codec) < 0) {
              logerror("while openning Codec" << _codec_id);

            } else {
              logdebug("Codec opened:" << _codec_id);
              _opened = true;
            }
          } catch (...) {
            logerror("Exception while openning Codec" << _codec_id);
          }
          return 0;
        }
        return -1;
      }

      void Codec::setFlag(int flag) {
        _flags |= flag;
      }

      Codec::~Codec() {
        close();
      }

      void Codec::close() {
        if (_opened) {
          av_freep(&ctx->stats_in);
          avcodec_close(ctx);
          av_free(ctx);
          logdebug("Codec closed:" << _codec_id);
        } else {
          logdebug("Codec not closed, because it was not opened:" << _codec_id);
        }
        _opened = false;
      }

      void Codec::setWidth(int w) {
        _width = w;
      }

      void Codec::setHeight(int h) {
        _height = h;
      }

      void Codec::setPixelFormat(PixelFormat pf) {
        _pix_fmt = pf;
      }

      void Codec::setBitRate(int rate) {
        _bit_rate = rate;
      }

      void Codec::setTimeBase(AVRational tb) {
        _time_base = tb;
      }

      AVRational Codec::getTimeBase() {
        return _time_base ;
      }

      void Codec::setGopSize(int size) {
        _gop_size = size;
      }

      void Codec::setChannels(int c) {
        _channels = c;
      }

      void Codec::setSampleRate(int rate) {
        _sample_rate = rate;
      }

      void Codec::setSampleFormat(SampleFormat f) {
        _sample_format = f;
      }

      int Codec::getWidth() {
        return _width;
      }

      int Codec::getHeight() {
        return _height;
      }

      int Codec::getPixelFormat() {
        return ctx->pix_fmt;
        //				return _pix_fmt;
      }

      int Codec::getSampleRate() {
        return _sample_rate;
      }

      int Codec::getChannels() {
        return _channels;
      }

      void Codec::setStartTime(int64_t start) {
        _start_time = start;
      }

    }
  }
}
