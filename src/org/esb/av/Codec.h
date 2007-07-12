#ifndef ORG_ESB_AV_CODEC_H
#define ORG_ESB_AV_CODEC_H
extern "C" {
#include "avformat.h"
}
//#include "Frame.h"
#include "Packet.h"


namespace org{
    namespace esb{
        namespace av{
	class Frame;
            class Codec: public AVCodecContext{
                public:
                    Codec(const CodecID codecId);
                    Codec(AVCodecContext * codec);
                    CodecType getCodecType();
                    char * getCodecName();
                    int getCodecId();
                    AVCodecContext * getCodecContext();
		    void open();
		    Packet * encodeFrame(Frame & frame);
		    Frame * decode(Packet & packet);
                private:
                    AVCodecContext * _codecCtx;
                    AVCodec * _codec;
            };
        }
    }
}


#endif
