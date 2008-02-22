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
                    Codec();
                    ~Codec();
                    CodecType getCodecType();
                    char * getCodecName();
                    int getCodecId();
                    AVCodecContext * getCodecContext();
		    void open();
		    void initDefaults();
//		    Packet * encodeFrame(Frame & frame);
//		    Frame * decode(Packet & packet);
		    void setWidth(int w);
		    void setHeight(int h);
		    void setPixelFormat(PixelFormat pfm);
		    void setBitRate(int rate);
		    void setTimeBase(AVRational tb);
		    void setGopSize(int size);


		    template<class Archive>
		        void serialize(Archive & ar, const unsigned int version)
		            {
		                ar & _codec_id;
		                ar & _mode;
		                ar & _pix_fmt;
		                ar & _width;
		                ar & _height;
		                ar & _time_base.num;
		                ar & _time_base.den;
		                ar & _gop_size;
		                ar & _bit_rate;
/*
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
				*/
		            }
		    CodecID _codec_id;
                protected:
                    AVCodecContext * _codecCtx;
                    AVCodec * _codec;
                    void findCodec(int mode);
                    int _mode;
                private:
		    int _width;
		    int _height;
		    PixelFormat _pix_fmt;
		    int _bit_rate;
		    AVRational _time_base;
		    int _gop_size;
//            	    static void initialize();
            };
        }
    }
}


#endif
