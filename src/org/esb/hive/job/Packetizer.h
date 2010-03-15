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
#ifndef _PACKETIZER_H
#define	_PACKETIZER_H
#include <boost/shared_ptr.hpp>
#include "org/esb/av/Packet.h"
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
    namespace hive {
      namespace job {
        typedef boost::shared_ptr<org::esb::av::Packet> PacketPtr;
        typedef std::deque<PacketPtr> PacketListPtr;

        class Packetizer {
        private:

          enum State {
            STATE_NOP,
            STATE_START_I_FRAME,
            STATE_END_I_FRAME,
            STATE_START_PACKETGROUP,
            STATE_END_PACKETGROUP
          };

        public:

          struct StreamData {
            PacketListPtr packets;
//            CodecType codec_type;
//            CodecID codec_id;
            boost::shared_ptr<org::esb::av::Decoder> decoder;
            boost::shared_ptr<org::esb::av::Encoder> encoder;
            State state;
          };
          Packetizer(std::map<int, StreamData>);
          ~Packetizer();
          bool putPacket(PacketPtr);
          void flushStreams();
          PacketListPtr getPacketList();
          PacketListPtr removePacketList();
          int getPacketListCount();
        private:
//          State _state;
          bool processPacket(PacketPtr);
          bool processPacket2(PacketPtr);
          bool buildList(int stream_id);
          void addingPacketsFromQueue(int stream_id);

          std::map<int, StreamData> _streams;

          std::list<PacketListPtr> _packet_list;

          static const int MIN_AUDIO_PACKETS = 500;
          static const int MIN_VIDEO_PACKETS = 20;

          std::map<int, PacketListPtr> _overlap_queue;
          std::map<CodecID, int> _codec_overlap;
          std::map<CodecType, int> _codec_min_packets;

        };
      }
    }
  }
}

#endif	/* _PACKETIZER_H */

