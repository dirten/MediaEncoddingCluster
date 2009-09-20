
#include "StreamInfo.h"

/**
      terms.push_back(erl_mk_int(str->index));
      terms.push_back(erl_mk_int(str->codec->codec_type));
      terms.push_back(erl_mk_int(str->codec->codec_id));
      terms.push_back(erl_mk_int(str->codec->bit_rate));
      terms.push_back(erl_mk_int(str->codec->codec_type == CODEC_TYPE_VIDEO ? av_q2d(str->r_frame_rate) : str->codec->sample_rate));
      terms.push_back(erl_mk_int(str->time_base.num));
      terms.push_back(erl_mk_int(str->time_base.den));
      terms.push_back(erl_mk_int(str->codec->width));
      terms.push_back(erl_mk_int(str->codec->height));
      terms.push_back(erl_mk_int(str->codec->channels));
      terms.push_back(erl_mk_int(str->codec->gop_size));
      terms.push_back(erl_mk_int(str->codec->codec_type == CODEC_TYPE_VIDEO ? (int) str->codec->pix_fmt : (int) str->codec->sample_fmt));
      terms.push_back(erl_mk_string(Decimal(str->start_time).toString().c_str()));
      terms.push_back(erl_mk_string(Decimal(str->duration).toString().c_str()));

 */
namespace org {
  namespace esb {
    namespace av {

      StreamInfo::StreamInfo(AVStream * str, int idx){
        _s=str;
        _idx=idx;
      }
      int StreamInfo::getIndex(){
        return _idx;
      }
      int StreamInfo::getCodecType(){
        return _s->codec->codec_type;
      }
      int StreamInfo::getCodecId(){
        return _s->codec->codec_id;
      }
      int StreamInfo::getCodecBitrate(){
        return _s->codec->bit_rate;
      }
      AVRational StreamInfo::getFrameRate(){
        return _s->r_frame_rate;
      }
/*
      AVRational StreamInfo::getCodecFrameRate(){
        return _s->codec->frame_rate;
      }
*/
      int StreamInfo::getCodecSampleRate(){
        return _s->codec->sample_rate;
      }
      AVRational StreamInfo::getTimeBase(){
        return _s->time_base;
      }
      AVRational StreamInfo::getCodecTimeBase(){
        return _s->codec->time_base;
      }
      int StreamInfo::getWidth(){
        return _s->codec->width;
      }
      int StreamInfo::getheight(){
        return _s->codec->height;
      }
      int StreamInfo::getChannels(){
        return _s->codec->channels;
      }
      int StreamInfo::getGOPSize(){
        return _s->codec->gop_size;
      }
      int StreamInfo::getPixelFormat(){
        return _s->codec->pix_fmt;
      }
      int StreamInfo::getSampleFormat(){
        return _s->codec->sample_fmt;
      }
      int StreamInfo::getDuration(){
        return _s->duration;
      }
      float StreamInfo::getQuality(){
        return _s->quality;
      }
      long long int StreamInfo::getFirstPts(){
        return _s->start_time;
      }
      long long int StreamInfo::getFirstDts(){
        return _s->first_dts;
      }
      int StreamInfo::getNBIndexEntries(){
        return _s->nb_index_entries;
      }
    }
  }
}
