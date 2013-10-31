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
#include "StreamPacketizer.h"

#include "org/esb/av/Encoder.h"
#include "org/esb/av/Decoder.h"
using namespace org::esb::av;
namespace encodingtask {

  /**
  *
  */
  StreamPacketizer::StreamPacketizer()
  {
  }

  StreamPacketizer::StreamPacketizer(int size, Ptr<Decoder> decoder) {
    _codec_min_packets[AVMEDIA_TYPE_VIDEO] = MIN_VIDEO_PACKETS;
    _codec_min_packets[AVMEDIA_TYPE_AUDIO] = MIN_AUDIO_PACKETS;

    /**
         * init internal streams data
         */
    if (!decoder)
      LOGERROR("Decoder is NULL");
    //LOGDEBUG("CodecId:"<<decoder->getCodecId());
    _decoder=decoder;
    _stream.min_packet_count = size<=0?_codec_min_packets[decoder->getCodecType()]:size;
    _stream.state = STATE_NOP;
  }

  /**
       *
       */
  StreamPacketizer::~StreamPacketizer() {

  }

  /**
       *
       */
  bool StreamPacketizer::putPacket(PacketPtr ptr) {
    bool result = false;

    if (_codec_overlap.find(_decoder->getCodecId()) == _codec_overlap.end()) {
      _codec_overlap[_decoder->getCodecId()] = 0;
    }
    result = processPacket(ptr);
    return result;
  }

  /**
       *
       */
  PacketListPtr StreamPacketizer::getPacketList() {
    PacketListPtr t;
    if (_packet_list.size() > 0) {
      t = _packet_list.front();
    }
    return t;
  }

  PacketListPtr StreamPacketizer::removePacketList() {
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
  int StreamPacketizer::getPacketListCount() {
    return _packet_list.size();
  }

  Ptr<Decoder> StreamPacketizer::getDecoder()
  {
    return _decoder;
  }

  /**
       *
       */
  void StreamPacketizer::flushStreams() {
    _packet_list.push_back(_stream.packets);
    _stream.packets.clear();
    _overlap_queue.clear();
  }

  /***
       * this function packetizes the GOPs in chunks between the IFrames
       * i normal cases the order of the Pictures are
       * here will be each group of Picture be chunked, all of them cutted in the position of an I-Frame
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
  bool StreamPacketizer::processPacket(PacketPtr ptr) {
    bool result = false;
    //int stream_idx = ptr->getStreamIndex();

    if (_stream.state == STATE_NOP && (ptr->isKeyFrame() || _decoder->getCodecType() == AVMEDIA_TYPE_AUDIO)) {
      _stream.state = STATE_START_I_FRAME;
    }

    if (_stream.state == STATE_START_I_FRAME && _stream.packets.size() >= _stream.min_packet_count && ptr->isKeyFrame()) {
      _stream.state = STATE_END_I_FRAME;
    }

    if (_stream.state == STATE_START_I_FRAME) {
      _stream.packets.push_back(ptr);
    }

    if (_stream.state == STATE_END_I_FRAME) {
      _overlap_queue.push_back(ptr);
    }
    /**
         * case handling for mpeg2 video packets(formaly streams with b frames)
         * */
    if (_stream.state == STATE_END_I_FRAME && (_decoder->getCodecId() == CODEC_ID_MPEG2VIDEO && ptr->_pict_type == AV_PICTURE_TYPE_P)) {
      //LOGDEBUG("Processing Mpeg2 Stream");
      /*in the first roundtrip the stream packets look like this, the first B Frames are not removed*/
      /*_streams[stream_idx].packets =IBBPBBPBBPBB*/
      /*in the following roundtrip the stream packets look like this*/
      /*_streams[stream_idx].packets =IPBBPBBPBB*/
      /*_overlap_queue[stream_idx]   =IBBP*/
      _stream.state = STATE_START_I_FRAME;
      /**
           * appending the next IBB from the IBBP order to the actual packet_list
           * */
      _stream.packets.insert(_stream.packets.end(), _overlap_queue.begin(), _overlap_queue.end() - 1);
      /*_streams[stream_idx].packets =IBBPBBPBBPBBIBB*/
      /*_overlap_queue[stream_idx]   =IBBP*/

      _packet_list.push_back(_stream.packets);
      _stream.packets.clear();
      /*_streams[stream_idx].packets =    */
      /*_overlap_queue[stream_idx]   =IBBP*/


      /**
           * appending the IP frames from the IBBP order to the actual packet_list
           * that are the first and the last entries in the overlap queue
           * */
      _stream.packets.insert(_stream.packets.end(), _overlap_queue.begin(), _overlap_queue.begin() + 1);
      _stream.packets.insert(_stream.packets.end(), _overlap_queue.end() - 1, _overlap_queue.end());
      //            _streams[stream_idx].packets.insert(_streams[stream_idx].packets.end(), _overlap_queue[stream_idx].begin(), _overlap_queue[stream_idx].end());
      /*_streams[stream_idx].packets =IP  */
      /*_overlap_queue[stream_idx]   =IBBP*/


      _overlap_queue.clear();
      /*_streams[stream_idx].packets =IP */
      /*_overlap_queue[stream_idx]   =   */
      result = true;

    } else if (_stream.state == STATE_END_I_FRAME && _decoder->getCodecType() == AVMEDIA_TYPE_AUDIO) {
      /**************************************
           * this is used for all audio streams
           **************************************/
      _stream.state = STATE_START_I_FRAME;
      /**copying all Packets into the actual ProcessUnit*/
      _packet_list.push_back(_stream.packets);
      /**clear out the PacketList, because they are in the ProcessUnit*/
      _stream.packets.clear();
      /**bring in the first I-Frame for the next Process Unit*/
      _stream.packets.insert(_stream.packets.end(), _overlap_queue.begin(), _overlap_queue.end());
      /**clear out the overlap queue, because there is only an I-Frame packet and this is now in the next ProcessUnit*/
      _overlap_queue.clear();
      result = true;
    } else if (_stream.state == STATE_END_I_FRAME && _decoder->getCodecId() != CODEC_ID_MPEG2VIDEO) {
      /**********************************************************************
           * this is used for all video stream types except MPEG2 Video Streams
           **********************************************************************/

      _stream.state = STATE_START_I_FRAME;
      /**copying all Packets into the actual ProcessUnit*/
      _packet_list.push_back(_stream.packets);
      /**clear out the PacketList, because they are in the ProcessUnit*/
      _stream.packets.clear();
      /**bring in the first I-Frame for the next Process Unit*/
      _stream.packets.insert(_stream.packets.end(), _overlap_queue.begin(), _overlap_queue.end());
      /**clear out the overlap queue, because there is only an I-Frame packet and this is now in the next ProcessUnit*/
      _overlap_queue.clear();
      result = true;
    }
    return result;
  }
}

