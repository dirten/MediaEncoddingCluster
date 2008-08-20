#ifndef ORG_ESB_AV_AUDIOFRAME_H
#define ORG_ESB_AV_AUDIOFRAME_H

#include "Frame.h"
namespace org{
namespace esb{
namespace av{
//class Frame;
class AudioFrame:public Frame {
  public:
    AudioFrame();
    AudioFrame(const AudioFrame & frame);
    AudioFrame(int format, int samplerate);
};

}}}
#endif
