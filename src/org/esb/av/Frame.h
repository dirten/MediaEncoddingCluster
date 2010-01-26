
#ifndef ORG_ESB_AV_FRAME_H
#define ORG_ESB_AV_FRAME_H
#include "AV.h"
#include "Packet.h"
#include "Codec.h"

#include <boost/shared_ptr.hpp>

namespace org {
  namespace esb {
    namespace av {

      class Frame {
      public:
        Frame();
        Frame(PixelFormat format, int width, int height, bool allocate = true);
        Frame(uint8_t * buffer, int64_t size);

        //                Frame(const Frame & source);
        //                Frame operator=(Frame & frame);
        ~Frame();

        uint8_t * getData();
        int getWidth();
        int getHeight();
        int getSize();
        PixelFormat getFormat();
        AVPacket * getPacket();
        Frame getFrame(int format = 0);
        void setPts(int64_t pts);
        void setDts(int64_t pts);
        int64_t getPts();
        int64_t getDts();
        int getDuration();
        void setDuration(int);
        AVFrame * getAVFrame();
        bool isFinished();
        void setFinished(bool);
        std::string toString();
        void setTimeBase(AVRational);
        AVRational getTimeBase();
        void dumpHex();
        int getFrameCount();
        void setFrameCount(int d);

        //	void setFrame(AVFrame * frame);
        int _height;
        int _width;
        PixelFormat _pixFormat;
        //    private:
        AVCodecContext * _codecContext;
        AVPacket * _packet;
        //	AVFrame * _frame;
        uint8_t * _buffer;
      private:
        bool _isFinished;
        int64_t _dts;
        int64_t _pts;
        boost::shared_ptr<AVFrame> framePtr;
        AVRational _time_base;
        int _frames;
      public:
        bool _allocated;
        int _frameFinished;
        int64_t pos;
        int duration;
        int _size;
        int _type;
        int channels;
        int sample_rate;
        int stream_index;

      };
    }
  }
}
#endif

