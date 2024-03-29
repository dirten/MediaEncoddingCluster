#ifndef ORG_ESB_AV_AV_H
#define ORG_ESB_AV_AV_H
#pragma warning( disable : 4005 )
#pragma warning( disable : 4244 )

#ifdef	__cplusplus
extern "C" {
#endif
#undef FF_API_FLAC_GLOBAL_OPTS
#define FF_API_FLAC_GLOBAL_OPTS 0
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
#include <libavutil/fifo.h>
#include <libavutil/rational.h>
#include <libswresample/swresample.h>
#include <libavfilter/avfilter.h>
#include <libavfilter/avfilter.h>
#include <libavfilter/avfiltergraph.h>
#include <libavfilter/avcodec.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <inttypes.h>
typedef AVCodecID CodecID;
#ifdef	__cplusplus
}
#endif

#undef USE_TIME_BASE_Q
#endif

