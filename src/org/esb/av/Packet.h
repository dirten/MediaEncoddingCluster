#ifndef ORG_ESB_PACKET_H
#define ORG_ESB_PACKET_H
#include "avformat.h"


namespace org{
    namespace esb{
	namespace av{
	    class Packet:public AVPacket
	    {
		public:
		    Packet();
		    ~Packet();
            uint8_t * getData();
            int getSize();
            int getPts();
            int getDts();
            int getFlags();
            int getStreamIndex();
            int getDuration();
            void * getPriv();
            int64_t getPosition();
	    };
	}
    }
}
#endif
