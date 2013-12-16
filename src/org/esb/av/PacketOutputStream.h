#ifndef ORG_ESB_AV_PACKETOUTPUTSTREAM_H
#define ORG_ESB_AV_PACKETOUTPUTSTREAM_H
#include "org/esb/io/OutputStream.h"
#include "org/esb/io/FileOutputStream.h"

#include "Packet.h"
#include "Codec.h"
#include "org/esb/util/Log.h"
#include <list>
#include <map>
#include "exports.h"
#include "AVPipe.h"
using namespace org::esb::io;
namespace org {
  namespace esb {
    namespace av {

      class AV_EXPORT PacketOutputStream : public OutputStream, public AVPipe {
        classlogger("org.esb.av.PacketOutputStream")
      public:
        PacketOutputStream(OutputStream * os, std::string statsfile="");
        ~PacketOutputStream();
        //int writePacket(Packet * packet);
        void setEncoder(Codec & code);
        void setEncoder(Codec & code, int stream_id);
        bool newPacket(Ptr<Packet>p);
         bool newFrame(Ptr<Frame>){}
    std::list<AVStream*> getStreamList();
        bool init();
        int writePacket(Packet & packet);
        void write(char * buffer, int length);
        void write(vector<unsigned char>&buffer);
        void flush();
        bool close();
      private:
        bool _isInitialized;
        OutputStream * _target;
        AVFormatContext * _fmtCtx;
        list<AVStream*> streams;
        map<int, long long int> streamPts;
        map<int, long long int> streamDts;
        org::esb::io::FileOutputStream * _stats_fos;
      };
    }
  }
}

#endif
