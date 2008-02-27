#ifndef ORG_ESB_AV_PACKETOUTPUTSTREAM_H
#define ORG_ESB_AV_PACKETOUTPUTSTREAM_H
#include "org/esb/io/OutputStream.h"
#include "Packet.h"
#include "Encoder.h"

using namespace org::esb::io;
namespace org{
	namespace esb{
		namespace av{
			class PacketOutputStream: public OutputStream{
				public:
					PacketOutputStream(OutputStream * os);
					~PacketOutputStream();
					void writePacket(Packet * packet);
					void setEncoder(Encoder & encoder);
					void setEncoder(Encoder & encoder, int stream_id);
					void init();
					void writePacket(Packet & packet);
					void write(char * buffer, int length);
					void write(vector<unsigned char>&buffer);
					void flush();
				private:
					OutputStream * _target;
					AVFormatContext * _fmtCtx;
			};
		}
	}
}

#endif
