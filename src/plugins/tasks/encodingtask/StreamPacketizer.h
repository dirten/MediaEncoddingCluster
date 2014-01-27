/*----------------------------------------------------------------------
 *  File    : Packetizer.h
 *  Author  : Jan Hölscher <jan.hoelscher@esblab.com>
 *  Purpose :
 *  Created : 6. November 2009, 12:30 by Jan Hölscher <jan.hoelscher@esblab.com>
 *
 *
 * MediaEncodingCluster, Copyright (C) 2001-2009   Jan Hölscher
 *
 * This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307 USA
 *
 * ----------------------------------------------------------------------
 */
#ifndef _STREAMPACKETIZER_H
#define	_STREAMPACKETIZER_H
#include <boost/shared_ptr.hpp>
#include "org/esb/av/Packet.h"
#include "StreamData.h"
#include "exports.h"
//#include "org/esb/hive/job/ProcessUnit.h"


#include <deque>
#include <map>
#include <list>
namespace org {
  namespace esb {
    namespace av {
      class Decoder;
      class Encoder;
    }
  }
}
    namespace encodingtask {
        class ENCTASK_EXPORT StreamPacketizer {
        private:
        public:
            StreamPacketizer();
            StreamPacketizer(int size, Ptr<org::esb::av::Decoder>);
          ~StreamPacketizer();
          bool putPacket(org::esb::av::PacketPtr);
          void flushStreams();
          org::esb::av::PacketListPtr getPacketList();
          org::esb::av::PacketListPtr removePacketList();
          int getPacketListCount();
          Ptr<org::esb::av::Decoder> getDecoder();
        private:
          bool processPacket(org::esb::av::PacketPtr);
          StreamData _stream;
          Ptr<org::esb::av::Decoder> _decoder;

          std::list<org::esb::av::PacketListPtr> _packet_list;

          static const int MIN_AUDIO_PACKETS = 5000;
          static const int MIN_VIDEO_PACKETS = 500;

          org::esb::av::PacketListPtr _overlap_queue;

          std::map<CodecID, int> _codec_overlap;
          std::map<AVMediaType, int> _codec_min_packets;

          int delay;

        };
    }
#endif	/* _STREAMPACKETIZER_H */

