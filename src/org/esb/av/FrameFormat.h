#ifndef ORG_ESB_AV_FRAMEFORMAT_H
#define ORG_ESB_AV_FRAMEFORMAT_H

#include "avformat.h"


namespace org{
namespace esb{
namespace av{
struct FrameFormat{
	PixelFormat pixel_format;
	int width;
	int height;	
};
}}}
#endif
