#ifndef ORG_ESB_AV_AVINPUTSTREAM_H
#define ORG_ESB_AV_AVINPUTSTREAM_H
#include "Codec.h"
#include "Frame.h"
#include "avformat.h"




namespace org{
    namespace esb{
        namespace av{
            class AVInputStream{
                public:
                    AVInputStream(AVFormatContext * context, int streamIndex);
                    Codec * getCodec();
                    long getDuration();
                    float getTimeBase();
                    long getNumberFrames();
                    Frame * getNextFrame();
                    Frame * getFrame(int frameIndex);
                private:
                    AVStream * _avStream;
                    AVFormatContext * _formatContext;
		    AVCodecContext * _codecContext;
		    AVCodec * _codec;
                    int _streamIndex;
            };
        }
    }
}

#endif

