/* 
 * File:   Packetizer.cpp
 * Author: jholscher
 *
 * Created on 27. Oktober 2009, 23:34
 */

#include "Packetizer.h"

namespace org {
  namespace esb {
    namespace hive {
      namespace job {

        Packetizer::Packetizer(std::map<int, StreamData> stream_data) {
          std::map<int, StreamData>::iterator it = stream_data.begin();
          for (; it != stream_data.end(); it++) {
            (*it).second.jut_size_reached = false;
          }
          _streams = stream_data;
        }

        Packetizer::~Packetizer() {

        }

        bool Packetizer::putPacket(PacketPtr ptr) {
          bool result = false;
          //in case of stream is not mapped through stream_data element, discard packet
          if (_streams.find(ptr->getStreamIndex()) == _streams.end()) {
            return false;
          }

          int stream_id = ptr->getStreamIndex();
          if (_streams[stream_id].codec_type == CODEC_TYPE_VIDEO)
            result = processVideoPacket(ptr);
          else if (_streams[stream_id].codec_type == CODEC_TYPE_AUDIO)
            result = processAudioPacket(ptr);
          return result;
        }

        PacketListPtr Packetizer::getPacketList() {
          PacketListPtr t;
          if (_packet_list.size() > 0) {
            t = _packet_list.front();
            //            _packet_list.pop_front();
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
        int Packetizer::getPacketListCount() {
          return _packet_list.size();
        }

        void Packetizer::flushStreams() {
          std::map<int, StreamData>::iterator it = _streams.begin();
          for (; it != _streams.end(); it++) {
            buildList((*it).first);
          }
        }

        bool Packetizer::buildList(int stream_id) {
          logdebug("Build Packet List for stream#:" << stream_id << " packet count" << _streams[stream_id].packets.size())
          _packet_list.push_back(_streams[stream_id].packets);
          _streams[stream_id].packets.clear();
          return true;
        }

        bool Packetizer::processAudioPacket(PacketPtr ptr) {
          bool result = false;
          //put the packet into the entire PacketList
          if (ptr->getSize() > 0)
            _streams[ptr->getStreamIndex()].packets.push_back(ptr);

          //when we have enough audio packets for a ProcessUnit
          if (_streams[ptr->getStreamIndex()].packets.size() >= _streams[ptr->getStreamIndex()].min_packet_count) {
            //buildList when enough packets available
            result = buildList(ptr->getStreamIndex());
          }
          return result;
        }

        bool Packetizer::processVideoPacket(PacketPtr ptr) {
          bool result = false;
          _jut_queue[ptr->getStreamIndex()].push_back(ptr);

          if (_jut_queue[ptr->getStreamIndex()].size() >= _streams[ptr->getStreamIndex()].jut_size)
            _streams[ptr->getStreamIndex()].jut_size_reached = true;
          //processing packets in the Queue while reading from input stream
          if (_streams[ptr->getStreamIndex()].jut_size_reached) {
            //getting Packet from queue
            boost::shared_ptr<Packet> p = _jut_queue[ptr->getStreamIndex()].front();
            //remove packet from queue
            _jut_queue[ptr->getStreamIndex()].pop_front();

            _streams[ptr->getStreamIndex()].packets.push_back(p);
            //when we have enough packets for a ProcessUnit
            if (_jut_queue[ptr->getStreamIndex()].front()->isKeyFrame() &&
                _streams[ptr->getStreamIndex()].packets.size() >= _streams[ptr->getStreamIndex()].min_packet_count) {
              //setup b frame offset
              for (int a = 0; a < _jut_queue[ptr->getStreamIndex()].size() && a < _streams[ptr->getStreamIndex()].jut_size - 1; a++) {
                _streams[ptr->getStreamIndex()].packets.push_back(_jut_queue[ptr->getStreamIndex()][a]);
              }
            //buildList when enough packets available
              result = buildList(ptr->getStreamIndex());
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

