#ifndef ORG_ESB_AV_CODEC_H
#define ORG_ESB_AV_CODEC_H
extern "C" {
#include "avformat.h"
}
//#include "Frame.h"


namespace org{
    namespace esb{
        namespace av{
            class Codec{
                public:
                    Codec(const CodecID codecId);
                    Codec(AVCodecContext * codec);
                    CodecType getCodecType();
                    char * getCodecName();
                    int getCodecId();
                    AVCodecContext * getCodecContext();
					void open();
//		    void encodeFrame(Frame * frame);
//		    void decodeFrame(Frame * frame);
                private:
                    AVCodecContext * _codecCtx;
                    AVCodec * _codec;
            };
        }
    }
}


#endif
