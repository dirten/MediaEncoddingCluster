#include "MaxPacketCountFilter.h"
#include "../Packet.h"
namespace org {
  namespace esb {
    namespace av {

      MaxPacketCountFilter::MaxPacketCountFilter(int max)
      {
        _maxPackets=max;
      }


      bool MaxPacketCountFilter::newPacket(Ptr<Packet> p){
        if(_maxPackets>0){
          _maxPackets--;
          return pushPacket(p);
        }else{
          return false;//pushPacket(new Packet());
        }
      }
    }
  }
}
