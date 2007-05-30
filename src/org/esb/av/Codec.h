#ifndef ORG_ESB_AV_CODEC_H
#define ORG_ESB_AV_CODEC_H
#include "avformat.h"
#include "Frame.h"


namespace org{
    namespace esb{
        namespace av{
            class Codec{
                public:
                    Codec(const char * codecName);
                    Codec(AVCodecContext * codec);
                    CodecType getCodecType();
                    char * getCodecName();
                    int getCodecId();
		    void encodeFrame(Frame * frame);
		    void decodeFrame(Frame * frame);
                private:
                    AVCodecContext * _codecCtx;
                    AVCodec * _codec;
            };
        }
    }
}


#endif
