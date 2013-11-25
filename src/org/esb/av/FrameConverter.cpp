#include "Codec.h"

#include "FrameConverter.h"
#include "FrameFormat.h"
#include "Frame.h"
//#include "swscale.h"
#include <iostream>
#include "org/esb/lang/Exception.h"

#define MAX_AUDIO_PACKET_SIZE (128 * 1024)
using namespace std;
namespace org {
  namespace esb {
    namespace av {
      boost::mutex FrameConverter::ctx_mutex;

      /**
       * initialize the FrameConverter with the parameters from the De/Encoder
       */
      FrameConverter::FrameConverter(Decoder * dec, Encoder * enc) {
        LOGDEBUG("FrameConverter::FrameConverter(Decoder * dec, Encoder * enc)");
        _swsContext = NULL;
        _swr_ctx=NULL;
        _audioCtx = NULL;
        _frameRateCompensateBase = 0;
        _gop_size = -1;
        _deinterlace = true;
        _keep_aspect_ratio = true;
        insamples = 0;
        outsamples = 0;
        last_insamples = 0;
        last_outsamples = 0;
        inframes = 0;
        outframes = 0;
        duplicatedframes = 0;
        inchannels = 0;
        int sws_flags = 1;
        _dec = dec;
        _enc = enc;
        if (dec->getCodecType() != enc->getCodecType()) {
          LOGERROR("the Decoder and Encoder must be from the same Type");
        }
        /*
        if (dec->getCodecType() == AVMEDIA_TYPE_AUDIO && enc->getCodecType() == AVMEDIA_TYPE_AUDIO) {
          if (dec->getSampleFormat() != SAMPLE_FMT_S16)
            LOGWARN("Warning, using s16 intermediate sample format for resampling\n");
          _audioCtx = av_audio_resample_init(enc->getChannels(), dec->getChannels(), enc->getSampleRate(), dec->getSampleRate(), enc->getSampleFormat(), dec->getSampleFormat(), 16, 10, 0, 0.8 // this line is simple copied from ffmpeg
                  );
          if (!_audioCtx)
            LOGERROR("Could not initialize Audio Resample Context");
        }*/
        Rational r;
        r.num = dec->getFrameRate().den;
        r.den = dec->getFrameRate().num;

        _frame_rate_filter = new FrameRateFilter(r, enc->getTimeBase());

        if (dec->getCodecType() == AVMEDIA_TYPE_VIDEO && enc->getCodecType() == AVMEDIA_TYPE_VIDEO) {
          Format in = dec->getOutputFormat();
          Format out = enc->getInputFormat();

          _swsContext = sws_getContext(in.width, in.height, in.pixel_format, out.width, out.height, out.pixel_format, sws_flags, NULL, NULL, NULL);
          if (_swsContext == NULL){
            LOGERROR("Could not initialize SWSCALE");
            throw org::esb::lang::Exception("Could not initialize SWSCALE");
          }
        }
      }

      FrameConverter::~FrameConverter() {
        if (_swsContext)
          sws_freeContext(_swsContext);
        _swsContext = 0;
        if (_audioCtx)
          audio_resample_close(_audioCtx);
        _audioCtx = 0;
      }

      void FrameConverter::convert(Frame & in_frame, Frame & out_frame) {
        LOGTRACEMETHOD("Convert Frame");

        LOGDEBUG(in_frame.toString());
        LOGDEBUG(out_frame.toString());
        if (_dec->getCodecType() == AVMEDIA_TYPE_VIDEO) {
          
          if (doDeinterlaceFrame(in_frame, in_frame)) {
            
          }
          convertVideo(in_frame, out_frame);

        }
        if (_dec->getCodecType() == AVMEDIA_TYPE_AUDIO) {
          convertAudio(in_frame, out_frame);
        }
        rescaleTimestamp(in_frame, out_frame);
        if (_dec->getCodecType() == AVMEDIA_TYPE_AUDIO)
          compensateAudioResampling(in_frame, out_frame);
        if (_dec->getCodecType() == AVMEDIA_TYPE_VIDEO)
          compensateFrameRateConversion(in_frame, out_frame);
        out_frame.setPixelAspectRatio(in_frame.getPixelAspectRatio());
        out_frame.setDisplayAspectRatio(in_frame.getDisplayAspectRatio());
        out_frame.setStorageAspectRatio(in_frame.getStorageAspectRatio());
        LOGDEBUG("out frame"+out_frame.toString());
      }

