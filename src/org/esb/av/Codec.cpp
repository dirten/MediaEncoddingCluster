/*----------------------------------------------------------------------
 *  File    : ProcessUnit.cpp
 *  Author  : Jan Hölscher <jan.hoelscher@esblab.com>
 *  Purpose :
 *  Created : 6. November 2009, 12:30 by Jan Hölscher <jan.hoelscher@esblab.com>
 *
 *
 * MediaEncodingCluster, Copyright (C) 2001-2009   Jan Hölscher
 *
 * This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307 USA
 *
 * ----------------------------------------------------------------------
 */
#include "Codec.h"
#include "Frame.h"
#include <iostream>
#include <assert.h>
#include "org/esb/util/Log.h"
#include "org/esb/util/StringUtil.h"


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
      boost::mutex Codec::open_close_mutex;

      /**
       * @TODO: need to copy all attributes from context to our own context structure
       * because of a memleak in decoder->open()
       */

      /**
       * @WARNING: this construcotr is dangerous because ffmpeg would try to decode the first frames
       * in case to resolve stream information,
       * that may cause in an error like mpegvideo@warning: first frame is no keyframe
       * it seems that ffmpeg does not cleanly close the openned coded or what ever???
       *
       * also, please dont try to decode Chunks from the Packetizer with a Decoder constructed by this Method!!!
       *
       * !!!!!!!!!!!!!!!!!!!! please be carefull, that take me some days to find it out !!!!!!!!!!!!!!!!!!!
       * @param s
       * @param mode
       */
      Codec::Codec(AVStream * s, int mode) {
        LOGWARN("!!!PLEASE DONT USE THIS CONSTRUCTOR!!!");
        ctx = s->codec;
        _mode = mode;
        //		ctx->codec_id=ctx->codec->id;
        findCodec(mode);
        //        ctx->codec = _codec;
        _opened = false;
        _pre_allocated = true;


        ctx->request_channels = 2;
        ctx->request_channel_layout = 2;
        _bytes_discard = 0;

        const AVOption * option = NULL;
        while (option = av_next_option(s->codec, option)) {
          if (option->offset > 0) {
            int len = 1000;
            char data[1000];
            av_get_string(s->codec, option->name, NULL, data, len);
            if (strlen(data) > 0) {
              //LOGDEBUG("Setting Context Option "<<option->name<<"="<<data);
              _options[option->name] = std::string(data);
            } else {
              LOGDEBUG("No data for Context Option " << option->name);
            }
          }
        }
        if (_codec && _codec->type & AVMEDIA_TYPE_AUDIO) {
          setTimeBase(1, ctx->sample_rate);
        }
        _frame_rate.num = s->r_frame_rate.num;
        _frame_rate.den = s->r_frame_rate.den;

        //		_codec_resolved=false;
      }

      Codec::Codec(int mode) : _mode(mode) {
        //        logdebug("Codec::Codec()");
        ctx = avcodec_alloc_context();
        setContextDefaults();
        _opened = false;
        _codec_resolved = false;
        _pre_allocated = false;
        _bytes_discard = 0;
        _frame_rate.num = 0;
        _frame_rate.den = 0;
      }

      Codec::Codec(const CodecID codecId, int mode) {
        //        logdebug("Codec::Codec(const CodecID codecId=" << codecId << ", int mode=" << mode << ")");
        _codec_resolved = false;
        _mode = mode;
        ctx = avcodec_alloc_context();
        ctx->codec_id = codecId;
        if (codecId>-1) {
          findCodec(mode);
          if (_codec_resolved) {
            avcodec_get_context_defaults2(ctx, _codec->type);
          }
          ctx->codec_id = codecId;
          setContextDefaults();
        }

        /*
         * special handling of the test decoder,
         * in that case a CodecId of -1 is entered,
         * see test/org/esb/av/TestDecoder
         */
        _opened = codecId>-1 ? false : true;

        _pre_allocated = false;
        _bytes_discard = 0;
        _frame_rate.num = 0;
        _frame_rate.den = 0;
        fifo = NULL;
      }

      void Codec::setCodecId(CodecID id) {
        ctx->codec_id = id;
      }

      int Codec::setCodecOption(std::string opt, std::string arg) {
        //LOGTRACE("setCodecOption(" << opt << "," << arg << ")");
        _options[opt] = arg;
        return 0;
      }

      std::string Codec::getCodecOption(std::string opt) {
        std::string result;
        if (_options.count(opt) > 0) {
          result = _options[opt];
        }
        return result;
      }

      /*
      bool Codec::saveCodecOption() {
        AVClass *c = *(AVClass**) ctx;
        const AVOption *o = c->option;
        for (; o && o->name; o++) {
          int buf_len = 2000;
          char* buf = new char[buf_len];
          av_get_string(ctx, o->name, NULL, buf, buf_len);
          LOGINFO("org.esb.av.Codec", "OptionName=" << o->name << " OptionString=" << buf);
          delete buf;
        }
      }

      bool Codec::loadCodecOption() {

      }
       */
      void Codec::reset() {
        if (_opened)
          avcodec_flush_buffers(ctx);
      }

      void Codec::setContextDefaults() {

        //        ctx->global_quality = 1000000;
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
        //        ctx->idct_algo = FF_IDCT_AUTO;
        //        ctx->skip_idct = AVDISCARD_DEFAULT;
        //        ctx->error_recognition = FF_ER_CAREFUL;
        //        ctx->error_concealment = 3;
        //        ctx->workaround_bugs = 1;
        ctx->debug = 0;
        ctx->debug_mv = 0;
        ctx->request_channels = 2;
        ctx->request_channel_layout = 2;
        if (ctx->codec_id == CODEC_ID_MPEG2VIDEO) {
          ctx->max_b_frames = 2;
          //          ctx->has_b_frames = 1;
        }
        //ctx->extradata = NULL;
        //        ctx->me_method = ME_EPZS;
        //        ctx->mb_decision = 2;
        //        ctx->bit_rate_tolerance = 4000000;
        /*default settings for x264*/
        //        ctx->me_range = 16;
        //        ctx->max_qdiff = 4;
        //        ctx->qmin = 2;
        //        ctx->qmax = 31;
        //        ctx->qcompress = 0.5;
        //        ctx->qblur = 0.5;
        /**
         * this will come from the codecfactory in the future*/
        /*
        ctx->flags |= CODEC_FLAG_AC_PRED;
        ctx->flags |= CODEC_FLAG_4MV;
        ctx->flags |= CODEC_FLAG_MV0;
        ctx->me_cmp = 2;
        ctx->me_sub_cmp = 2;
        ctx->trellis = 1;
         */

      }

      AVMediaType Codec::getCodecType() {
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
            LOGERROR("Decoder not found for id :" << ctx->codec_id);
            result = false;
          }
        } else
          if (mode == ENCODER) {
          _codec = avcodec_find_encoder(ctx->codec_id);
          if (_codec == NULL) {
            LOGERROR("Encoder not found for id :" << ctx->codec_id);
            result = false;
          }
        } else {
          LOGERROR("Mode not set for Codec");
        }
        if (result) {
          ctx->codec_type = _codec->type;
          _codec_resolved = true;
        } else {
          LOGERROR("in resolving codec id:" << ctx->codec_id);
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

      bool Codec::open() {

        //        boost::mutex::scoped_lock scoped_lock(open_close_mutex);
        boost::mutex::scoped_lock scoped_lock(ffmpeg_mutex);
        //ctx->strict_std_compliance = FF_COMPLIANCE_VERY_STRICT;
        if (_opened)return _opened;
        findCodec(_mode);
        if (!_codec) {
          LOGERROR("_codec not initialized!");
          return false;

        }
        //        if (findCodec(_mode)) {
        //          ctx = avcodec_alloc_context();
        //          setParams();
        if (false && _codec->capabilities & CODEC_CAP_TRUNCATED) {
          ctx->flags = 0; // |= CODEC_FLAG_TRUNCATED;
          cout << "CodecCapTruncated" << endl;
        }
        std::map<std::string, std::string>::iterator opit = _options.begin();
        for (; opit != _options.end(); opit++) {
          std::string opt = (*opit).first;
          std::string arg = (*opit).second;
          //LOGTRACE("av_set_string3(" << opt << "," << arg << ")");
          if (_codec->type == AVMEDIA_TYPE_AUDIO && opt == "b") {
            LOGWARN("Option b is not valid for Audio Codecs, it overwrites the Option ab");
            LOGINFO("dropping Option b for this Codec");
            continue;
          }
          int type;
          int ret = 0;
          const AVOption *o = NULL;
          //int opt_types[]={0};
          //if(_codec->type==CODEC_TYPE_VIDEO)
          int optflags=0;
          if(_codec->type == AVMEDIA_TYPE_AUDIO){
            optflags=AV_OPT_FLAG_AUDIO_PARAM;
          }else if(_codec->type == AVMEDIA_TYPE_VIDEO){
            optflags=AV_OPT_FLAG_VIDEO_PARAM;
          }
          if (_mode == ENCODER) {
            optflags|=AV_OPT_FLAG_ENCODING_PARAM;
          }else if (_mode == DECODER) {
            optflags|=AV_OPT_FLAG_DECODING_PARAM;
          }
          int opt_types[] = {optflags, 0};
          for (type = 0; type < 2 && ret >= 0; type++) {
            const AVOption *o2 = av_find_opt(ctx, opt.c_str(), NULL, opt_types[type], opt_types[type]);
            if (o2 && (o2->flags & _mode||o2->flags==0)) {
              ret = av_set_string3(ctx, opt.c_str(), arg.c_str(), 1, &o);
              LOGDEBUG("Set Codec param '" << arg << "' for option '" << opt << "' in "<<(_mode==ENCODER?"Encoder":"Decoder"));
              if(ret)
                LOGERROR("Invalid value '" << arg << "' for option '" << opt << "' in "<<(_mode==ENCODER?"Encoder":"Decoder"));
            } else {
              if(type==2)
                LOGWARN("Option not found: "<<opt.c_str())
            }
          }
          if (o && ret != 0) {
            LOGERROR("Invalid value '" << arg << "' for option '" << opt << "' in "<<(_mode==ENCODER?"Encoder":"Decoder"));
          }
          if (!o) {
            //LOGWARN("Option not found:" << opt);
          }
        }
        //setFlag(CODEC_FLAG_PSNR);
        if (_codec && _codec->type & AVMEDIA_TYPE_AUDIO) {
          setTimeBase(1, ctx->sample_rate);
        }
        if (_codec && _codec->type == AVMEDIA_TYPE_VIDEO) {
          if (_frame_rate.num == 0 && _frame_rate.den == 0) {
            _frame_rate.num = ctx->time_base.den;
            _frame_rate.den = ctx->time_base.num;
          }
        }

        try {

          if (avcodec_open(ctx, _codec) < 0) {
            LOGERROR("openning Codec (" << ctx->codec_id << ")");

          } else {
            LOGDEBUG("Codec opened:" << ctx->codec_id);
            fifo = av_fifo_alloc(1024);
            _opened = true;
          }
        } catch (...) {
          LOGERROR("Exception while openning Codec (" << ctx->codec_id << ")");
        }
        return _opened;
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
        boost::mutex::scoped_lock scoped_lock(ffmpeg_mutex);

        if (_opened) {
          LOGTRACE("void Codec::close("<<this<<"");
          //LOGINFO("Closing codec ("<<ctx->codec_id<<")");
          if (ctx) {
            if (ctx->extradata_size > 0 && !_pre_allocated) {
              av_freep(&ctx->extradata);
            }
            avcodec_close(ctx);
          }

          //          LOGDEBUG( "recently fifo size:" << av_fifo_size(fifo));
          if (fifo)
            av_fifo_free(fifo);
          //          logdebug("Codec closed:" << _codec_id);
        } else {
          //logdebug("Codec not closed, because it was not opened:" << ctx->codec_id);
        }
        if (ctx && !_pre_allocated) {
          av_free(ctx);
          ctx = NULL;
        }
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

      void Codec::setTimeBase(int num, int den) {
        ctx->time_base.num = num;
        ctx->time_base.den = den;
      }

      AVRational Codec::getTimeBase() {
        return ctx->time_base;
      }

      void Codec::setFrameRate(AVRational fr) {
        _frame_rate = fr;
      }

      void Codec::setFrameRate(int num, int den) {
        _frame_rate.num = num;
        _frame_rate.den = den;
      }

      AVRational Codec::getFrameRate() {
        return _frame_rate;
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

      int64_t Codec::getFrameBytes() {
        int osize = av_get_bits_per_sample_fmt(ctx->sample_fmt) / 8;

        return ctx->frame_size * osize * ctx->channels;
      }

      int Codec::getBitsPerCodedSample() {
        return ctx->bits_per_coded_sample;
      }

      void Codec::setBitsPerCodedSample(int v) {
        ctx->bits_per_coded_sample = v;
      }

      Format Codec::getOutputFormat() {
        _output_format.width = ctx->width;
        _output_format.height = ctx->height;
        //_output_format.pixel_format = STD_PIX_FMT;
        _output_format.pixel_format = ctx->pix_fmt;
        return _output_format;
      }

      Format Codec::getInputFormat() {
        _input_format.width = ctx->width;
        _input_format.height = ctx->height;

        //_input_format.pixel_format = STD_PIX_FMT;
        _input_format.pixel_format = ctx->pix_fmt;
        return _input_format;

      }

      void Codec::setOutputFormat(Format f) {

      }

      void Codec::setInputFormat(Format f) {

      }

      std::list<Format> Codec::getSupportedInputFormats() {
        std::list<Format> result;

        return result;
      }

      std::map<std::string, std::string> Codec::getCodecOptions() {
        std::map<std::string, std::string> result;
        const AVOption *opt = NULL;
        while ((opt = av_next_option(ctx, opt)) != NULL) {
          int len = 1000;
          char data[1000];
          av_get_string(ctx, opt->name, NULL, data, len);
          result[opt->name] = data;
        }
        return result;
      }

      /*
      void Codec::setStartTime(int64_t start) {
        _start_time = start;
      }
       */
      std::string Codec::toString() {
        using namespace org::esb::util;
        std::string data;
		data.append("Codec ID:").append(StringUtil::toString(ctx->codec_id)).append("\r\n");
        if (_opened) {
          data.append("Codec Name:").append(ctx->codec->name).append("\r\n");
          data.append("Codec Type:").append(ctx->codec_type == AVMEDIA_TYPE_AUDIO ? "AUDIO" : "VIDEO").append("\r\n");
          data.append("Width:").append(StringUtil::toString(getWidth())).append("\r\n");
          data.append("Height:").append(StringUtil::toString(getHeight())).append("\r\n");
          data.append("Channels:").append(StringUtil::toString(getChannels())).append("\r\n");
          data.append("RequestChannelLayout:").append(StringUtil::toString(ctx->request_channel_layout)).append("\r\n");
          data.append("ChannelLayout:").append(StringUtil::toString(ctx->channel_layout)).append("\r\n");
          data.append("BitRate:").append(StringUtil::toString(ctx->bit_rate)).append("\r\n");
          data.append("GOP:").append(StringUtil::toString(ctx->gop_size)).append("\r\n");
          data.append("SampleRate:").append(StringUtil::toString(getSampleRate())).append("\r\n");
          data.append("SampleFormat:").append(StringUtil::toString(getSampleFormat())).append("\r\n");
          data.append("PixelFormat:").append(StringUtil::toString(getPixelFormat())).append("\r\n");
          data.append("TimeBase:").append(StringUtil::toString(ctx->time_base.num)).append("/");
          data.append(StringUtil::toString(ctx->time_base.den)).append("\r\n");
          data.append("BFrameStrategie:").append(StringUtil::toString(ctx->b_frame_strategy)).append("\r\n");
          data.append("MaxBFrames:").append(StringUtil::toString(!!ctx->max_b_frames)).append("\r\n");
          data.append("HasBFrames:").append(StringUtil::toString(ctx->has_b_frames)).append("\r\n");
          data.append("Delay:").append(StringUtil::toString(ctx->delay)).append("\r\n");
          char buf[256];
          avcodec_string(buf, sizeof (buf), ctx, _mode);
          data.append("InternalData:").append(std::string(buf)).append("\r\n");
        }
        return data;
      }
    }
  }
}
