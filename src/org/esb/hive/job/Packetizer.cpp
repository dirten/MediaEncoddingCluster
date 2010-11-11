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

#include "org/esb/av/Encoder.h"
#include "org/esb/av/Decoder.h"
using namespace org::esb::av;
namespace org {
  namespace esb {
    namespace hive {
      namespace job {

        /**
        *
        */
        Packetizer::Packetizer(std::map<int, StreamData> stream_data) {
          _codec_min_packets[CODEC_TYPE_VIDEO] = MIN_VIDEO_PACKETS;
          _codec_min_packets[CODEC_TYPE_AUDIO] = MIN_AUDIO_PACKETS;

          _streams = stream_data;
          /**
          * init internal streams data
          */
          std::map<int, StreamData>::iterator it = _streams.begin();
          for (; it != _streams.end(); it++) {
            if ((*it).second.min_packet_count<=0)
              (*it).second.min_packet_count=_codec_min_packets[(*it).second.decoder.get()->getCodecType()];
            if ((*it).second.decoder.get() == NULL)
              LOGERROR("Decoder is NULL");
            if ((*it).second.encoder.get() == NULL)
              LOGERROR("Encoder is NULL");
            (*it).second.state = STATE_NOP;

          }
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

          if (_codec_overlap.find(_streams[ptr->getStreamIndex()].decoder->getCodecId()) == _codec_overlap.end()) {
            _codec_overlap[_streams[ptr->getStreamIndex()].decoder->getCodecId()] = 0;
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
          std::map<int, StreamData>::iterator streams=_streams.begin();
          for(;streams!=_streams.end();streams++){
            int stream_idx=(*streams).first;
            _packet_list.push_back(_streams[stream_idx].packets);
            _streams[stream_idx].packets.clear();
            _overlap_queue[stream_idx].clear();
          }          
        }

        /***
        * this function packetizes the GOPs in chunks between the IFrames
        * i normal cases the order of the Pictures are
        * here will be each group of Picture chunked into(all of them cutted in the position of an I-Frame)
        * IPPPPPPPPIPPPPPPPPIPPPPPPPP
        * IPPPPPPPP
        *          IPPPPPPPP
        *                   IPPPPPPPP
        *
        * in a MPeg Stream there is a reorder with BFrames
        * each group will be also chunked in the position of the I-Frame, but in the case of the delay from the Decoder
        * we will append the next packets until a P-Frame arrived.
        * IPBBPBBPBBIBBPBBPBBIBBPBBPBBIBBP
        * IPBBPBBPBBIBB
        *           I  PBBPBBIBB
        *                    I  PBBPBBIBB
        *
        *
        */
        bool Packetizer::processPacket(PacketPtr ptr) {
          bool result = false;
          int stream_idx = ptr->getStreamIndex();

          if (_streams[stream_idx].state == STATE_NOP && (ptr->isKeyFrame() || _streams[stream_idx].decoder->getCodecType() == CODEC_TYPE_AUDIO)) {
            _streams[stream_idx].state = STATE_START_I_FRAME;
          }

          if (_streams[stream_idx].state == STATE_START_I_FRAME && _streams[stream_idx].packets.size() >= _streams[stream_idx].min_packet_count && ptr->isKeyFrame()) {
            _streams[stream_idx].state = STATE_END_I_FRAME;
          }

          if (_streams[stream_idx].state == STATE_START_I_FRAME) {
            _streams[stream_idx].packets.push_back(ptr);
          }

          if (_streams[stream_idx].state == STATE_END_I_FRAME) {
            _overlap_queue[stream_idx].push_back(ptr);
          }
          /**
          * case handling for mpeg2 video packets(formaly streams with b frames)
          * */
          if (_streams[stream_idx].state == STATE_END_I_FRAME && (_streams[stream_idx].decoder->getCodecId() == CODEC_ID_MPEG2VIDEO && ptr->_pict_type == FF_P_TYPE)) {
            /*in the first roundtrip the stream packets look like this, the first B Frames are not removed*/
            /*_streams[stream_idx].packets =IBBPBBPBBPBB*/
            /*in the following roundtrip the stream packets look like this*/
            /*_streams[stream_idx].packets =IPBBPBBPBB*/
            /*_overlap_queue[stream_idx]   =IBBP*/
            _streams[stream_idx].state = STATE_START_I_FRAME;
            /**
            * appending the next IBB from the IBBP order to the actual packet_list
            * */
            _streams[stream_idx].packets.insert(_streams[stream_idx].packets.end(), _overlap_queue[stream_idx].begin(), _overlap_queue[stream_idx].end()-1 );
            /*_streams[stream_idx].packets =IBBPBBPBBPBBIBB*/
            /*_overlap_queue[stream_idx]   =IBBP*/

            _packet_list.push_back(_streams[stream_idx].packets);
            _streams[stream_idx].packets.clear();
            /*_streams[stream_idx].packets =    */
            /*_overlap_queue[stream_idx]   =IBBP*/


            /**
            * appending the IP frames from the IBBP order to the actual packet_list
            * that are the first and the last entries in the overlap queue
            * */
            _streams[stream_idx].packets.insert(_streams[stream_idx].packets.end(), _overlap_queue[stream_idx].begin(), _overlap_queue[stream_idx].begin()+1);
            _streams[stream_idx].packets.insert(_streams[stream_idx].packets.end(), _overlap_queue[stream_idx].end()-1, _overlap_queue[stream_idx].end());
//            _streams[stream_idx].packets.insert(_streams[stream_idx].packets.end(), _overlap_queue[stream_idx].begin(), _overlap_queue[stream_idx].end());
            /*_streams[stream_idx].packets =IP  */
            /*_overlap_queue[stream_idx]   =IBBP*/


            _overlap_queue[stream_idx].clear();
            /*_streams[stream_idx].packets =IP */
            /*_overlap_queue[stream_idx]   =   */
            result = true;

          } else
            /**
            * this is used for all audio streams 
            * */
            if (_streams[stream_idx].state == STATE_END_I_FRAME&&_streams[stream_idx].decoder->getCodecType() == CODEC_TYPE_AUDIO) {
              _streams[stream_idx].state = STATE_START_I_FRAME;
              /**copying all Packets into the actual ProcessUnit*/
              _packet_list.push_back(_streams[stream_idx].packets);
              /**clear out the PacketList, because they are in the ProcessUnit*/
              _streams[stream_idx].packets.clear();
              /**bring in the first I-Frame for the next Process Unit*/
              _streams[stream_idx].packets.insert(_streams[stream_idx].packets.end(), _overlap_queue[stream_idx].begin(), _overlap_queue[stream_idx].end());
              /**clear out the overlap queue, because there is only an I-Frame packet and this is now in the next ProcessUnit*/
              _overlap_queue[stream_idx].clear();
              result = true;
            }else
              /**
              * this is used for all video stream types except MPEG2 Video Streams
              * */
              if (_streams[stream_idx].state == STATE_END_I_FRAME&&_streams[stream_idx].decoder->getCodecId() != CODEC_ID_MPEG2VIDEO) {
                _streams[stream_idx].state = STATE_START_I_FRAME;
                /**copying all Packets into the actual ProcessUnit*/
                _packet_list.push_back(_streams[stream_idx].packets);
                /**clear out the PacketList, because they are in the ProcessUnit*/
                _streams[stream_idx].packets.clear();
                /**bring in the first I-Frame for the next Process Unit*/
                _streams[stream_idx].packets.insert(_streams[stream_idx].packets.end(), _overlap_queue[stream_idx].begin(), _overlap_queue[stream_idx].end());
                /**clear out the overlap queue, because there is only an I-Frame packet and this is now in the next ProcessUnit*/
                _overlap_queue[stream_idx].clear();
                result = true;
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

