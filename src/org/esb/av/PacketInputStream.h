#ifndef ORG_ESB_AV_PACKETINPUTSTREAM_H
#define ORG_ESB_AV_PACKETINPUTSTREAM_H
#include <vector>
#include "org/esb/io/InputStream.h"
#include "Packet.h"
#include "Codec.h"
using namespace org::esb::io;
namespace org{
namespace esb{
namespace av{
class PacketInputStream: public InputStream{
    public:
        PacketInputStream(InputStream * is);
        PacketInputStream(InputStream & is);
        ~PacketInputStream();
        int readPacket(Packet&packet);
        Packet  readPacket();
        int read(unsigned char * buffer, int length);
        int read(vector<unsigned char>&buffer);
        int read();
        long long int available(bool blocking=false);
        uint64_t getDuration();
        Codec * getCodec();
        void skip(long packets);
    private:
        AVFormatContext * _formatCtx;
        Codec * _codec2;
	AVCodecContext * _codecCtx;
        AVCodec * _codec;
	
        int readPacketFromFormatIS(Packet&packet);
        int readPacketFromIS(Packet&packet);
        Packet readPacketFromFormatIS();
//        __attribute__((deprecated))Packet & readPacketFromIS();
        Packet & readPacketFromIS();
//	Packet _packet;
        int _streamIndex;
        int _readFrom;
        InputStream * _source;
};
}}}
#endif
