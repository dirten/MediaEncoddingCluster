/* 
 * File:   AudioFormat.cpp
 * Author: jholscher
 * 
 * Created on 29. August 2010, 19:29
 */

#include "AudioFormat.h"
namespace org {
  namespace esb {
    namespace av {

      AudioFormat::AudioFormat( AVCodecContext * ctx) {
        _sample_format = ctx->sample_fmt;
        _sample_rate = ctx->sample_rate;
        _sample_size_in_bits = av_get_bits_per_sample_format(ctx->sample_fmt) / 8;
        _channels = ctx->channels;
        _signedd = 0;
        _frame_size_in_bits = ctx->frame_size;
      }

      AudioFormat::AudioFormat(SampleFormat fmt, int samplerate, int sampleSizeInBits, int channels, int signedd, int frameSizeInBits) {
        _sample_format = fmt;
        _sample_rate = samplerate;
        _sample_size_in_bits = sampleSizeInBits;
        _channels = channels;
        _signedd = signedd;
        _frame_size_in_bits = frameSizeInBits;
      }

      SampleFormat AudioFormat::getSampleFormat() {
        return _sample_format;
      }

      int AudioFormat::getSampleRate() {
        return _sample_rate;
      }

      int AudioFormat::getSampleSizeInBits() {
        return _sample_size_in_bits;
      }

      int AudioFormat::getChannels() {
        return _channels;
      }

      int AudioFormat::getSigned() {
        return _signedd;
      }

      int AudioFormat::getFrameSizeInBits() {
        return _frame_size_in_bits;
      }

      AudioFormat::AudioFormat(const Format& orig):Format(orig) {
        /*
        _sample_format = orig._sample_format;
        _sample_rate = orig._sample_rate;
        _sample_size_in_bits = orig._sample_size_in_bits;
        _channels = orig._channels;
        _signedd = orig._signedd;
        _frame_size_in_bits = orig._frame_size_in_bits;
        _codec=orig._codec;
        _codec_ctx=orig._codec_ctx;*/
      }

      bool AudioFormat::operator==(const AudioFormat & orig) {
        return _sample_format == orig._sample_format&&
        _sample_rate == orig._sample_rate&&
        _sample_size_in_bits == orig._sample_size_in_bits&&
        _channels == orig._channels&&
        _signedd == orig._signedd&&
        _frame_size_in_bits == orig._frame_size_in_bits;

      }

    }
  }
}
