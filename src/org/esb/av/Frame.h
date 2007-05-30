#ifndef ORG_ESB_AV_FRAME_H
#define ORG_ESB_AV_FRAME_H
#include "avformat.h"



namespace org{
namespace esb{
namespace av{
class Frame {
    public:
	Frame(AVPacket * packet);
	char * getData();
	int getSize();
};
}}}
#endif

