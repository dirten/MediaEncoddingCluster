#ifndef RESAMPLER_H
#define RESAMPLER_H
#include <inttypes.h>
#include "AV.h"
#include "Frame.h"
namespace org {
  namespace esb {
    namespace av {

      class Resampler
      {
        public:
          Resampler();
          ~Resampler();
          void setSourceChannelLayout(int64_t);
          void setSourceSampleRate(int64_t);
          void setSourceSampleFormat(AVSampleFormat);

          void setTargetChannelLayout(int64_t);
          void setTargetSampleRate(int64_t);
          void setTargetSampleFormat(AVSampleFormat);
          void setTargetSampleSize(int64_t);

          int resample(uint8_t ** src_data, int src_size , uint8_t ** trg_data, int trg_size);
          int resample(Frame & src_data,Frame & trg_data);
          int init();
        private:
          SwrContext * _swr_ctx;
          int64_t src_channel_layout;
          int64_t trg_channel_layout;

          int64_t src_sample_rate;
          int64_t trg_sample_rate;

          int64_t trg_sample_size;

          AVSampleFormat src_sample_format;
          AVSampleFormat trg_sample_format;
      };
    }
  }
}
#endif // RESAMPLER_H
