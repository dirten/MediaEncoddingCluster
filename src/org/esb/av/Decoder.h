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
	Frame * decode(Packet & packet);

};
}}}

#endif
