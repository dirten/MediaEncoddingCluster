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
#include "org/esb/util/Foreach.h"
#include "FormatBaseStream.h"
#include "CodecPropertyTransformer.h"


using namespace std;

//void __attribute__ ((constructor)) my_init (void);

void my_init() {
  using namespace org::esb::av;
  //	cout << "test initialised"<<endl;
  //	logdebug("init av package");
  av_register_all();
  //avcodec_init();
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
        _stream_index=-1;
        _dict=NULL;
        //boost::mutex::scoped_lock scoped_lock(ffmpeg_mutex);
        LOGWARN("!!!PLEASE DONT USE THIS CONSTRUCTOR!!!");
        //ctx = s->codec;
        ctx = avcodec_alloc_context();
        avcodec_copy_context(ctx, s->codec);
        _mode = mode;
        ctx->codec_id=s->codec->codec_id;//codec->id;
        findCodec(mode);
        //        ctx->codec = _codec;
        _opened = false;
        _pre_allocated = false;
        emptyFrameIsEOF=false;


        //ctx->request_channels = 2;
        //ctx->request_channel_layout = AV_CH_LAYOUT_STEREO;
        _bytes_discard = 0;

        const AVOption * option = NULL;
        while (option = av_opt_next(s->codec, option)) {
          if (option->offset > 0) {

            /*jump over deprecated options*/
            if (
            strcmp(option->name, "lpc_coeff_precision") == 0 ||
            strcmp(option->name, "prediction_order_method") == 0 ||
            strcmp(option->name, "min_partition_order") == 0 ||
            strcmp(option->name, "max_partition_order") == 0 ||
            strcmp(option->name, "sub_charenc") == 0 ||
            strcmp(option->name, "lpc_type") == 0 ||
            strcmp(option->name, "drc_scale") == 0 ||
            strcmp(option->name, "lpc_passes") == 0
            )continue;

            int len = 1000;
            char data[1000];
            av_get_string(s->codec, option->name, NULL, data, len);
            if (strlen(data) > 0&&atoi(data)>=0) {
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
        _frame_rate=av_guess_frame_rate(NULL,s,NULL);
        //		_codec_resolved=false;
      }

      Codec::Codec(int mode) : _mode(mode) {
        _stream_index=-1;

        _dict=NULL;
        //        logdebug("Codec::Codec()");
        ctx = avcodec_alloc_context();
        setContextDefaults();
        _opened = false;
        _codec_resolved = false;
        _pre_allocated = false;
        _bytes_discard = 0;
        _frame_rate.num = 0;
        _frame_rate.den = 0;
        emptyFrameIsEOF=false;

        fifo = NULL;
      }

      Codec::Codec(std::map<std::string, std::string> data,int mode): _mode(mode)
      {
        _stream_index=-1;

        _dict=NULL;
        _codec_resolved = false;
        _opened = false;
        _mode = mode;
        emptyFrameIsEOF=false;

        setCodecOption("codec_name", data["codec_id"]);
        LOGDEBUG("Search for codec:"<<data["codec_id"]);
        ctx = avcodec_alloc_context();
        _codec = findCodecByName(data["codec_id"], mode);
        if (_codec) {
          LOGDEBUG("Code Name:" << _codec->name);
          avcodec_get_context_defaults2(ctx, _codec->type);
          ctx->codec_id = _codec->id;
          setContextDefaults();
        }else{
          LOGERROR("Codec Not Found:"<<data["codec_id"]);
        }
        fifo = NULL;
        _pre_allocated = false;
        _frame_rate.num = 0;
        _frame_rate.den = 0;
        _bytes_discard = 0;

        //ctx->codec_id = data["codec_id"];
        /*
          findCodec(mode);
          if (_codec_resolved) {
            //avcodec_get_context_defaults3(ctx, _codec);
            avcodec_get_context_defaults2(ctx, _codec->type);
            //LOGERROR("error in setting defaults for the codec");

          }
*/
        org::esb::av::CodecPropertyTransformer transformer(data);
        std::map<std::string, std::string> params = transformer.getCodecProperties();
        typedef std::map<std::string, std::string> Parameter;
        foreach(Parameter::value_type param, params) {
          LOGDEBUG("Parameter key=" << param.first << " value=" << param.second);
          setCodecOption(param.first, param.second);
          //sdata.pass2encoder->setCodecOption(param.first, param.second);
        }
      }

      Codec::Codec(const Codec & cp)
      {
        _stream_index=-1;
        emptyFrameIsEOF=false;

        _dict=NULL;
        _codec_resolved = false;
        _opened = false;
        ctx = avcodec_alloc_context();
        ctx->codec_id=cp.ctx->codec_id;
        _mode=cp._mode;
        ctx->flags=cp.ctx->flags;
        ctx->pix_fmt=cp.ctx->pix_fmt;
        ctx->width=cp.ctx->width;
        ctx->height=cp.ctx->height;
        ctx->time_base=cp.ctx->time_base;
        _frame_rate=cp._frame_rate;
        ctx->gop_size=cp.ctx->gop_size;
        ctx->bit_rate=cp.ctx->bit_rate;
        ctx->channels=cp.ctx->channels;
        ctx->sample_rate=cp.ctx->sample_rate;
        ctx->sample_fmt=cp.ctx->sample_fmt;
        _bytes_discard=cp._bytes_discard;
        ctx->bits_per_coded_sample=cp.ctx->bits_per_coded_sample;
        _options=cp._options;
        ctx->codec_type=cp.ctx->codec_type;
        _stream_index=cp._stream_index;

        if (_mode == Codec::DECODER) {
          ctx->extradata_size=cp.ctx->extradata_size;
          //LOGDEBUG("Extra data"<<ctx->extradata_size);
          if (ctx->extradata_size > 0) {
            ctx->extradata = static_cast<boost::uint8_t*> (av_malloc(ctx->extradata_size + FF_INPUT_BUFFER_PADDING_SIZE));
            memset(ctx->extradata, 0, ctx->extradata_size + FF_INPUT_BUFFER_PADDING_SIZE);
            memcpy(ctx->extradata, cp.ctx->extradata, ctx->extradata_size);
            //boost::serialization::make_binary_object(ctx->extradata, ctx->extradata_size);
          } else {
            ctx->extradata = NULL;
          }
        }
      }

      Codec::Codec(std::string codec_name, int mode) {
        _stream_index=-1;
        emptyFrameIsEOF=false;

        _dict=NULL;
        _codec_resolved = false;
        _opened = false;
        _mode = mode;
        setCodecOption("codec_name", codec_name);
        ctx = avcodec_alloc_context();
        _codec = findCodecByName(codec_name, mode);
        if (_codec) {
          LOGDEBUG("Code Name:" << _codec->name);
          avcodec_get_context_defaults2(ctx, _codec->type);
          ctx->codec_id = _codec->id;
          setContextDefaults();
        }
        fifo = NULL;
        _pre_allocated = false;
        _frame_rate.num = 0;
        _frame_rate.den = 0;
        _bytes_discard = 0;


      }

      Codec::Codec(const CodecID codecId, int mode) {
        _stream_index=-1;
        emptyFrameIsEOF=false;

        _dict=NULL;
        //        logdebug("Codec::Codec(const CodecID codecId=" << codecId << ", int mode=" << mode << ")");
        //boost::mutex::scoped_lock scoped_lock(ffmpeg_mutex);
        _codec_resolved = false;
        _mode = mode;
        _opened = false;
        ctx = avcodec_alloc_context();
        //return;
        ctx->codec_id = codecId;
        if (codecId>-1) {
          findCodec(mode);
          if (_codec_resolved) {
            //avcodec_get_context_defaults3(ctx, _codec);
            avcodec_get_context_defaults2(ctx, _codec->type);
            //LOGERROR("error in setting defaults for the codec");

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
        LOGTRACE("setCodecOption(" << opt << "," << arg << ")");
        _options[opt] = arg;
        //av_dict_set(&_dict,opt.c_str(),arg.c_str(),0);
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
        
        return;
        //        ctx->global_quality = 1000000;
        ctx->pix_fmt = (PixelFormat) 0;
        ctx->width = 0;
        ctx->height = 0;
        ctx->bit_rate = 0;
        ctx->time_base.num = 0;
        ctx->time_base.den = 0;
        ctx->gop_size = 0;
        ctx->sample_rate = 0;
        ctx->sample_fmt = AV_SAMPLE_FMT_NONE;
        ctx->channels = 0;
        //        ctx->idct_algo = FF_IDCT_AUTO;
        //        ctx->skip_idct = AVDISCARD_DEFAULT;
        //        ctx->error_recognition = FF_ER_CAREFUL;
        //        ctx->error_concealment = 3;
        //        ctx->workaround_bugs = 1;
        ctx->debug = 0;
        ctx->debug_mv = 0;
        //ctx->request_channels = 2;
        //ctx->request_channel_layout = AV_CH_LAYOUT_STEREO;
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

      const char *Codec::getCodecName() {
        return _codec->name;
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

      AVCodec * Codec::findCodecByName(std::string name, int mode) {
        AVCodec* result = NULL;
        //        if(_codec_resolved)return result;
        //        logdebug("try to find " << (mode == DECODER ? "Decoder" : "Encoder") << " with id:" << _codec_id);
        if (mode == DECODER) {
          result = avcodec_find_decoder_by_name(name.c_str());
          if (result == NULL) {
            LOGERROR("Decoder not found for name :" << name);
          }
        } else
          if (mode == ENCODER) {
            result = avcodec_find_encoder_by_name(name.c_str());
            if (result == NULL) {
              LOGERROR("Encoder not found for id :" << name);
            }
          } else {
            LOGERROR("Mode not set for Codec");
          }
        if (result) {
          ctx->codec_type = result->type;
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
        ctx->strict_std_compliance = FF_COMPLIANCE_EXPERIMENTAL;
        if (_opened) {
          LOGINFO("Codec is allready openned! codec id"<<getCodecId());
          return _opened;
        }

        if (_options.count("codec_name")==0) {
          findCodec(_mode);
        } else {
          _codec = findCodecByName(_options["codec_name"], _mode);
        }

        if (!_codec) {
          LOGERROR("_codec not initialized!");
          return false;
        }

        if (getCodecType() == AVMEDIA_TYPE_VIDEO) {
          if (_options.count("width"))
            setWidth(atoi((*_options.find("width")).second.c_str()));
          if (_options.count("height"))
            setHeight(atoi((*_options.find("height")).second.c_str()));
          setPixelFormat((PixelFormat) 0);
          if (_codec->pix_fmts) {
            setPixelFormat(_codec->pix_fmts[0]);
          }
        }

        if (getCodecType() == AVMEDIA_TYPE_AUDIO) {
          if (_codec->sample_fmts) {
            setSampleFormat(_codec->sample_fmts[0]);
          }
        }

        std::map<std::string, std::string>::iterator opit = _options.begin();
        for (;opit != _options.end(); opit++) {
          av_dict_set(&_dict,(*opit).first.c_str(),(*opit).second.c_str(),0);
        }

        if (_codec && _codec->type & AVMEDIA_TYPE_AUDIO) {
          AVDictionaryEntry *t = NULL;
          t = av_dict_get(_dict, "ar", t, AV_DICT_IGNORE_SUFFIX);
          if(t){
            setTimeBase(1, atoi(t->value));
          }else{
            LOGERROR("no sample_rate found for audio codec");
          }
        }
        if (_codec && _codec->type == AVMEDIA_TYPE_VIDEO) {
          if (_frame_rate.num == 0 && _frame_rate.den == 0) {
            _frame_rate.num = ctx->time_base.den;
            _frame_rate.den = ctx->time_base.num;
          }
        }

        try {
          ctx->thread_count = 1;
          setCodecOption("threads","1");
          av_dict_set(&_dict,"threads","1",0);
          av_dict_set(&_dict,"thread_type","0",0);
          //av_dict_set(&_dict,"channel_layout",("0x"+org::esb::util::StringUtil::toString(ctx->channel_layout)).c_str(),0);
          {
            AVDictionaryEntry *t = NULL;
            while ((t = av_dict_get(_dict, "", t, AV_DICT_IGNORE_SUFFIX)))
              LOGDEBUG((_mode == ENCODER ? "Encoder" : "Decoder")<<"Setting CodecDictionary Key:"<<t->key<<" val:"<<t->value);
          }
          //ctx->channel_layout=AV_CH_LAYOUT_STEREO;
          LOGDEBUG("channel bfore open:"<<ctx->channels);
          //ctx->refcounted_frames=1;
          //ctx->channel_layout=AV_CH_LAYOUT_STEREO;

          if (avcodec_open2(ctx, _codec, &_dict) < 0) {
            LOGERROR("error in openning Codec (" << ctx->codec_id << ")");
          } else {
            std::string codec_mode = (_mode == ENCODER) ? "Encoder" : "Decoder";
            LOGDEBUG(codec_mode << " opened:" << ctx->codec_id<<"("<<this<<")");
            LOGDEBUG("CodecThreadCount="<<ctx->thread_count);
            //fifo = av_fifo_alloc(1024);
            _opened = true;
          }
          LOGDEBUG("channel after open:"<<ctx->channels);
          AVDictionaryEntry *t = NULL;
          while ((t = av_dict_get(_dict, "", t, AV_DICT_IGNORE_SUFFIX)))
            LOGDEBUG((_mode == ENCODER ? "Encoder" : "Decoder")<<"Invalid CodecDictionary Key:"<<t->key<<" val:"<<t->value);
          av_dict_free(&_dict);
          //av_dict_set(dst, t->key, t->value, flags);
        } catch (...) {
          LOGERROR("Exception while openning Codec (" << ctx->codec_id << ")");
        }

        LOGDEBUG("channel layout after open :"<<ctx->channel_layout)
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
        //boost::mutex::scoped_lock scoped_lock(ffmpeg_mutex);

        //if (_opened && avcodec_is_open(ctx)) {
        LOGDEBUG(((_mode == ENCODER) ? "Encoder" : "Decoder")<<" Codec::close(" << this << ")");
        //LOGINFO("Closing codec ("<<ctx->codec_id<<")");
        if (ctx) {
          if (ctx->extradata_size > 0 ){
            av_freep(&ctx->extradata);
          }
          //ctx->thread_count = 1;
          avcodec_close(ctx);
        }
        //          LOGDEBUG( "recently fifo size:" << av_fifo_size(fifo));
        //if (fifo)
        //av_fifo_free(fifo);
        //          logdebug("Codec closed:" << _codec_id);
        //} else {
        //  LOGDEBUG("Codec not closed, because it was not opened:" << ctx <<"("<<this<<")");
        //}
        LOGDEBUG("free context");
        if (ctx ) {
          av_free(ctx);
          ctx = NULL;
        }
        _opened = false;
        LOGDEBUG("Codec closed:"<<this);
      }

      void Codec::setWidth(int w) {
        setCodecOption("width", org::esb::util::StringUtil::toString(w));
        ctx->width = w;
      }

      void Codec::setHeight(int h) {
        setCodecOption("height", org::esb::util::StringUtil::toString(h));
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
        setCodecOption("time_base",org::esb::util::StringUtil::toString(num)+"/"+org::esb::util::StringUtil::toString(den));
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
        setCodecOption("ar", org::esb::util::StringUtil::toString(rate));
      }

      void Codec::setSampleFormat(AVSampleFormat f) {
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

      AVSampleFormat Codec::getSampleFormat() {
        return ctx->sample_fmt;
      }

      int Codec::getChannels() {
        return ctx->channels;
      }

      int64_t Codec::getChannelLayout(){
        if(!ctx->channel_layout){
          ctx->channel_layout=av_get_default_channel_layout(ctx->channels);
        }
        return ctx->channel_layout;
      }
      void Codec::setChannelLayout(int64_t l){
        ctx->channel_layout=l;
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
          if (strcmp(opt->name, "lpc_coeff_precision") == 0 ||
          strcmp(opt->name, "prediction_order_method") == 0 ||
          strcmp(opt->name, "min_partition_order") == 0 ||
          strcmp(opt->name, "max_partition_order") == 0 ||
          strcmp(opt->name, "lpc_type") == 0 ||
          strcmp(opt->name, "drc_scale") == 0 ||
          strcmp(opt->name, "lpc_passes") == 0
          )continue;
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
          data.append("Codec Name:").append(_codec->name).append("\r\n");
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
          //data.append("CRF:").append(StringUtil::toString(ctx->crf)).append("\r\n");
          char buf[256];
          avcodec_string(buf, sizeof (buf), ctx, _mode);
          data.append("InternalData:").append(std::string(buf)).append("\r\n");
        }
        return data;
      }

      void Codec::setStreamIndex(int idx)
      {
        _stream_index=idx;
      }
      int Codec::getStreamIndex(){
        return _stream_index;
      }

    }
  }
}
