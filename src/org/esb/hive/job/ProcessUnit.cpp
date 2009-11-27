/*----------------------------------------------------------------------
 *  File    : ProcessUnit.cpp
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
#include "ProcessUnit.h"
#include "org/esb/av/Frame.h"
#include "org/esb/av/FrameFormat.h"

#include "org/esb/av/Sink.h"

#include "org/esb/util/Log.h"

using namespace org::esb::hive::job;
using namespace org::esb::av;

bool toDebug = false;

class PacketSink : public Sink {
public:

  PacketSink() {
  }

  void write(void * p) {
    Packet* pt = (Packet*) p;
    boost::shared_ptr<Packet> pEnc(new Packet(*pt));
    pkts.push_back(pEnc);
    if (toDebug) {
      logdebug("outputpacket");
      pEnc->toString();
    }
  }

  std::list<boost::shared_ptr<Packet> > getList() {
    return pkts;
  }
private:
  std::list<boost::shared_ptr<Packet> > pkts;

};

ProcessUnit::ProcessUnit() {
  _decoder = NULL;
  _encoder = NULL;
  _converter=NULL;
  codec = NULL;
  _target_stream = 0;
  _source_stream = 0;
  _frame_group = 0;
  _frame_count = 0;
  _process_unit = 0;
  _last_process_unit=false;
  id = 0;
  
}

ProcessUnit::~ProcessUnit() {
  /*
    if (_decoder != NULL)
      delete _decoder;
    if (_encoder != NULL)
      delete _encoder;
    _decoder = NULL;
    _encoder = NULL;
   */
  //    _decoder=NULL;
  //    _encoder=NULL;

  //    _target_stream=0;
  //    _input_packets.clear();
  //    _output_packets.clear();

}

void ProcessUnit::process() {
  int insize = 0, outsize = 0;

  if (_decoder != NULL)
    _decoder->open();
  if (_encoder != NULL)
    _encoder->open();
  /*creating a frame converter*/
  if(_converter==NULL)
    _converter=new FrameConverter(_decoder, _encoder);
  if (toDebug) {
    logdebug("Codex openned");
    logdebug(_decoder->toString());
    logdebug(_encoder->toString());
  }
  /*creating a packetsink for storing the encoded Packetsf from the encoder*/
  PacketSink sink;
  _encoder->setSink(&sink);
  _encoder->setOutputStream(NULL);


//  FrameConverter conv(_decoder, _encoder);

  if (toDebug)
    logdebug("Converter created");

  list< boost::shared_ptr<Packet> >::iterator it;

  /*i dont know if we need this in the Future*/
  //  multiset<boost::shared_ptr<Frame>, PtsComparator > pts_list;
  //  multiset<boost::shared_ptr<Packet>, PtsPacketComparator > pts_packets;
  int64_t last_pts=-1;
  /*loop over each Packet received */
  for (it = _input_packets.begin(); it != _input_packets.end(); it++) {
    if (toDebug)
      logdebug("Loop");
    /*get the Packet Pointer from the list*/
    boost::shared_ptr<Packet> p = *it;

    /*sum the packet sizes for later output*/
    insize += p->packet->size;
    if (toDebug) {
      logdebug("inputpacket")
      p->toString();
    }
    /*Decoding the Packet into a Frame*/
    Frame * tmp = _decoder->decode2(*p);

    if (toDebug) {
      logdebug("Frame Decoded");
      logdebug(tmp->toString());
    }
    /*when frame not finished, then it is nothing to to and continue with the next packet*/
    if (!tmp->isFinished()) {
      delete tmp;
      continue;
    }
    if (toDebug)
      logdebug("Frame Buffer > 0");

    /*target frame for conversion*/
    Frame * f = NULL;

    /*allocation frame data for specified type*/
    if (_decoder->ctx->codec_type == CODEC_TYPE_VIDEO)
      f = new Frame(_encoder->getPixelFormat(), _encoder->getWidth(), _encoder->getHeight());
    if (_decoder->ctx->codec_type == CODEC_TYPE_AUDIO)
      f = new Frame();
    if (toDebug)
      logdebug("try Frame Convert");
    /*converting the source frame to target frame*/
    _converter->convert(*tmp, *f);

    /**
     * this is for down rating the frame rate, e.g. from 25fps down to 15fps...
     * there are some pictures to drop!
     * @TODO: test out if this is ok when some Rational Framerate is given, e.g. 23.976fps?
     */
    if(last_pts>0&&last_pts==f->getPts()){
      delete f;
      delete tmp;
      continue;
    }
    last_pts=f->getPts();



    if (toDebug) {
      logdebug("Frame Converted");
      f->toString();
    }

    /*encode the frame into a packet*/
    /*NOTE: the encoder write Packets to the PacketSink, because some codecs duplicates frames*/
    int ret = _encoder->encode(*f);
    delete tmp;
    delete f;
    //cout << "PacketPts:" << ret.packet->pts << "\tPacketDts:" << ret.packet->dts << "\t";
    if (toDebug)
      logdebug("Frame Encoded");

    //    boost::shared_ptr<Packet> pEnc(new Packet(ret));
    if (toDebug)
      logdebug("Packet Created");

    //	    pEnc->packet->dts=AV_NOPTS_VALUE;
    outsize += ret;
  }
  /*now process the delayed Frames from the encoder*/
  logdebug("Encode Packet delay");
  bool have_more_frames=true;
  while(have_more_frames){
    if(_encoder->encode()<=0){
      have_more_frames=0;
    }
  }
  /*
    if (_decoder != NULL)
      delete _decoder;
    if (_encoder != NULL)
      delete _encoder;
    _decoder = 0;
    _encoder = 0;
   */
  _output_packets = sink.getList();

}

std::string toString() {
  std::stringstream t;
  return t.str();
}
