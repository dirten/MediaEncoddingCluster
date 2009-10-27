/* 
 * File:   Packetizer.h
 * Author: jholscher
 *
 * Created on 27. Oktober 2009, 23:35
 */

#ifndef _PACKETIZER_H
#define	_PACKETIZER_H
#include <boost/shared_ptr.hpp>
#include "org/esb/av/Packet.h"
#include "org/esb/hive/job/ProcessUnit.h"
namespace org{
  namespace esb{
    namespace hive{
      namespace job{
        typedef boost::shared_ptr<org::esb::av::Packet> PacketPtr;
        typedef boost::shared_ptr<org::esb::hive::job::ProcessUnit> ProcessUnitPtr;
        class Packetizer{
        public:
          Packetizer();
          ~Packetizer();
          bool putPacket(PacketPtr);
          ProcessUnitPtr getProcessUnit();
        private:
          bool processAudioPacket(PacketPtr);
          bool processVideoPacket(PacketPtr);

        };
      }
    }
  }
}

#endif	/* _PACKETIZER_H */

