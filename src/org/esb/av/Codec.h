#ifndef ORG_ESB_AV_CODEC_H
#define ORG_ESB_AV_CODEC_H
#include "avformat.h"



namespace org{
    namespace esb{
        namespace av{
            class Codec{
                public:
                    Codec(const char * codecName);
                    Codec(AVCodecContext * codec);
                    CodecType getCodecType();
                    char * getCodecName();
                private:
                    AVCodecContext * _codecCtx;
                    AVCodec * _codec;
            };
        }
    }
}


#endif