      void FrameConverter::rescaleTimestamp(Frame & in_frame, Frame & out_frame) {
        LOGDEBUG("Rescale timestamp");
        //LOGDEBUG("Decoder:"<<_dec->toString());
        //LOGDEBUG("Encoder:"<<_enc->getTimeBase());
#ifdef USE_TIME_BASE_Q
        out_frame.setTimeBase(in_frame.getTimeBase());
        out_frame.setPts(in_frame.getPts());
        out_frame.setDts(in_frame.getDts());
        out_frame.setDuration(in_frame.getDuration());
#else
        out_frame.setTimeBase(_enc->getTimeBase());
        out_frame.setPts(av_rescale_q(in_frame.getPts(), in_frame.getTimeBase(), _enc->getTimeBase()));
        out_frame.setDts(av_rescale_q(in_frame.getDts(), in_frame.getTimeBase(), _enc->getTimeBase()));

        //        out_frame.setDuration(av_rescale_q(in_frame.getDuration(), in_frame.getTimeBase(), _enc->getTimeBase()));
        //if(_enc->getCodecType()==CODEC_TYPE_VIDEO)
        //out_frame.setDuration(_enc->getTimeBase().num);
        //if(_enc->getCodecType()==CODEC_TYPE_AUDIO)
        //out_frame.setDuration(_enc->getTimeBase().num);
#endif
        LOGDEBUG("Rescale timestamp ready");
      }

      void FrameConverter::setDeinterlace(bool deinterlace) {
        _deinterlace = deinterlace;
      }

      void FrameConverter::setKeepAspectRatio(bool kar) {
        _keep_aspect_ratio = kar;
      }

      void FrameConverter::setFrameRateCompensateBase(double val) {
        LOGDEBUG("setFrameRateCompensateBase(" << val << ")");
        /**the +0.001 is to compensate rounding error*/
        _frameRateCompensateBase = val + 0.0001;
        _frame_rate_filter->setCompensateBase(val);
      }

      void FrameConverter::setGopSize(int gop) {
        LOGDEBUG("setGopSize(" << gop << ")");
        _gop_size = gop;
      }

