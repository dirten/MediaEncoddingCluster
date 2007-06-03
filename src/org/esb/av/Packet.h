#ifndef ORG_ESB_PACKET_H
#define ORG_ESB_PACKET_H
#include "avformat.h"


namespace org{
    namespace esb{
	namespace av{
	    class Packet: public AVPacket{
		public:
		    Packet(AVPacket * packet);
		    ~Packet();

	    };
	}
    }
}
#endif
