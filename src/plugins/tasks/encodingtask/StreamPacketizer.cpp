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
#include "org/esb/util/StringUtil.h"
#include "org/esb/util/Foreach.h"
using namespace org::esb::av;
namespace encodingtask {

  class StreamPacketizerPacketSink : public AVPipe {
  public:
    StreamPacketizerPacketSink():AVPipe() {
    }

    bool newFrame(Ptr<Frame>){return false;}

    bool newPacket(Ptr<Packet> p){
      pkts.push_back(p);

      return true;
    }

    std::list<boost::shared_ptr<Packet> > getList() {
      return pkts;
    }

  private:
    std::list<boost::shared_ptr<Packet> > pkts;
  };

  /**
  *
  */
  StreamPacketizer::StreamPacketizer()
  {
  }

  StreamPacketizer::StreamPacketizer(org::esb::av::Encoder * encoder, Ptr<org::esb::av::Decoder> decoder){
    _codec_min_packets[AVMEDIA_TYPE_VIDEO] = MIN_VIDEO_PACKETS;
    _codec_min_packets[AVMEDIA_TYPE_AUDIO] = MIN_AUDIO_PACKETS;

    /**
         * init internal streams data
         */
    if (!decoder)
      LOGERROR("Decoder is NULL");
    if (!encoder)
      LOGERROR("Encoder is NULL");
    //LOGDEBUG("CodecId:"<<decoder->getCodecId());
    _decoder=decoder;
    encoder_sample_rate=encoder->getSampleRate();
    encoder_frame_size=encoder->ctx->frame_size;
    //_encoder=encoder;
    _stream.min_packet_count = 0;// _codec_min_packets[decoder->getCodecType()];
    _stream.state = STATE_NOP;
    delay=0;
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
    delay=0;
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
    calulatePUSizeForPacket(ptr);
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
  int StreamPacketizer::euklid(int a, int b)
  {
    if (b == 0)
      return a;
     else
      return euklid(b, a % b);

  }

  void StreamPacketizer::calulatePUSizeForPacket(org::esb::av::PacketPtr p){
    if(_stream.min_packet_count==0){
      if(_decoder->getCodecType()==AVMEDIA_TYPE_AUDIO){

        /*calculation the frame boundaries*/
        int dec_frame_size=p->getDuration();
        dec_frame_size*=10;
        dec_frame_size=av_rescale_q(dec_frame_size, p->getTimeBase(), Rational(1,encoder_sample_rate));

        int gcd=av_gcd(dec_frame_size, encoder_frame_size);
        int lcm=(dec_frame_size*encoder_frame_size*10)/gcd;

        _stream.min_packet_count=(lcm*10)/dec_frame_size;
      }
      if(_decoder->getCodecType()==AVMEDIA_TYPE_VIDEO){
        _stream.min_packet_count=MIN_VIDEO_PACKETS;
      }
    }
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
       * each group will also be chunked in the position of the I-Frame, but in the case of the delay from the Decoder
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

    if(_stream.state == STATE_END_I_FRAME && (_decoder->getCodecOption("has_b_frames") == "1")){
      LOGDEBUG("has a bframe")
          PacketPtr iFrame=_overlap_queue.front();
      delay++;
      //if(delay>=3){

      /*this makes the assumption, that B-Frames has allways a lower pts than the previous I-Frame*/
      if(iFrame->getPts()<ptr->getPts()){
        _stream.state = STATE_START_I_FRAME;
        _stream.packets.insert(_stream.packets.end(), _overlap_queue.begin(), _overlap_queue.end()-1);
        _packet_list.push_back(_stream.packets);
        _stream.packets.clear();


        _stream.packets.push_back(iFrame);

        foreach(PacketPtr packet,_overlap_queue){
          if(packet->getPts()>iFrame->getPts()){
            _stream.packets.push_back(packet);
          }
        }
        //_stream.packets.insert(_stream.packets.end(), _overlap_queue.begin(), _overlap_queue.end());

        _overlap_queue.clear();

        result=true;
        delay=0;
      }
    }else
      /**
         * case handling for mpeg2 video packets(formaly streams with b frames)
         * */
      if (false && _stream.state == STATE_END_I_FRAME && (_decoder->getCodecId() == CODEC_ID_MPEG2VIDEO /*&& ptr->_pict_type == AV_PICTURE_TYPE_P*/)) {
        LOGDEBUG("decode Mpeg2 Stream");

        //ptr->getAVPacket()->flags
        Ptr<Frame> frame=_decoder->decode2(*ptr.get());
        LOGDEBUG("Frame:"<<frame->toString());
        LOGDEBUG("FrameType:"<<frame->getAVFrame()->pict_type);
        if(!frame->isFinished()){
          delay++;
        }else
          //Packet :IBBPBBPBBPBBPBBP
          //Frame  :
          if(frame->getAVFrame()->pict_type== AV_PICTURE_TYPE_P){
            LOGDEBUG("Prediction Picture with delay of "<<delay)
                //IBBPBBPBBP
                //   IBBP
                //LOGDEBUG("frame type = "+ org::esb::util::StringUtil::toString(frame->getAVFrame()->pict_type));
                /*in the first roundtrip the stream packets look like this, the first B Frames are not removed*/
                /*_streams[stream_idx].packets =IBBPBBPBBPBB*/
                /*in the following roundtrip the stream packets look like this*/
                /*_streams[stream_idx].packets =IPBBPBBPBB*/
                /*_overlap_queue[stream_idx]   =IBBP*/
                _stream.state = STATE_START_I_FRAME;
            /**
           * appending the next IBB from the IBBP order to the actual packet_list
           * */
            _stream.packets.insert(_stream.packets.end(), _overlap_queue.begin(), _overlap_queue.end() - (1+delay));
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
            _stream.packets.insert(_stream.packets.end(), _overlap_queue.end() - (1+delay), _overlap_queue.end());
            //            _streams[stream_idx].packets.insert(_streams[stream_idx].packets.end(), _overlap_queue[stream_idx].begin(), _overlap_queue[stream_idx].end());
            /*_streams[stream_idx].packets =IP  */
            /*_overlap_queue[stream_idx]   =IBBP*/


            _overlap_queue.clear();
            /*_streams[stream_idx].packets =IP */
            /*_overlap_queue[stream_idx]   =   */
            result = true;
            delay=0;
            _decoder->reset();
          }
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
      } else if (_stream.state == STATE_END_I_FRAME) {
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


