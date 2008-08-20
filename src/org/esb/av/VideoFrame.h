#ifndef ORG_ESB_AV_VIDEOFRAME_H
#define ORG_ESB_AV_VIDEOFRAME_H

#include "Frame.h"
namespace org{
namespace esb{
namespace av{
//class Frame;
class VideoFrame:public Frame {
  public:
    VideoFrame();
    VideoFrame(const VideoFrame & frame);
    VideoFrame(PixelFormat format, int width, int height);

};

}}}
#endif

