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
	Frame(PixelFormat format, int width, int height);
//	Frame(int format, int width, int height, unsigned char * data);
//	Frame(Frame * source, int format);
	Frame(const Frame & source);
//	Frame(Packet * packet, Codec * codec);
    Frame operator=(Frame & frame);

	~Frame();
	uint8_t * getData();
    int getWidth();
    int getHeight();
	int getSize();
	int getFormat();
	AVPacket * getPacket();
	Frame getFrame(int format=0);
	void setPts(int64_t pts);
	void setDts(int64_t pts);
	int64_t getPts();
	int64_t getDts();
//	void setFrame(AVFrame * frame);
        int _height;
        int _width;
        int _pixFormat;
//    private:
        AVCodecContext * _codecContext;
	AVPacket * _packet;
//	AVFrame * _frame;
        uint8_t * _buffer;
		private:
        int64_t _dts;
        int64_t _pts;

    public:
		int _frameFinished;
        int64_t pos;
        int duration;
        int _size;
        int _type;
        int channels;
        int sample_rate;
        int stream_index;

};
}}}
#endif

