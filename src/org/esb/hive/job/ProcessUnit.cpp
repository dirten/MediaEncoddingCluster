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
#include "org/esb/util/StringUtil.h"
#include "boost/thread/detail/thread.hpp"
#include "org/esb/config/config.h"
#include "org/esb/io/File.h"

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
  if (_encoder->getCodecOption("multipass") == "1" || _encoder->getCodecOption("multipass") == "true") {
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

  processInternal();

  if (hasProperty("2pass")) {
    LOGDEBUG("Performing Pass 2");
    delete _converter;
    _output_packets.clear();
    _converter = NULL;
    _decoder = _2passdecoder;

    _encoder = _2passencoder;
    //_encoder->setCodecID(CODEC_ID_LIBXVID);
    _encoder->setCodecOption("flags", "pass2");
    //_encoder->setCodecOption("g", org::esb::util::StringUtil::toString(_input_packets.size()));
    _encoder->setCodecOption("passlogfile", oss.str());
    _encoder->setFlag(CODEC_FLAG_PASS2);
    processInternal();
  }
  org::esb::io::File statsfile(oss.str());
  _end = microsec_clock::local_time();
  if(statsfile.exists()){
    statsfile.deleteFile();
  }
  if(_encoder->getCodecType()==AVMEDIA_TYPE_VIDEO){
    time_duration diff = _end - _start;
    if(diff.total_seconds()>0)
      _fps=_input_packets.size()/diff.total_seconds();
    else
      _fps=_input_packets.size();
  }
}

