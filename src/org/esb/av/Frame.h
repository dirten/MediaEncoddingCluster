#ifndef ORG_ESB_AV_FRAME_H
#define ORG_ESB_AV_FRAME_H
#include "AV.h"
#include "Packet.h"
#include "Codec.h"


namespace org{
namespace esb{
namespace av{
class Frame :public AVFrame{
    public:
	Frame();
	Frame(int format, int width, int height);
	Frame(int format, int width, int height, unsigned char * data);
	Frame(Frame * source, int format);
	Frame(Packet * packet, Codec * codec);
	~Frame();
	uint8_t * getData();
    int getWidth();
    int getHeight();
	int getSize();
	int getFormat();
	AVPacket * getPacket();
	Frame getFrame(int format=0);
//	void setFrame(AVFrame * frame);
        int _height;
        int _width;
        int _pixFormat;
//    private:
        AVCodecContext * _codecContext;
	AVPacket * _packet;
//	AVFrame * _frame;
        uint8_t * _buffer;
    public:
	int _frameFinished;
        int64_t dts;
        int64_t pos;
        int duration;
        int _size;

};
}}}
#endif

