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
        src_channel_layout=d;
      }

      void Resampler::setSourceSampleRate(int64_t d){
        av_opt_set_int(_swr_ctx, "in_sample_rate", d, 0);
        src_sample_rate=d;
      }

      void Resampler::setSourceSampleFormat(AVSampleFormat d){
        av_opt_set_sample_fmt(_swr_ctx, "in_sample_fmt", d, 0);
        src_sample_format=d;
      }

      void Resampler::setTargetChannelLayout(int64_t d){
        av_opt_set_int(_swr_ctx, "out_channel_layout", d, 0);
        trg_channel_layout=d;
      }

      void Resampler::setTargetSampleRate(int64_t d){
        av_opt_set_int(_swr_ctx, "out_sample_rate", d, 0);
        trg_sample_rate=d;
      }

      void Resampler::setTargetSampleFormat(AVSampleFormat d){
        av_opt_set_sample_fmt(_swr_ctx, "out_sample_fmt", d, 0);
        trg_sample_format=d;
      }

      int Resampler::resample(Frame & src_data,Frame & trg_data){
        AVFrame * src_frame=src_data.getAVFrame();
        AVFrame * trg_frame=trg_data.getAVFrame();
        return resample(src_frame->data,src_frame->nb_samples,trg_frame->data,trg_frame->nb_samples);
        //return 0;
      }
      int Resampler::init(){
        int ret=0;
        if ((ret = swr_init(_swr_ctx)) < 0) {
          throw org::esb::lang::Exception(__FILE__,__LINE__,"Failed to initialize the resampling context");

        }
      }

      int Resampler::resample(uint8_t ** src_data, int src_size , uint8_t ** trg_data, int trg_size){
        int src_linesize, dst_linesize;

        int src_nb_channels = av_get_channel_layout_nb_channels(src_channel_layout);

        int trg_nb_channels = av_get_channel_layout_nb_channels(trg_channel_layout);
        int trg_nb_samples = av_rescale_rnd(src_size, trg_sample_rate, src_sample_rate, AV_ROUND_UP);
        int ret = av_samples_alloc_array_and_samples(&trg_data, &dst_linesize, trg_nb_channels, trg_nb_samples, trg_sample_format, 0);

        if (ret < 0) {
          throw org::esb::lang::Exception(__FILE__,__LINE__,"could not allocate samples array");
        }
        ret = swr_convert(_swr_ctx, trg_data, trg_nb_samples, (const uint8_t **)src_data, src_size);
        if (ret < 0) {
          throw org::esb::lang::Exception(__FILE__,__LINE__,"Error while converting samples");
        }
        /*
        if (ret < 0) {
          fprintf(stderr, "Could not allocate destination samples\n");
        }
        int64_t delay=swr_get_delay(_swr_ctx,sample_rate);
        */
        return 0;
      }
    }
  }
}
