#include "Codec.h"
#include "Frame.h"
#include <iostream>
#include <assert.h>
#include "org/esb/util/Log.h"
#include "org/esb/util/Decimal.h"

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

      Codec::Codec(AVCodecContext * c, int mode) {
        ctx = c;
        _mode = mode;
        findCodec(mode);
//        ctx->codec = _codec;
        _opened = false;
        //		_codec_resolved=false;
      }

      Codec::Codec() {
        logdebug("Codec::Codec()");
        ctx = avcodec_alloc_context();
        setContextDefaults();
        _opened = false;
        _codec_resolved = false;
      }

      Codec::Codec(const CodecID codecId, int mode) {
        logdebug("Codec::Codec(const CodecID codecId=" << codecId << ", int mode=" << mode << ")");
        _codec_resolved = false;
        _mode = mode;
        ctx = avcodec_alloc_context();
        ctx->codec_id = codecId;
        findCodec(mode);
		avcodec_get_context_defaults2(ctx, _codec->type);
        ctx->codec_id = codecId;
        setContextDefaults();

        _opened = false;
        /*
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
         */
      }

      void Codec::setContextDefaults() {

        ctx->global_quality = 1000000;
        ctx->pix_fmt = (PixelFormat) 0;
        ctx->width = 0;
        ctx->height = 0;
        ctx->bit_rate = 0;
        ctx->time_base.num = 0;
        ctx->time_base.den = 0;
        ctx->gop_size = 0;
        ctx->sample_rate = 0;
        ctx->sample_fmt = (SampleFormat) 0;
        ctx->channels = 0;
		ctx->idct_algo=FF_IDCT_AUTO;
		ctx->skip_idct=AVDISCARD_DEFAULT;
		ctx->error_recognition= FF_ER_CAREFUL;
		ctx->error_concealment=3;
		ctx->workaround_bugs=1;
		ctx->debug=1;
		ctx->debug_mv=0;

        if (ctx->codec_id == CODEC_ID_MPEG2VIDEO) {
          ctx->max_b_frames = 2;
        }
        /*default settings for x264*/
        ctx->me_range = 16;
        ctx->max_qdiff = 4;
        ctx->qmin = 10;
        ctx->qmax = 51;
        ctx->qcompress = 0.6;

      }

      CodecType Codec::getCodecType() {
        return ctx->codec_type;
      }

      char *Codec::getCodecName() {
        return ctx->codec_name;
      }

      CodecID Codec::getCodecId() {
        return ctx->codec_id;
      }

      bool Codec::findCodec(int mode) {
        bool result = true;
        //        if(_codec_resolved)return result;
        //        logdebug("try to find " << (mode == DECODER ? "Decoder" : "Encoder") << " with id:" << _codec_id);
        if (mode == DECODER) {
          _codec = avcodec_find_decoder(ctx->codec_id);
          if (_codec == NULL) {
            logerror("Decoder not found for id :" << ctx->codec_id);
            result = false;
          }
        } else
          if (mode == ENCODER) {
          _codec = avcodec_find_encoder(ctx->codec_id);
          if (_codec == NULL) {
            logerror("Encoder not found for id :" << ctx->codec_id);
            result = false;
          }
        } else {
          logerror("Mode not set for Codec");
        }
        if (result) {
          ctx->codec_type = _codec->type;
          _codec_resolved = true;
        } else {
          logerror("in resolving codec");
        }

        return result;
      }

      void Codec::setParams() {
        /*
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
        }*/
        //        ctx->flags |= _flags;
        //					ctx->start_time=_start_time;
      }

      int Codec::open() {
        if (_opened)return 0;
        findCodec(_mode);
        //        if (findCodec(_mode)) {
        //          ctx = avcodec_alloc_context();
        //          setParams();
        if (_codec->capabilities & CODEC_CAP_TRUNCATED) {
          //			        	ctx->flags |= CODEC_FLAG_TRUNCATED;
          //					    cout <<"CodecCapTruncated"<<endl;
        }
        //				    ctx->flags |=CODEC_FLAG_LOW_DELAY;
        try {
          if (avcodec_open(ctx, _codec) < 0) {
            logerror("while openning Codec" << ctx->codec_id);

          } else {
            //              logdebug("Codec opened:" << _codec_id);
            fifo = av_fifo_alloc(1024);
            _opened = true;
          }
        } catch (...) {
          logerror("Exception while openning Codec" << ctx->codec_id);
        }
        return 0;
        //        }
        //        return -1;
      }

      void Codec::setFlag(int flag) {
        ctx->flags |= flag;
      }

      Codec::~Codec() {
        close();
      }

      void Codec::close() {
        av_freep(&ctx->stats_in);
        if (_opened) {
          avcodec_close(ctx);
          av_fifo_free(fifo);
          //          logdebug("Codec closed:" << _codec_id);
        } else {
          logdebug("Codec not closed, because it was not opened:" << ctx->codec_id);
        }
        av_free(ctx);
        _opened = false;
      }

      void Codec::setWidth(int w) {
        ctx->width = w;
      }

      void Codec::setHeight(int h) {
        ctx->height = h;
      }

      void Codec::setPixelFormat(PixelFormat pf) {
        ctx->pix_fmt = pf;
      }

      void Codec::setBitRate(int rate) {
        ctx->bit_rate = rate;
      }

      void Codec::setTimeBase(AVRational tb) {
        ctx->time_base = tb;
      }

      AVRational Codec::getTimeBase() {
        return ctx->time_base;
      }

      void Codec::setGopSize(int size) {
        ctx->gop_size = size;
      }

      void Codec::setChannels(int c) {
        ctx->channels = c;
      }

      void Codec::setSampleRate(int rate) {
        ctx->sample_rate = rate;
      }

      void Codec::setSampleFormat(SampleFormat f) {
        ctx->sample_fmt = f;
      }

      int Codec::getWidth() {
        return ctx->width;
      }

      int Codec::getHeight() {
        return ctx->height;
      }

      PixelFormat Codec::getPixelFormat() {
        return ctx->pix_fmt;
        //				return _pix_fmt;
      }

      int Codec::getSampleRate() {
        return ctx->sample_rate;
      }

      int Codec::getBitRate() {
        return ctx->bit_rate;
      }

      SampleFormat Codec::getSampleFormat() {
        return ctx->sample_fmt;
      }

      int Codec::getChannels() {
        return ctx->channels;
      }

      int Codec::getGopSize() {
        return ctx->gop_size;
      }

      int Codec::getFlags() {
        return ctx->flags;
      }

            /*
            void Codec::setStartTime(int64_t start) {
              _start_time = start;
            }
       */
      std::string Codec::toString() {
        using namespace org::esb::util;
        std::string data;
        data.append("Codec ID:").append(Decimal(ctx->codec_id).toString()).append("\r\n");
        data.append("Codec Name:").append(ctx->codec->name).append("\r\n");
        data.append("Codec Type:").append(ctx->codec_type == CODEC_TYPE_AUDIO ? "AUDIO" : "VIDEO").append("\r\n");
        data.append("Width:").append(Decimal(getWidth()).toString()).append("\r\n");
        data.append("Height:").append(Decimal(getHeight()).toString()).append("\r\n");
        data.append("Channels:").append(Decimal(getChannels()).toString()).append("\r\n");
        data.append("RequestChannelLayout:").append(Decimal(ctx->request_channel_layout).toString()).append("\r\n");
        data.append("ChannelLayout:").append(Decimal(ctx->channel_layout).toString()).append("\r\n");
        data.append("BitRate:").append(Decimal(ctx->bit_rate).toString()).append("\r\n");
        data.append("GOP:").append(Decimal(ctx->gop_size).toString()).append("\r\n");
        data.append("SampleRate:").append(Decimal(getSampleRate()).toString()).append("\r\n");
        data.append("SampleFormat:").append(Decimal(getSampleFormat()).toString()).append("\r\n");
        data.append("PixelFormat:").append(Decimal(getPixelFormat()).toString()).append("\r\n");
        char buf[256];
        avcodec_string(buf, sizeof (buf), ctx, _mode);
        data.append("InternalData:").append(std::string(buf)).append("\r\n");
        return data;
      }
    }
  }
}