      void FrameConverter::compensateFrameRateConversion(Frame & input, Frame & out) {
        _frame_rate_filter->process(input, out);
        return;
        int frames = 1;
        AVRational input_framerate = _dec->getFrameRate();
        AVRational output_framerate = _enc->getFrameRate();
        if ((_gop_size > 0 || _gop_size == -1) && (_dec->getFrameRate().num != _enc->getFrameRate().num || _dec->getFrameRate().den != _enc->getFrameRate().den)) {
          /**
           * calculating the framerate difference
           * delta=(input_timestamp/decoder_framerate_num*decoder_framerate_den*encoder_framerate_num/encoder_framerate_den)-output_timestamp/encoder_framerate_den
           *
           * */

          int64_t last_input_pts = _dec->getLastTimeStamp();
          int64_t last_output_pts = _enc->getLastTimeStamp();

          double in = (((double) last_input_pts) / input_framerate.num * input_framerate.den * output_framerate.num / output_framerate.den);
          double out = (((double) last_output_pts) / output_framerate.den);
          double delta = in - out;
          delta += _frameRateCompensateBase;

          //          double a = static_cast<double> ((double) _dec->getLastTimeStamp()*(double) _dec->getTimeBase().num / (double) _dec->getTimeBase().den);
          //          double a = static_cast<double> ((double) input.getPts() / (double) _dec->getTimeBase().den);
          //          double delta = _frameRateCompensateBase + a / av_q2d(_enc->getTimeBase()) - _enc->getLastTimeStamp();
          if (delta > 2.0)
            frames = static_cast<int> (floor(delta + 0.5));
          if (delta <= -0.6)
            frames = 0; //static_cast<int> (floor(delta - 0.5));
          LOGDEBUG(
          "inframe.pts=" << input.getPts() <<
          ":_dec->getLastTimeStamp()=" << _dec->getLastTimeStamp() <<
          ":_dec->getTimeBase().den=" << _dec->getTimeBase().den <<
          ":av_q2d(_enc->getTimeBase())=" << av_q2d(_enc->getTimeBase()) <<
          ":_enc->getLastTimeStamp()=" << _enc->getLastTimeStamp() <<
          ":vdelta=" << delta << ":frames=" << frames);
          if (_gop_size > 0)
            _gop_size--;
        }
        out.setFrameCount(frames);
        return;
        inframes++; //=_dec->getLastTimeStamp();

        //outframes=av_rescale_q(inframes,  output_framerate,input_framerate);
        outframes = ((((inframes * _dec->getFrameRate().den) / _dec->getFrameRate().num) / _enc->getFrameRate().den) * _enc->getFrameRate().num);
        outframes += _frameRateCompensateBase;
        outframes -= 1;

        LOGDEBUG("inframes=" << inframes << " outframes=" << outframes << " duplicates=" << duplicatedframes);
        LOGDEBUG("CompensateBase=" << _frameRateCompensateBase);
        LOGDEBUG("calculated_value=" << outframes - inframes - duplicatedframes);
        LOGDEBUG("DecoderTimeBase=" << _dec->getTimeBase().num << "/" << _dec->getTimeBase().den);
        LOGDEBUG("EncoderFrameRate=" << _enc->getFrameRate().num << "/" << _enc->getFrameRate().den);
        LOGDEBUG("EncoderTimeBase=" << _enc->getTimeBase().num << "/" << _enc->getTimeBase().den);
        if ((outframes - inframes - duplicatedframes) >= 1.0) {
          out.setFrameCount(outframes - inframes - duplicatedframes + 1);
          duplicatedframes = outframes - inframes;
          //inframes+=outframes-inframes;
        } else
          if ((outframes - inframes - duplicatedframes - 0.0001) <= -1.0) {
            out.setFrameCount(0);
            duplicatedframes = outframes - inframes - 0.0001;
            //inframes+=outframes-inframes;
          }
        double tmp = 0;
        _frameRateCompensateBase = modf(outframes, &tmp);

        //out.setFrameCount(frames);

      }

      bool FrameConverter::doDeinterlaceFrame(Frame & in_frame, Frame & out_frame) {
        bool result = false;
        LOGDEBUG("deinterlacing frame");
        int size = avpicture_get_size(_dec->getPixelFormat(),_dec->getWidth(),_dec->getHeight());
        uint8_t * buf=static_cast<uint8_t*>(av_malloc(size));
        AVPicture *pic;
        AVPicture pic_tmp;
        pic=&pic_tmp;
        avpicture_fill(pic,buf,_dec->getPixelFormat(),_dec->getWidth(),_dec->getHeight());
        /* deinterlace : must be done before any resize */
        if (avpicture_deinterlace(pic, (const AVPicture*) in_frame.getAVFrame(), _dec->getPixelFormat(), _dec->getWidth(), _dec->getHeight()) < 0) {
          /* if error, do not deinterlace */
          av_free(buf);
          result = false;
        } else {
          *((AVPicture*)in_frame.getAVFrame())=*pic;
          av_free(in_frame._buffer);
          in_frame._buffer=buf;
          result = true;
        }
        LOGDEBUG("deinterlacing frame ready");
        return result;
      }

