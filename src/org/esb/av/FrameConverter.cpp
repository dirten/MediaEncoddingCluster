
#include "Codec.h"

#include "FrameConverter.h"
#include "FrameFormat.h"
#include "Frame.h"
//#include "swscale.h"
#include <iostream>



#define MAX_AUDIO_PACKET_SIZE (128 * 1024)
using namespace std;
namespace org {
  namespace esb {
    namespace av {

      /**
       * initialize the FrameConverter with the parameters from the De/Encoder
       */
      FrameConverter::FrameConverter(Decoder * dec, Encoder * enc) {
        _swsContext = NULL;
        _audioCtx = NULL;
        _frameRateCompensateBase = 0;
        _gop_size = -1;
        int sws_flags = 1;
        _dec = dec;
        _enc = enc;
        if (dec->getCodecType() != enc->getCodecType()) {
          LOGERROR("org.esb.av.FrameConverter", "the Decoder and Encoder must be from the same Type");
        }
        if (dec->getCodecType() == CODEC_TYPE_AUDIO && enc->getCodecType() == CODEC_TYPE_AUDIO) {
          if (dec->getSampleFormat() != SAMPLE_FMT_S16)
            LOGWARN("org.esb.av.FrameConverter", "Warning, using s16 intermediate sample format for resampling\n");
          _audioCtx = av_audio_resample_init(
              enc->getChannels(), dec->ctx->request_channel_layout,
              enc->getSampleRate(), dec->getSampleRate(),
              enc->getSampleFormat(), dec->getSampleFormat(),
              16, 10, 0, 0.8 // this line is simple copied from ffmpeg
              );
          if (!_audioCtx)
            LOGERROR("org.esb.av.FrameConverter", "Could not initialize Audio Resample Context");
        }
        if (dec->getCodecType() == CODEC_TYPE_VIDEO && enc->getCodecType() == CODEC_TYPE_VIDEO) {
          _swsContext = sws_getContext(
              dec->getWidth(), dec->getHeight(), dec->getPixelFormat(),
              enc->getWidth(), enc->getHeight(), enc->getPixelFormat(),
              sws_flags, NULL, NULL, NULL);
          if (_swsContext == NULL)
            LOGERROR("org.esb.av.FrameConverter", "Could not initialize SWSCALE");
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
        LOGTRACEMETHOD("org.esb.av.FrameConverter", "Convert Frame");

        LOGDEBUG("org.esb.av.FrameConverter", in_frame.toString());
        if (_dec->getCodecType() == CODEC_TYPE_VIDEO) {
          convertVideo(in_frame, out_frame);
        }
        if (_dec->getCodecType() == CODEC_TYPE_AUDIO) {
          convertAudio(in_frame, out_frame);
        }
        rescaleTimestamp(in_frame, out_frame);
        if (_dec->getCodecType() == CODEC_TYPE_AUDIO)
          compensateAudioResampling(in_frame, out_frame);
        if (_dec->getCodecType() == CODEC_TYPE_VIDEO)
          compensateFrameRateConversion(in_frame, out_frame);
        LOGDEBUG("org.esb.av.FrameConverter", out_frame.toString());
      }

      void FrameConverter::rescaleTimestamp(Frame & in_frame, Frame & out_frame) {
#ifdef USE_TIME_BASE_Q
        out_frame.setTimeBase(in_frame.getTimeBase());
        out_frame.setPts(in_frame.getPts());
        out_frame.setDts(in_frame.getDts());
        out_frame.setDuration(in_frame.getDuration());
#else
        out_frame.setTimeBase(_enc->getTimeBase());
        out_frame.setPts(av_rescale_q(in_frame.getPts(), in_frame.getTimeBase(), _enc->getTimeBase()));
        out_frame.setDts(av_rescale_q(in_frame.getDts(), in_frame.getTimeBase(), _enc->getTimeBase()));
        out_frame.setDuration(av_rescale_q(in_frame.getDuration(), in_frame.getTimeBase(), _enc->getTimeBase()));
#endif
      }

      void FrameConverter::setFrameRateCompensateBase(double val) {
        LOGDEBUG("org.esb.av.FrameConverter", "setFrameRateCompensateBase(" << val << ")");
        _frameRateCompensateBase = val;
      }

      void FrameConverter::setGopSize(int gop) {
        LOGDEBUG("org.esb.av.FrameConverter", "setGopSize(" << gop << ")");
        _gop_size = gop;
      }

      void FrameConverter::compensateFrameRateConversion(Frame & input, Frame & out) {
        int frames = 1;
        if (_gop_size > 0||_gop_size==-1) {
          double a = static_cast<double> ((double) _dec->getLastTimeStamp() / (double) _dec->getTimeBase().den);
//          double a = static_cast<double> ((double) input.getPts() / (double) _dec->getTimeBase().den);
          double delta = _frameRateCompensateBase + a / av_q2d(_enc->getTimeBase()) - _enc->getLastTimeStamp();
          if (delta >= 2.0)
            frames = static_cast<int> (floor(delta + 0.5));
          if (delta <= -0.6)
            frames = 0; //static_cast<int> (floor(delta - 0.5));
          LOGDEBUG(
              "org.esb.av.FrameConverter",
              "inframe.pts=" << input.getPts() <<
              ":_dec->getLastTimeStamp()="<<_dec->getLastTimeStamp()<<
              ":_dec->getTimeBase().den=" << _dec->getTimeBase().den <<
              ":av_q2d(_enc->getTimeBase())=" << av_q2d(_enc->getTimeBase()) <<
              ":_enc->getLastTimeStamp()=" << _enc->getLastTimeStamp() <<
              ":vdelta=" << delta << ":frames=" << frames);
          if(_gop_size>0)
            _gop_size--;
        }
        out.setFrameCount(frames);

      }

      void FrameConverter::compensateAudioResampling(Frame & input, Frame & out) {
        /**
         * calculating the delta between the input and the output timestamps
         */
        AVRational ar;
        ar.num = 1;
        ar.den = 1000000;
        if (_dec->getLastTimeStamp() == AV_NOPTS_VALUE || _enc->getLastTimeStamp() == AV_NOPTS_VALUE)return;
        int64_t inpts = av_rescale_q(_dec->getLastTimeStamp(), _dec->getTimeBase(), _enc->getTimeBase());
        int64_t outpts = _enc->getLastTimeStamp();
        double delta = inpts - outpts - _enc->getSamplesBufferd();
        LOGDEBUG("org.esb.av.FrameConverter", "Resample Comensate delta:" << delta << " inpts:" << inpts << " outpts:" << outpts << " fifo:" << _enc->getSamplesBufferd());

        av_resample_compensate(
            *(struct AVResampleContext**) _audioCtx,
            delta,
            _enc->getSampleRate() / 2
            );
      }

      /**
       * this rescales the input Frame Data into the output Frame Data
       */
      void FrameConverter::convertVideo(Frame & in_frame, Frame & out_frame) {
        //        LOGTRACEMETHOD("org.esb.av.FrameConverter","Convert Video");
        out_frame._type = in_frame._type;
        sws_scale(
            _swsContext,
            in_frame.getAVFrame()->data,
            in_frame.getAVFrame()->linesize,
            0,
            in_frame.getHeight(),
            out_frame.getAVFrame()->data,
            out_frame.getAVFrame()->linesize
            );
        out_frame.setTimeBase(in_frame.getTimeBase());
        out_frame.pos = in_frame.pos;
        out_frame.setPts(in_frame.getPts());
        out_frame.setDts(in_frame.getDts());
        out_frame.stream_index = in_frame.stream_index;
        out_frame.duration = in_frame.duration;
      }

      /**
       * this resample the input Frame data into the output Frame data
       */
      void FrameConverter::convertAudio(Frame & in_frame, Frame & out_frame) {
        //        LOGTRACEMETHOD("org.esb.av.FrameConverter","Convert Audio");
        int isize = av_get_bits_per_sample_format(_dec->getSampleFormat()) / 8;
        int osize = av_get_bits_per_sample_format(_enc->getSampleFormat()) / 8;
        uint8_t * audio_buf = (uint8_t*) av_malloc(2 * MAX_AUDIO_PACKET_SIZE);
        int out_size = audio_resample(
            _audioCtx,
            (short *) audio_buf,
            (short *) in_frame._buffer,
            in_frame._size / (in_frame.channels * isize)
            );
        out_frame._allocated = true;
        out_frame._buffer = audio_buf;
        out_frame.pos = in_frame.pos;
        out_frame.stream_index = in_frame.stream_index;
        out_frame._size = out_size * _enc->getChannels() * osize;
        out_frame._type = CODEC_TYPE_AUDIO;
        out_frame.channels = _enc->getChannels();
        out_frame.sample_rate = _enc->getSampleRate();
      }
    }
  }
}

