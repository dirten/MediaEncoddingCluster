#ifndef ORG_ESB_AV_DECODER
#define ORG_ESB_AV_DECODER
#include "Codec.h"
#include "AV.h"

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
	Frame decodeLast();
	void analyzePacket(Packet & packet);
//	void decode(Packet & packet, Frame & frame);
	
	template<class Archive>
	    void serialize(Archive & ar, const unsigned int version){
          ar & boost::serialization::base_object<Codec>(*this);
	    }
	    
	    
//		prívate:
			Frame decodeVideo(Packet & packet);
			Frame decodeAudio(Packet & packet);


};
}}}

#endif
