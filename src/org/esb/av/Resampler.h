#ifndef RESAMPLER_H
#define RESAMPLER_H
#include <inttypes.h>
#include "AV.h"
namespace org {
  namespace esb {
    namespace av {

      class Resampler
      {
        public:
          Resampler();
          void setSourceChannelLayout(int64_t);
          void setSourceSampleRate(int64_t);
          void setSourceSampleFormat(AVSampleFormat);

          void setTargetChannelLayout(int64_t);
          void setTargetSampleRate(int64_t);
          void setTargetSampleFormat(AVSampleFormat);

          void resample(uint8_t * src, uint8_t * dst);
        private:
          SwrContext * _swr_ctx;
      };
    }
  }
}
#endif // RESAMPLER_H
