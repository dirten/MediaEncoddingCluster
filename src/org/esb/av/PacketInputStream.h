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
        ~PacketInputStream();
        Packet * readPacket();
        int read(unsigned char * buffer, int length);
        int available(bool blocking=false);        
        uint64_t getDuration();
        Codec * getCodec();
    private:
        AVFormatContext * _formatCtx;
        AVCodecContext * _codecCtx;
        AVCodec * _codec;
        
        int _streamIndex;
};
}}}

