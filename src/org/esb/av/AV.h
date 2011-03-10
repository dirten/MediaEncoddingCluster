#ifndef ORG_ESB_AV_AV_H
#define ORG_ESB_AV_AV_H
#pragma warning( disable : 4005 )
#pragma warning( disable : 4244 )
//#include "org/esb/util/Log.h"
#ifdef	__cplusplus
extern "C" {
#endif

//extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavcodec/opt.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
#include <libavutil/fifo.h>
#include <libavutil/rational.h>
#ifdef	__cplusplus
}
#endif

#include "boost/thread/mutex.hpp"

#undef USE_TIME_BASE_Q
#ifndef ORG_ESB_AV_FFMPEGMUTEX_H
#define ORG_ESB_AV_FFMPEGMUTEX_H
namespace org {
  namespace esb {
    namespace av {
      static boost::mutex ffmpeg_mutex;
    }
  }
}

#endif
#endif

