
#include "Codec.h"

#include "FrameConverter.h"
#include "FrameFormat.h"
#include "Frame.h"
//#include "swscale.h"
#include <iostream>

#ifdef DEBUG
#undef DEBUG
#endif

#define MAX_AUDIO_PACKET_SIZE (128 * 1024)
using namespace std;
namespace org {
  namespace esb {
    namespace av {

      FrameConverter::FrameConverter(FrameFormat & out_format) {
        _swsContext = 0;
        _outFormat = &out_format;

      }

      FrameConverter::FrameConverter(Decoder * dec, Encoder * enc) {
        _swsContext = NULL;
        _audioCtx = NULL;
        int sws_flags = 1;
        _dec = dec;
        _enc = enc;
        if (dec->getCodecType() == CODEC_TYPE_AUDIO && enc->getCodecType() == CODEC_TYPE_AUDIO) {
          _audioCtx = av_audio_resample_init(
              enc->getChannels(), dec->getChannels(),
              enc->getSampleRate(), dec->getSampleRate(),
              enc->getSampleFormat(), dec->getSampleFormat(),
              16, 10, 0, 0.8 // this line is simple copied from ffmpeg
              );
          if (!_audioCtx)
            logerror("Could not initialize Audio Resample Context");
        }
        if (dec->getCodecType() == CODEC_TYPE_VIDEO && enc->getCodecType() == CODEC_TYPE_VIDEO) {
          _swsContext = sws_getContext(
              dec->getWidth(), dec->getHeight(), dec->getPixelFormat(),
              enc->getWidth(), enc->getHeight(), enc->getPixelFormat(),
              sws_flags, NULL, NULL, NULL);
          if (_swsContext == NULL)
            logerror("Could not initialize SWSCALE");
        }
      }

      /*
      FrameConverter::FrameConverter(FrameFormat & in_format, FrameFormat & out_format) {
        _swsContext = 0;
        _outFormat = &out_format;
        _inFormat = &in_format;
        int sws_flags = 1;

        _audioCtx = av_audio_resample_init(
            _outFormat->channels,
            _inFormat->channels,
            _outFormat->samplerate,
            _inFormat->samplerate,
            SAMPLE_FMT_S16,
            SAMPLE_FMT_S16,
            16,
            10,
            0,
            0.8
            );

        _swsContext = sws_getContext(
            _inFormat->width, _inFormat->height, _inFormat->pixel_format,
            _outFormat->width, _outFormat->height, _outFormat->pixel_format,
            sws_flags, NULL, NULL, NULL);

      }
       */
      FrameConverter::~FrameConverter() {
        if (_swsContext)
          sws_freeContext(_swsContext);
        _swsContext = 0;
        if (_audioCtx)
          audio_resample_close(_audioCtx);
        _audioCtx = 0;
      }

      Frame FrameConverter::convert(Frame & in_frame) {
        if (in_frame._type == CODEC_TYPE_VIDEO) {
          return convertVideo(in_frame);
        }
        if (in_frame._type == CODEC_TYPE_AUDIO) {
          //          return convertAudio(in_frame);
        }
        return in_frame;
      }

      void FrameConverter::convert(Frame & in_frame, Frame & out_frame) {
#ifdef DEBUG
        logdebug(in_frame.toString());
#endif
        if (_dec->getCodecType() == CODEC_TYPE_VIDEO) {
          convertVideo(in_frame, out_frame);
        }
        if (_dec->getCodecType() == CODEC_TYPE_AUDIO) {
          convertAudio(in_frame, out_frame);
        }
        rescaleTimestamp(in_frame, out_frame);
#ifdef DEBUG
        logdebug(out_frame.toString());
#endif
      }

      void FrameConverter::rescaleTimestamp(Frame & in_frame, Frame & out_frame) {
        out_frame.setTimeBase(_enc->getTimeBase());
        //        return;
        out_frame.setPts(av_rescale_q(in_frame.getPts(), in_frame.getTimeBase(), _enc->getTimeBase()));
        out_frame.setDts(av_rescale_q(in_frame.getDts(), in_frame.getTimeBase(), _enc->getTimeBase()));
        out_frame.setDuration(av_rescale_q(in_frame.getDuration(), in_frame.getTimeBase(), _enc->getTimeBase()));

      }

      Frame FrameConverter::convertVideo(Frame & in_frame) {
        Frame out_frame(_enc->getPixelFormat(), _enc->getWidth(), _enc->getHeight());
        out_frame._type = in_frame._type;

        int sws_flags = 1;
        /*
                _swsContext = sws_getCachedContext(_swsContext,
                    in_frame.getWidth(), in_frame.getHeight(),
                    (PixelFormat) in_frame.getFormat(),
                    _outFormat->width, _outFormat->height,
                    _outFormat->pixel_format, sws_flags, NULL, NULL, NULL);
         */

        sws_scale(_swsContext, in_frame.getAVFrame()->data, in_frame.getAVFrame()->linesize, 0, in_frame.getHeight(), out_frame.getAVFrame()->data, out_frame.getAVFrame()->linesize);
        //        out_frame.setTimeBase(in_frame.getTimeBase());
        out_frame.pos = in_frame.pos;
        out_frame.setPts(in_frame.getPts());
        out_frame.setDts(in_frame.getDts());
        out_frame.stream_index = in_frame.stream_index;
        out_frame.duration = in_frame.duration;
        return out_frame;
      }

