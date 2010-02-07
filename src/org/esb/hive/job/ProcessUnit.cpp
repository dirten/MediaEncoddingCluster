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
  }

  std::list<boost::shared_ptr<Packet> > getList() {
    return pkts;
  }
private:
  std::list<boost::shared_ptr<Packet> > pkts;

};

ProcessUnit::ProcessUnit() {
/*
  _decoder = NULL;
  _encoder = NULL;*/
  _converter=NULL;
  _target_stream = 0;
  _source_stream = 0;
  _frame_group = 0;
  _frame_count = 0;
  _process_unit = 0;
  _last_process_unit=false;
  id = 0;
  _frameRateCompensateBase=0.0;
  _gop_size=-1;
  _expected_frame_count=-1;
  
}

ProcessUnit::~ProcessUnit() {
}

void ProcessUnit::process() {
  LOGTRACEMETHOD("org.esb.hive.job.ProcessUnit","ProcessUnit");
  int insize = 0, outsize = 0;

  if (_decoder != NULL)
    if(!_decoder->open()){
      LOGERROR("org.esb.hive.job.ProcessUnit","fail to open the decoder (ProcessUnitID:"<<_process_unit<<")");
      return;
    }
  if (_encoder != NULL)
    if(!_encoder->open()){
      LOGERROR("org.esb.hive.job.ProcessUnit","fail to open the encoder (ProcessUnitID:"<<_process_unit<<")");
      return;
    }
  /*creating a frame converter*/
  if(_converter==NULL)
    _converter=new FrameConverter(_decoder.get(), _encoder.get());
  _converter->setFrameRateCompensateBase(_frameRateCompensateBase);
  _converter->setGopSize(_gop_size);
    LOGTRACE("org.esb.hive.job.ProcessUnit","Codex openned");
    LOGTRACE("org.esb.hive.job.ProcessUnit",_decoder->toString());
    LOGTRACE("org.esb.hive.job.ProcessUnit",_encoder->toString());
  /*creating a packetsink for storing the encoded Packetsf from the encoder*/
  PacketSink sink;
  _encoder->setSink(&sink);
  _encoder->setOutputStream(NULL);


//  FrameConverter conv(_decoder, _encoder);

  list< boost::shared_ptr<Packet> >::iterator it;

  /*i dont know if we need this in the Future*/
  //  multiset<boost::shared_ptr<Frame>, PtsComparator > pts_list;
  //  multiset<boost::shared_ptr<Packet>, PtsPacketComparator > pts_packets;
  int64_t last_pts=-1;
  bool compute_delayed_frames=false;
  /*loop over each Packet received */
  for (it = _input_packets.begin(); it != _input_packets.end()||compute_delayed_frames; ) {
      LOGTRACE("org.esb.hive.job.ProcessUnit","Loop");
    /*get the Packet Pointer from the list*/
      boost::shared_ptr<Packet> p;
      if(!compute_delayed_frames){
        p = *it;
        it++;
      }
      else{
        p=boost::shared_ptr<Packet>(new Packet());
        p->setTimeBase(_input_packets.front()->getTimeBase());
        p->setDuration(_input_packets.front()->getDuration());
      }
    /*sum the packet sizes for later output*/
    insize += p->packet->size;
      LOGTRACE("org.esb.hive.job.ProcessUnit","Inputpacket:"<<p->toString());
      p->toString();
    /*Decoding the Packet into a Frame*/
    Frame * tmp = _decoder->decode2(*p);

      LOGTRACE("org.esb.hive.job.ProcessUnit","Frame Decoded:"<<tmp->toString());
    /*when frame not finished, then it is nothing todo, continue with the next packet*/
    if (!tmp->isFinished()) {
      delete tmp;
      compute_delayed_frames=false;
      continue;
    }
    if(_decoder->getCodecType()==CODEC_TYPE_VIDEO&&it == _input_packets.end())
      compute_delayed_frames=true;
//      LOGTRACE("org.esb.hive.job.ProcessUnit","Frame Buffer > 0");

    /*target frame for conversion*/
    Frame * f = NULL;

    /*allocation frame data for specified type*/
    if (_decoder->ctx->codec_type == CODEC_TYPE_VIDEO)
      f = new Frame(_encoder->getPixelFormat(), _encoder->getWidth(), _encoder->getHeight());
    if (_decoder->ctx->codec_type == CODEC_TYPE_AUDIO)
      f = new Frame();
      LOGTRACE("org.esb.hive.job.ProcessUnit","try Frame Convert");
    /*converting the source frame to target frame*/
    _converter->convert(*tmp, *f);

    /**
     * this is for down rating the frame rate, e.g. from 25fps down to 15fps...
     * there are some pictures to drop!
     * @TODO: test out if this is ok when some Rational Framerate is given, e.g. 23.976fps?
     * @TODO: is it possible to make this in the decoder or encoder or frameconverter?
     */
    if(false&&last_pts>0&&last_pts==f->getPts()){
      delete f;
      delete tmp;
      continue;
    }
    last_pts=f->getPts();



      LOGTRACE("org.esb.hive.job.ProcessUnit","Frame Converted"<<f->toString());
      

    /*encode the frame into a packet*/
    /*NOTE: the encoder write Packets to the PacketSink, because some codecs duplicates frames*/
    int ret = _encoder->encode(*f);
    delete tmp;
    delete f;
    LOGTRACE("org.esb.hive.job.ProcessUnit","Frame Encoded");
    outsize += ret;
  }
  /*now process the delayed Frames from the encoder*/
  LOGTRACE("org.esb.hive.job.ProcessUnit","Encode Packet delay");
  bool have_more_frames=_encoder->getCodecType()==CODEC_TYPE_VIDEO;
  while(have_more_frames){
    if(_encoder->encode()<=0){
      have_more_frames=0;
    }
  }
  _output_packets = sink.getList();
  if(_expected_frame_count!=-1&&_output_packets.size()!=_expected_frame_count)
    LOGERROR("org.esb.hive.job.ProcessUnit","Expected Frame cont differ from resulting Frame count: expected="<<_expected_frame_count<<" got="<<_output_packets.size())
}

std::string toString() {
  std::stringstream t;
  return t.str();
}
