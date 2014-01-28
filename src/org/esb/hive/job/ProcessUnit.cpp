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
#include "org/esb/av/Decoder.h"
#include "org/esb/av/Encoder.h"
#include "org/esb/av/AVFilter.h"

#include "org/esb/av/Sink.h"
#include "org/esb/av/AVPipe.h"

#include "org/esb/util/Log.h"
#include "org/esb/util/StringUtil.h"
#include "boost/thread/detail/thread.hpp"
#include "org/esb/config/config.h"
#include "org/esb/io/File.h"

using namespace org::esb::hive::job;
using namespace org::esb::av;

using org::esb::av::AVPipe;
bool toDebug = false;

class MyPacketSink : public AVPipe {
public:

    MyPacketSink():AVPipe() {
  }
  bool newFrame(Ptr<Frame>){return false;}
  bool newPacket(Ptr<Packet> p){
    LOGDEBUG("push packet into sink");
    pkts.push_back(p);

    return true;
  }

  std::list<boost::shared_ptr<Packet> > getList() {
    return pkts;
  }
private:
  std::list<boost::shared_ptr<Packet> > pkts;

};

ProcessUnit::ProcessUnit() {
  _target_stream = 0;
  _source_stream = 0;
  _frame_group = 0;
  _frame_count = 0;
  _process_unit = 0;
  _last_process_unit = false;
  _frameRateCompensateBase = 0.0;
  _gop_size = -1;
  _expected_frame_count = -1;
  _deinterlace = 0;
  _keep_aspect_ratio = 0;
  _discard_audio_bytes = -1;
  _converter = NULL;
  _job_id="";
  _sequence=0;
  id = 0;
  _fps=0;
}

ProcessUnit::~ProcessUnit() {
}

void ProcessUnit::setFps(int fps){
  _fps=fps;
}

int ProcessUnit::getFps(){
  return _fps;
}

void ProcessUnit::setJobId(std::string uuid) {
  _job_id = uuid;
}

std::string ProcessUnit::getJobId() {
  return _job_id;
}

void ProcessUnit::process() {
  std::ostringstream oss;
  _start = microsec_clock::local_time();
  if (false && _encoder->getCodecOption("multipass") == "1" || _encoder->getCodecOption("multipass") == "true") {
    LOGDEBUG("Two Pass Enabled");
    setProperty("2pass", "true");
    oss << org::esb::config::Config::get("hive.tmp_path");
    oss << "/";
    oss << boost::this_thread::get_id();
  }

  if (hasProperty("2pass")) {
    LOGDEBUG("Performing Pass 1");
    _encoder->setCodecOption("flags", "pass1");
    //_encoder->setCodecOption("g", org::esb::util::StringUtil::toString(_input_packets.size()));
    LOGDEBUG("Thread pass1:" << oss.str());
    _encoder->setCodecOption("passlogfile", oss.str());
    _encoder->setFlag(CODEC_FLAG_PASS1);
  }

  processInternal2();

  if (hasProperty("2pass")) {
    LOGDEBUG("Performing Pass 2");
    delete _converter;
    _output_packets.clear();
    _converter = NULL;
    //_decoder = boost::shared_ptr<Decoder>(new Decoder(*_decoder.get()));//_2passdecoder;
    //_encoder = boost::shared_ptr<Encoder>(new Encoder(*_encoder.get()));//_2passencoder;
    _decoder->reset();
    _encoder->reset();
    //_encoder->setCodecID(CODEC_ID_LIBXVID);
    _encoder->setCodecOption("flags", "pass2");
    //_encoder->setCodecOption("g", org::esb::util::StringUtil::toString(_input_packets.size()));
    _encoder->setCodecOption("passlogfile", oss.str());
    _encoder->setFlag(CODEC_FLAG_PASS2);

    _decoder->clearTargets();
    processInternal2();
  }
  org::esb::io::File statsfile(oss.str());
  _end = microsec_clock::local_time();
  if(statsfile.exists()){
    statsfile.deleteFile();
  }
  if(_encoder->getCodecType()==AVMEDIA_TYPE_VIDEO && _output_packets.size()>0){
    time_duration diff = _end - _start;
    if(diff.total_seconds()>0)
      _fps=_input_packets.size()/diff.total_seconds();
    else
      _fps=_input_packets.size();
  }
}