      void FrameConverter::compensateAudioResampling(Frame & input, Frame & out) {
        /**
         * calculating the delta between the input and the output timestamps
         */
        AVRational ar;
        ar.num = 1;
        ar.den = 1000000;
        if (_dec->getLastTimeStamp() == AV_NOPTS_VALUE || _enc->getLastTimeStamp() == AV_NOPTS_VALUE)
          return;
        int64_t inpts = av_rescale_q(_dec->getLastTimeStamp(), _dec->getTimeBase(), _enc->getTimeBase());
        int64_t outpts = _enc->getLastTimeStamp();
        double delta = inpts - outpts - _enc->getSamplesBufferd();
        LOGDEBUG("Resample Comensate delta:" << delta << " inpts:" << inpts << " outpts:" << outpts << " fifo:" << _enc->getSamplesBufferd());

        last_insamples = av_rescale_q(last_insamples, _dec->getTimeBase(), _enc->getTimeBase());
        insamples += last_insamples;
        outsamples += last_outsamples;
        delta = last_insamples - last_outsamples;
        LOGDEBUG("new Resample Comensate delta:" << delta << " last_insamples:" << last_insamples << " last_outsamples:" << last_outsamples << " all_insamples" << insamples << " alloutsamples:" << outsamples << " lastdiff:" << (last_insamples - last_outsamples) << " alldiff:" << (insamples - outsamples));

        av_resample_compensate(*(struct AVResampleContext**) _audioCtx, delta, _enc->getSampleRate() / 2);
      }

      /**
       * this rescales the input Frame Data into the output Frame Data
       */
      void FrameConverter::convertVideo(Frame & in_frame, Frame & out_frame) {
        boost::mutex::scoped_lock scoped_lock(ctx_mutex);
        out_frame._type = in_frame._type;
        if (!_swsContext) {
          LOGERROR("sws Context not initialised");
          return;
        }
        LOGDEBUG("CONVERT VIDEO");
        LOGDEBUG(in_frame.toString());
        sws_scale(_swsContext, in_frame.getAVFrame()->data, in_frame.getAVFrame()->linesize, 0, in_frame.getHeight(), out_frame.getAVFrame()->data, out_frame.getAVFrame()->linesize);
        out_frame.setTimeBase(in_frame.getTimeBase());
        out_frame.pos = in_frame.pos;
        out_frame.setPts(in_frame.getPts());
        out_frame.setDts(in_frame.getDts());
        out_frame.stream_index = in_frame.stream_index;
        out_frame.duration = in_frame.duration;
        LOGDEBUG(out_frame.toString());
        LOGDEBUG("CONVERT video READY");
      }

      /**
       * this resample the input Frame data into the output Frame data
       */
      void FrameConverter::convertAudio2(Frame & in_frame, Frame & out_frame) {
        if (_swr_ctx== NULL || inchannels != in_frame.channels) {
          inchannels = in_frame.channels;
          if(_swr_ctx) swr_free(&_swr_ctx);
          _swr_ctx = swr_alloc();

          /* set options */
              av_opt_set_int(_swr_ctx, "in_channel_layout",    _dec->getChannelLayout(), 0);
              av_opt_set_int(_swr_ctx, "in_sample_rate",       _dec->getSampleRate(), 0);
              av_opt_set_sample_fmt(_swr_ctx, "in_sample_fmt", _dec->getSampleFormat(), 0);

              av_opt_set_int(_swr_ctx, "out_channel_layout",    _enc->getChannelLayout(), 0);
              av_opt_set_int(_swr_ctx, "out_sample_rate",       _enc->getSampleRate(), 0);
              av_opt_set_sample_fmt(_swr_ctx, "out_sample_fmt", _enc->getSampleFormat(), 0);

        }
        if (!_swr_ctx) {
          throw org::esb::lang::Exception(__FILE__,__LINE__,"Could not initialize SWResample Context");
        }
        int isize = av_get_bytes_per_sample(_dec->getSampleFormat());
        int osize = av_get_bytes_per_sample(_enc->getSampleFormat());
        uint8_t * audio_buf = (uint8_t*) av_malloc(2 * MAX_AUDIO_PACKET_SIZE);
        /*
        int ret = swr_convert(_swr_ctx, audio_buf, dst_nb_samples, (const uint8_t **)src_data, src_nb_samples);
                if (ret < 0) {
                  throw org::esb::lang::Exception(__FILE__,__LINE__,"Could not initialize SWResample Context");
                }*/
      }

