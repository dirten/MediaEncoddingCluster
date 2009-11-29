/*----------------------------------------------------------------------
 *  File    : Packetizer.cpp
 *  Author  : Jan Hölscher <jan.hoelscher@esblab.com>
 *  Purpose :
 *  Created : 27. Oktober 2009, 23:34 by Jan Hölscher <jan.hoelscher@esblab.com>
 *  @version $Revision: 0 $
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
#include "Packetizer.h"

namespace org {
  namespace esb {
    namespace hive {
      namespace job {

        /**
         *
         */
        Packetizer::Packetizer(std::map<int, StreamData> stream_data) {
          _codec_overlap[CODEC_ID_MPEG2VIDEO] = 3;
          _codec_overlap[CODEC_ID_MP2] = 3;

          _codec_min_packets[CODEC_TYPE_VIDEO] = MIN_VIDEO_PACKETS;
          _codec_min_packets[CODEC_TYPE_AUDIO] = MIN_AUDIO_PACKETS;

          _streams = stream_data;

        }

        /**
         *
         */
        Packetizer::~Packetizer() {

        }

        /**
         *
         */
        bool Packetizer::putPacket(PacketPtr ptr) {
          bool result = false;
          //in case of stream is not mapped through stream_data element, discard packet
          if (_streams.find(ptr->getStreamIndex()) == _streams.end()) {
            return false;
          }

          if (_codec_overlap.find(_streams[ptr->getStreamIndex()].codec_id) == _codec_overlap.end()) {
            _codec_overlap[_streams[ptr->getStreamIndex()].codec_id] = 0;
          }
          result = processPacket(ptr);
          return result;
        }

        /**
         *
         */
        PacketListPtr Packetizer::getPacketList() {
          PacketListPtr t;
          if (_packet_list.size() > 0) {
            t = _packet_list.front();
          }
          return t;
        }

        PacketListPtr Packetizer::removePacketList() {
          PacketListPtr t;
          if (_packet_list.size() > 0) {
            t = _packet_list.front();
            _packet_list.pop_front();
          }
          return t;
        }

        /**
         *
         */
        int Packetizer::getPacketListCount() {
          return _packet_list.size();
        }

        /**
         *
         */
        void Packetizer::flushStreams() {
          std::map<int, StreamData>::iterator it = _streams.begin();
          for (; it != _streams.end(); it++) {
            PacketPtr pac = _overlap_queue[(*it).first].front();
            _streams[(*it).first].packets.push_back(pac);
            _overlap_queue[(*it).first].pop_front();
            buildList((*it).first);
          }
        }

        /**
         *
         */
        void Packetizer::addingPacketsFromQueue(int stream_id) {
          int qsize = _overlap_queue[stream_id].size();
          for (int a = 0; a < qsize - 1; a++) {
            _streams[stream_id].packets.push_back(_overlap_queue[stream_id][a]);
          }
        }

        /**
         *
         */
        bool Packetizer::buildList(int stream_id) {
          /**
           * need to append some packets to the current PacketList
           */
			if(_codec_overlap[_streams[stream_id].codec_id]>0)
				addingPacketsFromQueue(stream_id);
          _packet_list.push_back(_streams[stream_id].packets);
//          logdebug("Build Packet List for stream#:" << stream_id << " packet count" << _streams[stream_id].packets.size());
          _streams[stream_id].packets.clear();
			if(_codec_overlap[_streams[stream_id].codec_id]<0)
				addingPacketsFromQueue(stream_id);
          return true;
        }

        /**
         *
         */
        bool Packetizer::processPacket(PacketPtr ptr) {
          bool result = false;
          int stream_idx = ptr->getStreamIndex();
          _overlap_queue[stream_idx].push_back(ptr);

          if (_overlap_queue[stream_idx].size() > abs(_codec_overlap[_streams[stream_idx].codec_id]) + 1) {
            PacketPtr pac = _overlap_queue[stream_idx].front();
            _overlap_queue[stream_idx].pop_front();

            _streams[stream_idx].packets.push_back(pac);
            int size = static_cast<int> (_streams[stream_idx].packets.size());
            if (_overlap_queue[stream_idx].front()->isKeyFrame() &&
                size >= _codec_min_packets[_streams[stream_idx].codec_type]) {
              result = buildList(stream_idx);
            }
          }

          return result;
        }

      }
    }
  }
}

/*
 * 
 */

