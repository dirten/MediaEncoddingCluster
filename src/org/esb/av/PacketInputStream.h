#ifndef ORG_ESB_AV_PACKETINPUTSTREAM_H
#define ORG_ESB_AV_PACKETINPUTSTREAM_H
#include <vector>
#include "org/esb/io/InputStream.h"
#include "FormatInputStream.h"
#include "Packet.h"
#include "Codec.h"
using namespace org::esb::io;
namespace org{
namespace esb{
namespace av{
class PacketInputStream: public InputStream{
    public:
        PacketInputStream(InputStream * is, bool truncate=false, bool calc_offset=false);
        
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
        int64_t _start_offset;
/*        int _video_idx;
        int _audio_idx;
        int _video_idx_map;
        int _audio_idx_map;*/
        InputStream * _source;
        FormatInputStream * _fis;
        /** private CopyConstructor */
        PacketInputStream(InputStream & is);
        struct StreamData{
            /**
             * the offset from Stream first dts
             */
            int64_t start_dts_offset;
            /**
             * the offset from Stream first pts
             */
            int64_t start_pts_offset;
            /**
             * when packet dts is lower then this and the flags discard is set, then the packet will be discard
             */
            int64_t min_dts;
            /**
             * discard flag for the stream
             */
            bool discard;

        };
        std::map<int, StreamData> _streams;
};
}}}
#endif
