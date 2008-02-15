#ifndef ORG_ESBG_AV_ENCODER
#define ORG_ESBG_AV_ENCODER

#include "Codec.h"
#include <avcodec.h>


namespace org{
namespace esb{
namespace av{
//class Packet;
    class Encoder:public Codec{
	public:
	    Encoder(CodecID id);
	    Packet encode(Frame & f);
    
    };
}}}

#endif