      void FrameConverter::convertAudio(Frame & in_frame, Frame & out_frame) {

        if (_audioCtx == NULL || inchannels != in_frame.channels) {
          inchannels = in_frame.channels;
          if (_audioCtx)audio_resample_close(_audioCtx);
          if (_dec->getCodecType() == AVMEDIA_TYPE_AUDIO && _enc->getCodecType() == AVMEDIA_TYPE_AUDIO) {
            if (_dec->getSampleFormat() != AV_SAMPLE_FMT_S16){
              //_dec->setSampleFormat(AV_SAMPLE_FMT_S16);
              LOGWARN("Warning, using s16 intermediate sample format for resampling\n");
            }
            _audioCtx = av_audio_resample_init(_enc->getChannels(), _dec->getChannels(), _enc->getSampleRate(), _dec->getSampleRate(), _enc->getSampleFormat(), _dec->getSampleFormat(), 16, 10, 0, 0.8 );
            if (!_audioCtx){
              LOGERROR("Could not initialize Audio Resample Context");
              throw org::esb::lang::Exception(__FILE__,__LINE__,"Could not initialize Audio Resample Context");
            }
          }
        }

        if (_swr_ctx== NULL || inchannels != in_frame.channels) {
          inchannels = in_frame.channels;
          if(_swr_ctx) swr_free(&_swr_ctx);
          _swr_ctx = swr_alloc();
          if (!_swr_ctx) {
            throw org::esb::lang::Exception(__FILE__,__LINE__,"Could not initialize SWResample Context");
          }
        }


        //        LOGTRACEMETHOD("org.esb.av.FrameConverter","Convert Audio");
        boost::mutex::scoped_lock scoped_lock(ctx_mutex);
        if (!_audioCtx) {
          LOGERROR("_audioCtx not initialised");
          return;
        }
        LOGDEBUG("convert audio");
        int isize = av_get_bits_per_sample_fmt(_dec->getSampleFormat()) / 8;
        int osize = av_get_bits_per_sample_fmt(_enc->getSampleFormat()) / 8;
        uint8_t * audio_buf = (uint8_t*) av_malloc(2 * MAX_AUDIO_PACKET_SIZE);

        //int out_size = audio_resample(_audioCtx, (short *) audio_buf, (short *) in_frame.getAVFrame()->data[0], in_frame.getSampleCount() / (in_frame.channels * isize));
        int out_size = audio_resample(_audioCtx, (short *) audio_buf, (short *) in_frame._buffer, in_frame._size / (in_frame.channels * isize));
        if(out_size==0){
          LOGERROR("Audio resample failed");
          memcpy(audio_buf,in_frame._buffer,in_frame._size);
        }

        out_frame._allocated = true;
        out_frame._buffer = audio_buf;
        out_frame.pos = in_frame.pos;
        out_frame.stream_index = in_frame.stream_index;
        out_frame._size = in_frame._size;//out_size * _enc->getChannels() * osize;
        out_frame._type = AVMEDIA_TYPE_AUDIO;
        out_frame.channels = _enc->getChannels();
        out_frame.sample_rate = _enc->getSampleRate();
        out_frame.setDuration(out_size);
        last_insamples = in_frame._size / ((in_frame.channels / _enc->getChannels()) * isize);
        last_outsamples = out_frame._size / osize;
        LOGDEBUG("convert audio ready")
      }
    }
  }
}