void ProcessUnit::processInternal2() {
  if (_decoder && !_decoder->open()) {
    LOGERROR("fail to open the decoder (ProcessUnitID:" << _process_unit << " CodecID:" << _decoder->getCodecId() << ")");
    return;
  }

  if (_encoder && !_encoder->open()) {
    LOGERROR("fail to open the encoder (ProcessUnitID:" << _process_unit << "CodecID:" << _encoder->getCodecId() << ")");
    return;
  }

  org::esb::av::AVFilter * filter=NULL;
  if(_decoder->getCodecType()==AVMEDIA_TYPE_AUDIO){
    filter=new org::esb::av::AVFilter(AUDIO,"aresample=%sample_rate%,aformat=sample_fmts=%sample_format%:channel_layouts=0x%channel_layout%");

    filter->setInputParameter("channel_layout",StringUtil::toString(_decoder->getChannelLayout()));
    filter->setInputParameter("sample_rate",StringUtil::toString(_decoder->getSampleRate()));
    filter->setInputParameter("sample_format", av_get_sample_fmt_name(_decoder->getSampleFormat()));
    filter->setInputParameter("time_base", "1/"+StringUtil::toString(_decoder->getTimeBase().den));

    filter->setOutputParameter("channel_layout",StringUtil::toString(_encoder->getChannelLayout()));
    filter->setOutputParameter("frame_size",StringUtil::toString(_encoder->ctx->frame_size));
    filter->setOutputParameter("sample_rate",StringUtil::toString(_encoder->getSampleRate()));
    filter->setOutputParameter("sample_format", av_get_sample_fmt_name(_encoder->getSampleFormat()));
    filter->setOutputParameter("frame_size",StringUtil::toString(_encoder->ctx->frame_size));
  }

  if(_decoder->getCodecType()==AVMEDIA_TYPE_VIDEO){
    filter=new org::esb::av::AVFilter(VIDEO,"scale=%width%:%height%");

    filter->setInputParameter("width",StringUtil::toString(_decoder->getWidth()));
    filter->setInputParameter("height",StringUtil::toString(_decoder->getHeight()));
    filter->setInputParameter("pixel_format",StringUtil::toString(_decoder->getPixelFormat()));
    filter->setInputParameter("time_base", StringUtil::toString(_decoder->getTimeBase().num)+"/"+StringUtil::toString(_decoder->getTimeBase().den));
    filter->setInputParameter("sample_aspect_ratio", StringUtil::toString(_decoder->ctx->sample_aspect_ratio.num)+"/"+StringUtil::toString(_decoder->ctx->sample_aspect_ratio.den));

    filter->setOutputParameter("width",StringUtil::toString(_encoder->getWidth()));
    filter->setOutputParameter("height",StringUtil::toString(_encoder->getHeight()));
    filter->setOutputParameter("pixel_format",StringUtil::toString(_encoder->getPixelFormat()));

  }

  filter->init();

  /*init the packet sink*/
  MyPacketSink sink;//=new MyPacketSink();


  /*build up the transcoding chain*/
  /*i need to build a special frame filter for frame count on a MPEG2 Stream with B-Frames*/
  _decoder->addTarget(filter);
  filter->addTarget(_encoder.get());
  _encoder->addTarget(&sink);

  list<boost::shared_ptr<Packet> >::iterator it = _input_packets.begin();

  for (; it != _input_packets.end();it++) {
    boost::shared_ptr<Packet> p = *it;
    _decoder->newPacket(p);
  }

  if(_decoder->getCodecType()==AVMEDIA_TYPE_VIDEO){
    LOGDEBUG("flush");

    /*sending an empty packet is implicit a flush for the pipe*/
    //if(_decoder->getCodecId()!=AV_CODEC_ID_MPEG2VIDEO && _decoder->getCodecOption("has_b_frames") != "1"){
      while(_decoder->newPacket(new Packet()));
    //}else{
      //while(filter->newPacket(new Packet()));
      //while(filter->newFrame(new Frame()));
    //}
  }

  _output_packets = sink.getList();
  if(_decoder->getCodecId()==AV_CODEC_ID_MPEG2VIDEO){
    //_output_packets.pop_back();
  }
}


boost::shared_ptr<Decoder> ProcessUnit::getDecoder() {
  return _decoder;
}

boost::shared_ptr<Encoder> ProcessUnit::getEncoder() {
  return _encoder;
}

std::list<boost::shared_ptr<Packet> > ProcessUnit::getInputPacketList() {
  return _input_packets;
}

std::list<boost::shared_ptr<Packet> > ProcessUnit::getOutputPacketList() {
  return _output_packets;
}

int ProcessUnit::getGopSize() {
  return _gop_size;
}

int ProcessUnit::getExpectedFrameCount() {
  return _expected_frame_count;
}

void ProcessUnit::processPsnr(Frame * ref, Frame * cmp) {

}

Frame * ProcessUnit::convertToRgb(Frame * ref) {
  return NULL;
}

std::string toString() {
  std::stringstream t;
  return t.str();
}

void ProcessUnit::setProperty(std::string k, std::string v) {
  _properties[k] = v;
}

std::string ProcessUnit::getProperty(std::string k) {
  return _properties[k];
}

bool ProcessUnit::hasProperty(std::string k) {
  return _properties.count(k) > 0;
}