      void FrameConverter::convertVideo(Frame & in_frame, Frame & out_frame) {
        out_frame._type = in_frame._type;

        int sws_flags = 1;
        /*
                _swsContext = sws_getCachedContext(_swsContext,
                    in_frame.getWidth(), in_frame.getHeight(),
                    (PixelFormat) in_frame.getFormat(),
                    _outFormat->width, _outFormat->height,
                    _outFormat->pixel_format, sws_flags, NULL, NULL, NULL);
         */

        sws_scale(_swsContext, in_frame.getAVFrame()->data, in_frame.getAVFrame()->linesize, 0, in_frame.getHeight(), out_frame.getAVFrame()->data, out_frame.getAVFrame()->linesize);
        out_frame.setTimeBase(in_frame.getTimeBase());
        out_frame.pos = in_frame.pos;
        out_frame.setPts(in_frame.getPts());
        out_frame.setDts(in_frame.getDts());
        out_frame.stream_index = in_frame.stream_index;
        out_frame.duration = in_frame.duration;
        //        return out_frame;
      }

      /*
      Frame FrameConverter::convertAudio(Frame & in_frame) {
        //		return in_frame;
        int isize = av_get_bits_per_sample_format(_dec->getSampleFormat()) / 8;
        uint8_t * audio_buf = new uint8_t[2 * MAX_AUDIO_PACKET_SIZE];
        int delta = _dec->getSampleRate() - _enc->getSampleRate();


        //        av_resample_compensate(*(struct AVResampleContext**) _audioCtx, -delta, _enc->getSampleRate());
        //        fprintf(stderr, "audio resample size %d channels %d isize %d\n",in_frame._size ,in_frame.channels , isize);
        int out_size = audio_resample(
            _audioCtx,
            (short *) audio_buf,
            (short *) in_frame._buffer,
            in_frame._size / (in_frame.channels * isize)
            );
        //		audio_resample_close( reCtx );

        Frame frame(audio_buf);
        //        frame._buffer = audio_buf;
        frame.setTimeBase(in_frame.getTimeBase());
        frame.setPts(in_frame.getPts());
        frame.setDts(in_frame.getDts());
        frame.pos = in_frame.pos;
        frame.stream_index = in_frame.stream_index;
        frame.duration = in_frame.duration;
        frame._size = out_size;
        frame._type = CODEC_TYPE_AUDIO;
        frame.channels = _enc->getChannels();
        frame.sample_rate = _enc->getSampleRate();

        return frame;
      }
       */
      void FrameConverter::convertAudio(Frame & in_frame, Frame & out_frame) {
        //		return in_frame;
        int isize = av_get_bits_per_sample_format(_dec->getSampleFormat()) / 8;
        int osize = av_get_bits_per_sample_format(_enc->getSampleFormat()) / 8;
        //        uint8_t * audio_buf = new uint8_t[2 * MAX_AUDIO_PACKET_SIZE];
        uint8_t * audio_buf = (uint8_t*) av_malloc(2 * MAX_AUDIO_PACKET_SIZE);
        //        int delta = _dec->getSampleRate() - _enc->getSampleRate();


        //        av_resample_compensate(*(struct AVResampleContext**) _audioCtx, -delta, _enc->getSampleRate());
        //        fprintf(stderr, "audio resample size %d channels %d isize %d\n",in_frame._size ,in_frame.channels , isize);
        int out_size = audio_resample(
            _audioCtx,
            (short *) audio_buf,
            (short *) in_frame._buffer,
            in_frame._size / (in_frame.channels * isize)
            );
        //		audio_resample_close( reCtx );
        //		logdebug("ResampleContextInChannels:");
        //        logdebug("OutSize:"<<out_size<<":SampleSize:"<<(in_frame._size / (in_frame.channels * isize)));
        //        Frame frame(audio_buf);
        out_frame._allocated = true;
        out_frame._buffer = audio_buf;
        //        out_frame.setTimeBase(in_frame.getTimeBase());
        out_frame.setPts(in_frame.getPts());
        out_frame.setDts(in_frame.getDts());
        out_frame.pos = in_frame.pos;
        out_frame.stream_index = in_frame.stream_index;
        out_frame.duration = in_frame.duration;
        out_frame._size = out_size * _enc->getChannels() * osize;
        out_frame._type = CODEC_TYPE_AUDIO;
        out_frame.channels = _enc->getChannels();
        out_frame.sample_rate = _enc->getSampleRate();

        //        return frame;
      }

    }
  }
}

