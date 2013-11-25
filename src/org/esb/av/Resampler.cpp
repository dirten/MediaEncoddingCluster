#include "Resampler.h"
#include "AV.h"

#include "org/esb/lang/Exception.h"

using org::esb::lang::Exception;

namespace org {
  namespace esb {
    namespace av {

      Resampler::Resampler()
      {
        _swr_ctx = swr_alloc();

      }
      void Resampler::setSourceChannelLayout(int64_t d){
        av_opt_set_int(_swr_ctx, "in_channel_layout", d, 0);
      }

      void Resampler::setSourceSampleRate(int64_t d){
        av_opt_set_int(_swr_ctx, "in_sample_rate", d, 0);
      }

      void Resampler::setSourceSampleFormat(AVSampleFormat d){
        av_opt_set_sample_fmt(_swr_ctx, "in_sample_fmt", d, 0);
      }

      void Resampler::setTargetChannelLayout(int64_t d){
        av_opt_set_int(_swr_ctx, "out_channel_layout", d, 0);
      }

      void Resampler::setTargetSampleRate(int64_t d){
        av_opt_set_int(_swr_ctx, "out_sample_rate", d, 0);
      }

      void Resampler::setTargetSampleFormat(AVSampleFormat d){
        av_opt_set_sample_fmt(_swr_ctx, "out_sample_fmt", d, 0);
      }

      void Resampler::resample(uint8_t * src, uint8_t * dst){
        int64_t sample_rate;
        if(av_opt_get_int(_swr_ctx, "in_sample_rate", 0, &sample_rate)<0){
          throw Exception("could not get input sample format");
        }
        int64_t delay=swr_get_delay(_swr_ctx,sample_rate);
      }
    }
  }
}