void ProcessUnit::processInternal() {

  LOGTRACEMETHOD("ProcessUnit");
  LOGDEBUG("CompensateBase" << _frameRateCompensateBase);
  int insize = 0, outsize = 0;

  if (_decoder != NULL)
    if (!_decoder->open()) {
      LOGERROR("fail to open the decoder (ProcessUnitID:" << _process_unit << " CodecID:" << _decoder->getCodecId() << ")");
      return;
    }
  if (_encoder != NULL)
    if (!_encoder->open()) {
      LOGERROR("fail to open the encoder (ProcessUnitID:" << _process_unit << "CodecID:" << _encoder->getCodecId() << ")");
      return;
    }
  /*creating a frame converter*/
  if (_converter == NULL)
    _converter = new FrameConverter(_decoder.get(), _encoder.get());
  _converter->setFrameRateCompensateBase(_frameRateCompensateBase);
  _converter->setGopSize(_gop_size);
  _converter->setDeinterlace(_deinterlace > 0);
  _converter->setKeepAspectRatio(_keep_aspect_ratio > 0);

  LOGTRACE("Codex openned");
  LOGTRACE(_decoder->toString());
  LOGTRACE(_encoder->toString());
  /*creating a packetsink for storing the encoded Packetsf from the encoder*/
  PacketSink sink;
  _encoder->setSink(&sink);
  _encoder->setOutputStream(NULL);

  /*configure the reference decoder to compute the psnr for video mages*/
  if (false && _encoder->getCodecType() == AVMEDIA_TYPE_VIDEO) {
    std::map<std::string, std::string>opt = _encoder->getCodecOptions();
    _refdecoder = boost::shared_ptr<Decoder > (new Decoder(_encoder->getCodecId()));
    std::map<std::string, std::string>::iterator opit = opt.begin();
    _refdecoder->setWidth(_encoder->getWidth());
    _refdecoder->setHeight(_encoder->getHeight());
    _refdecoder->setPixelFormat(_encoder->getPixelFormat());

    for (; opit != opt.end(); opit++) {
      if ((*opit).first != "extradata" || (*opit).first != "extradata_size")
        _refdecoder->setCodecOption((*opit).first, (*opit).second);
    }
    LOGDEBUG("EncoderExtrdataSize:" << _encoder->ctx->extradata_size);
    LOGDEBUG("RefDecoderExtrdataSize:" << _refdecoder->ctx->extradata_size);
    //std::cout << _encoder->ctx->extradata;
    _refdecoder->ctx->extradata = static_cast<uint8_t*> (av_malloc(_encoder->ctx->extradata_size));
    memcpy(_refdecoder->ctx->extradata, _encoder->ctx->extradata, _encoder->ctx->extradata_size);
    //_refdecoder->ctx->extradata[0] = 2;
    //_refdecoder->ctx->extradata_size=0;
    _refdecoder->open();
  }
  //  FrameConverter conv(_decoder, _encoder);

  list<boost::shared_ptr<Packet> >::iterator it;

  /*i dont know if we need this in the Future*/
  //  multiset<boost::shared_ptr<Frame>, PtsComparator > pts_list;
  //  multiset<boost::shared_ptr<Packet>, PtsPacketComparator > pts_packets;
  int64_t last_pts = -1;
  bool compute_delayed_frames = false;
  int stream_index = -1;
  int loop_count = 0;
  /*loop over each Packet received */
  it = _input_packets.begin();
  for (; it != _input_packets.end() || compute_delayed_frames;) {
    LOGDEBUG("Loop:" << ++loop_count);
    /*get the Packet Pointer from the list*/
    boost::shared_ptr<Packet> p;
    /**
     * special handling for delayed packets from the decoder
     * @TODO: redesign needed for a simpler maintenance
     */
    if (!compute_delayed_frames) {
      p = *it;
      stream_index = p->getStreamIndex();
      it++;
    } else {
      LOGDEBUG("delayed frame");
      p = boost::shared_ptr<Packet > (new Packet());
      p->setTimeBase(_input_packets.front()->getTimeBase());
      p->setDuration(_input_packets.front()->getDuration());
      p->setStreamIndex(stream_index);
    }
    /*sum the packet sizes for later output*/
    insize += p->packet->size;
    LOGTRACE("Inputpacket:" << p->toString());
    p->toString();
    /*Decoding the Packet into a Frame*/
    Frame * tmp = _decoder->decode2(*p);

    LOGTRACE("Frame Decoded:" << tmp->toString());
    /*when frame not finished, then it is nothing todo, continue with the next packet*/
    if (!tmp->isFinished()) {
      delete tmp;
      compute_delayed_frames = false;
      continue;
    }


    if (_decoder->getCodecId() != CODEC_ID_MPEG2VIDEO && it == _input_packets.end()) {
      LOGDEBUG("setting compute_delayed_frames=true");
      compute_delayed_frames = true;
    }
    //      LOGTRACE("org.esb.hive.job.ProcessUnit","Frame Buffer > 0");

    /*target frame for conversion*/
    Frame * f = NULL;
    /*allocation frame data for specified type*/
    if (_decoder->ctx->codec_type == AVMEDIA_TYPE_VIDEO)
      f = new Frame(_encoder->getInputFormat().pixel_format, _encoder->getWidth(), _encoder->getHeight());
    if (_decoder->ctx->codec_type == AVMEDIA_TYPE_AUDIO)
      f = new Frame();
    LOGTRACE("try Frame Convert");
    /*converting the source frame to target frame*/
    _converter->convert(*tmp, *f);
    //this could be usefull when one pass encoding on x264,
    //because the first encoded frames are very poor
    //if(false&&loop_count<5)
    //    f->getAVFrame()->pict_type=AV_PICTURE_TYPE_I;
    /**
     * @TODO: prepend silent audio bytes to prevent audio/video desync in distributed audio encoding
     * */
    if (false && _decoder->ctx->codec_type == AVMEDIA_TYPE_AUDIO &&
            _discard_audio_bytes > 0) {
      size_t size = f->_size + _discard_audio_bytes;
      uint8_t * tmp_buf = (uint8_t*) av_malloc(size);
      memset(tmp_buf, 0, size);
      memcpy(tmp_buf + _discard_audio_bytes, f->_buffer, f->_size);
      av_free(f->_buffer);
      f->_buffer = tmp_buf;
      f->_size = size;
      _discard_audio_bytes = 0;
    }

    LOGTRACE("Frame Converted" << f->toString());

    /*encode the frame into a packet*/
    /*NOTE: the encoder write Packets to the PacketSink, because some codecs duplicates frames*/

    /**
     * @TODO: simply it is better to calculate the psnr in the encoder!!!
     */

    int ret = _encoder->encode(*f);
    LOGTRACE("Frame Encoded");
    //LOGDEBUG("Stats="<<_encoder->ctx->stats_out);
    if (false && _encoder->getCodecType() == AVMEDIA_TYPE_VIDEO && sink.getList().size() > 0) {
      boost::shared_ptr<Packet>enc_packet = sink.getList().back();
      Frame * tmpf = _refdecoder->decode2(*enc_packet.get());
      if (tmpf->isFinished()) {
        LOGDEBUG("Reference Frame Decoded!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1");
        processPsnr(f, tmpf);
      }
      delete tmpf;
    }
    delete tmp;
    delete f;
    outsize += ret;
  }
  /*now process the delayed Frames from the encoder*/
  LOGTRACE("Encode Packet delay");
  bool have_more_frames = _encoder->getCodecType() == AVMEDIA_TYPE_VIDEO;
  while (have_more_frames) {
    if (_encoder->encode() <= 0) {
      have_more_frames = 0;
    }
  }
  _output_packets = sink.getList();
  if (_expected_frame_count != -1 && _output_packets.size() != _expected_frame_count)
    LOGWARN("PUID=" << _process_unit << " Expected Frame count diff from resulting Frame count: expected=" << _expected_frame_count << " got=" << _output_packets.size())
    //  _encoder->close();
    //  _decoder->close();
    //      delete _converter;
    //    _converter=NULL;

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

