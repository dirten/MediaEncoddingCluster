#ifndef MAXPACKETCOUNTFILTER_H
#define MAXPACKETCOUNTFILTER_H
#include "../AVPipe.h"
#include "../exports.h"

namespace org {
  namespace esb {
    namespace av {

      class AV_EXPORT MaxPacketCountFilter: public AVPipe
      {
        public:

          MaxPacketCountFilter(int max);
          void init(){/*NOP*/}
        private:
          bool newPacket(Ptr<Packet> p);
          bool newFrame(Ptr<Frame> p){return false;}
          int _maxPackets;
      };
    }
  }
}

#endif // MAXPACKETCOUNTFILTER_H
