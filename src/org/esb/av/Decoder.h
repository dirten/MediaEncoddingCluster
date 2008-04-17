#ifndef ORG_ESB_AV_DECODER
#define ORG_ESB_AV_DECODER
#include "Codec.h"
#include <avcodec.h>

namespace org{
namespace esb{
namespace av{
class Frame;
class Decoder:public Codec{
    public:
	Decoder(CodecID id);
	Decoder();
//	Frame * decode(Packet & packet);
	Frame decode(Packet & packet);
//	void decode(Packet & packet, Frame & frame);
	/*
	template<class Archive>
	    void serialize(Archive & ar, const unsigned int version){
        	ar & codec_id;
        	ar & _mode;
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
	    */
	    
//		prívate:
			Frame decodeVideo(Packet & packet);
			Frame decodeAudio(Packet & packet);


};
}}}

#endif
