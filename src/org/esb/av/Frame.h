#ifndef ORG_ESB_AV_FRAME_H
#define ORG_ESB_AV_FRAME_H
#include "avformat.h"



namespace org{
namespace esb{
namespace av{
class Frame {
    public:
	Frame(AVPacket * packet, AVCodecContext * codecContext);
	~Frame();
	uint8_t * getData();
    int getWidth();
    int getHeight();
	int getSize();
	AVPacket * getPacket();
	AVFrame * getFrame();
	void setFrame(AVFrame * frame);
    private:
    AVCodecContext * _codecContext;
	AVPacket * _packet;
	AVFrame * _frame;
};
}}}
#endif

