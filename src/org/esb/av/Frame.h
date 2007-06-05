#ifndef ORG_ESB_AV_FRAME_H
#define ORG_ESB_AV_FRAME_H
#include "avformat.h"
#include "Packet.h"
#include "Codec.h"


namespace org{
namespace esb{
namespace av{
class Frame :public AVFrame{
    public:
	Frame(int format, int width, int height);
	Frame(Frame * source, int format);
	Frame(Packet * packet, Codec * codec);
	~Frame();
	uint8_t * getData();
    int getWidth();
    int getHeight();
	int getSize();
	int getFormat();
	AVPacket * getPacket();
	Frame * getFrame(int format=0);
//	void setFrame(AVFrame * frame);
    private:
        AVCodecContext * _codecContext;
	    AVPacket * _packet;
	    AVFrame * _frame;
        int _height;
        int _width;
        int _pixFormat;
        uint8_t * _buffer;
};
}}}
#endif

