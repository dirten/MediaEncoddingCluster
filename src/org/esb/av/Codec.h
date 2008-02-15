#ifndef ORG_ESB_AV_CODEC_H
#define ORG_ESB_AV_CODEC_H
extern "C" {
#include "avformat.h"
}
#include <boost/serialization/binary_object.hpp>
#include "Packet.h"


namespace org{
    namespace esb{
        namespace av{
	class Frame;
            class Codec: public AVCodecContext{
                public:
		    const static int DECODER=1;
		    const static int ENCODER=2;
                    Codec(const CodecID codecId, int mode=DECODER);
                    Codec(AVCodecContext * codec);
                    ~Codec();
                    CodecType getCodecType();
                    char * getCodecName();
                    int getCodecId();
                    AVCodecContext * getCodecContext();
		    void open(int mode=DECODER);
		    void initDefaults();
		    Packet * encodeFrame(Frame & frame);
//		    Frame * decode(Packet & packet);

		    template<class Archive>
		        void serialize(Archive & ar, const unsigned int version)
		            {
		                ar & codec_id;
		                ar & codec_type;
		                ar & bit_rate;
		                ar & pix_fmt;
		                ar & width;
		                ar & height;
		                ar & has_b_frames;
		                ar & extradata_size;
				if(extradata==NULL){
				    extradata=new uint8_t[extradata_size];
				}
				ar & boost::serialization::make_binary_object(extradata,extradata_size);
		            }
                protected:
                    AVCodecContext * _codecCtx;
                    AVCodec * _codec;
		    CodecID _codec_id;
                    void findCodec(int mode);
            };
        }
    }
}


#endif
