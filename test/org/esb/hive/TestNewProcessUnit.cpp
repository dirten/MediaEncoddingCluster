/* 
 * File:   TestNewProcessUnit.cpp
 * Author: jholscher
 *
 * Created on 27. Oktober 2009, 23:27
 */

#include <stdlib.h>
#include <map>
#include <list>

#include "org/esb/util/Log.h"
#include "org/esb/util/StringUtil.h"

#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/FormatOutputStream.h"

#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/PacketOutputStream.h"

#include "org/esb/av/Decoder.h"
#include "org/esb/av/Encoder.h"

#include "org/esb/av/FrameConverter.h"
#include "org/esb/av/Frame.h"

#include "org/esb/hive/job/Packetizer.h"
#include "org/esb/hive/job/ProcessUnit.h"

#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/io/FileInputStream.h"

using namespace org::esb::av;
using namespace org::esb::io;
using namespace org::esb::util;
using namespace org::esb::hive::job;

struct StreamData2 {
  boost::shared_ptr<Decoder> dec;
  boost::shared_ptr<Encoder> enc;
  FrameConverter * conv;
};
//CodecID video_codec_id = CODEC_ID_VP8;
CodecID video_codec_id = CODEC_ID_H264;
//CodecID audio_codec_id = CODEC_ID_VORBIS;
CodecID audio_codec_id = CODEC_ID_MP2;

/**
 *
 */
void build_process_units(int argc, char** argv) {
  map<int, int> _smap;

  std::map<int, Packetizer::StreamData> stream_data;

  /*open the fixed test File or the file from command line input*/
  std::string src;
  std::string trg;
  std::string logconfigpath = MEC_SOURCE_DIR;
  logconfigpath.append("/res");
  //Log::open(logconfigpath);
  if (argc == 1) {
    src = MEC_SOURCE_DIR;
    src.append("/test.dvd");
    trg = MEC_SOURCE_DIR;
    trg.append("/test.mp4");
  } else {
    src = argv[1];
    trg = argv[2];
  }


  /*opening the input file and Packet Input Stream*/
  File f(src.c_str());
  FormatInputStream fis(&f);
  PacketInputStream pis(&fis);


  /*opening the output file and Packet Output Stream*/
  File f_out(trg.c_str());
  FormatOutputStream fos(&f_out);
  PacketOutputStream pos(&fos);
  map<int, StreamData2> _sdata;

  /*Create and open the input and output Codecs*/
  int c = fis.getStreamCount();
  int s = 0;

  for (int i = 0; i < c; i++) {
    if (fis.getStreamInfo(i)->getCodecType() != AVMEDIA_TYPE_VIDEO &&
        fis.getStreamInfo(i)->getCodecType() != AVMEDIA_TYPE_AUDIO) continue;
    _sdata[i].dec = boost::shared_ptr<Decoder > (new Decoder(fis.getAVStream(i)));
    _sdata[i].enc = boost::shared_ptr<Encoder > (new Encoder());
    stream_data[i].decoder=_sdata[i].dec;
    stream_data[i].encoder=_sdata[i].enc;
	stream_data[i].min_packet_count=0;
//    stream_data[i].codec_id = fis.getStreamInfo(i)->getCodecId();

    if (_sdata[i].dec->getCodecType() == AVMEDIA_TYPE_VIDEO) {
      //      _sdata[i].enc->setCodecId(CODEC_ID_MPEG4);
      //_sdata[i].enc->setCodecId(CODEC_ID_MPEG2VIDEO);
      _sdata[i].enc->setCodecId(video_codec_id);
      _sdata[i].enc->setBitRate(1024000);
      _sdata[i].enc->setWidth(320);
      _sdata[i].enc->setHeight(240);
      _sdata[i].enc->setGopSize(12);
      AVRational ar;
      ar.num = 1;
      ar.den = 30;
      _sdata[i].enc->setTimeBase(ar);

    } else if (_sdata[i].dec->getCodecType() == AVMEDIA_TYPE_AUDIO) {
      _sdata[i].enc->setCodecId(audio_codec_id);
      _sdata[i].enc->setBitRate(128000);
      _sdata[i].enc->setSampleRate(44100);
      _sdata[i].enc->setChannels(2);
      _sdata[i].enc->setSampleFormat(_sdata[i].dec->getSampleFormat());
      AVRational ar;
      ar.num = 1;
      ar.den = 44100;
      _sdata[i].enc->setTimeBase(ar);
    }
    if (fos._fmt->flags & AVFMT_GLOBALHEADER)
      _sdata[i].enc->setFlag(CODEC_FLAG_GLOBAL_HEADER);

    //    _sdata[i].dec->open();
    _sdata[i].enc->open();
    _smap[i] = s++;
    _sdata[i].conv = new FrameConverter(_sdata[i].dec.get(), _sdata[i].enc.get());
    pos.setEncoder(*_sdata[i].enc, _smap[i]);
    //    _sdata[i].enc->setOutputStream(&pos);
  }
  pos.init();
  Packetizer pti(stream_data);
  int pcount = 0;

  //  if (!pos.init())goto cleanup;
  fos.dumpFormat();
  for (int a = 0; a < 50000; a++) {
    Packet * p;
    //reading a packet from the Stream
    //when no more packets available(EOF) then it return <0
    if ((p = pis.readPacket()) == NULL)break;
    boost::shared_ptr<Packet> pPacket(p);
    if (pti.putPacket(pPacket)) {
      boost::shared_ptr<ProcessUnit> u(new ProcessUnit());
      //u->setProperty("2pass", "true");
      u->_decoder = _sdata[pPacket->getStreamIndex()].dec;
      u->_encoder = _sdata[pPacket->getStreamIndex()].enc;
      std::list<boost::shared_ptr<Packet> > list;
      PacketListPtr l = pti.removePacketList();
      PacketListPtr::iterator it = l.begin();
      for (; it != l.end(); it++) {
        list.push_back(*it);
      }
      u->_input_packets = list;
      //      u->process();
      //      delete u->_decoder;
      //      delete u->_encoder;

      std::string target = MEC_SOURCE_DIR;
      target += "/pu.";
      target += StringUtil::toString(++pcount);
      target += ".pu";
      File out(target);
      FileOutputStream fospu(&out);
      ObjectOutputStream oos(&fospu);
      oos.writeObject(*u);

      //      u->process();
    }
//	pti.flushStreams();
  }

  //  fis.close();

  map<int, StreamData2>::iterator streams = _sdata.begin();
  for (; streams != _sdata.end(); streams++) {
    //    delete (*streams).second.enc;
    //	  av_freep( (*streams).second.dec->ctx->extradata);
    //	  (*streams).second.dec->ctx->extradata_size=0;
    delete (*streams).second.conv;
  }
pti.flushStreams();
}

void process_units() {

  char * file = new char[100];
  char * outfile = new char[100];

  for (int a = 1; true; a++) {
    std::string src = MEC_SOURCE_DIR;
    std::string trg = MEC_SOURCE_DIR;
    sprintf(file, "/pu.%d.pu", a);
    sprintf(outfile, "/pu.%d.out", a);
    org::esb::io::File infile(src.append(file));
    if (!infile.exists())break;
    LOGDEBUG("Loading ProcessUnit #"<<infile.getFileName());
    org::esb::io::FileInputStream fis(&infile);
    org::esb::io::ObjectInputStream ois(&fis);
    org::esb::hive::job::ProcessUnit pu;
    ois.readObject(pu);
    LOGDEBUG(pu._decoder);
    pu.process();
    FileOutputStream fos(trg.append(outfile).c_str());
    ObjectOutputStream oos(&fos);
    oos.writeObject(pu);
    //  delete pu._decoder;
    //  delete pu._encoder;
    delete pu._converter;
  }
  delete []file;
  delete []outfile;

}

void write_file(int argc, char** argv) {
  std::string trg;
  if (argc == 1) {
    trg = MEC_SOURCE_DIR;
    trg.append("/test.mp4");
  } else {
    trg = argv[2];
  }





  File f_out(trg.c_str());
  FormatOutputStream fos(&f_out);
  PacketOutputStream pos(&fos);
  char * file = new char[300];
  Encoder * video_codec = new Encoder(video_codec_id);
  video_codec->setBitRate(1024000);
  video_codec->setWidth(320);
  video_codec->setHeight(240);
  video_codec->setGopSize(12);
  AVRational ar;
  ar.num = 1;
  ar.den = 30;
  video_codec->setTimeBase(ar);

  Encoder * audio_codec = new Encoder(audio_codec_id);
  audio_codec->setBitRate(128000);
  audio_codec->setSampleRate(44100);
  audio_codec->setChannels(2);
  audio_codec->setSampleFormat(SAMPLE_FMT_S16);
  if (fos._fmt->flags & AVFMT_GLOBALHEADER) {
    video_codec->setFlag(CODEC_FLAG_GLOBAL_HEADER);
    audio_codec->setFlag(CODEC_FLAG_GLOBAL_HEADER);
  }
  video_codec->open();
  //  video_codec->ctx->extradata_size=0;

  audio_codec->open();
  //  video_codec->ctx->extradata_size=0;
  pos.setEncoder(*video_codec, 0);
  pos.setEncoder(*audio_codec, 1);
  pos.init();
  int pts = 0;
  for (int a = 1; true; a++) {
    sprintf(file,  "%s/pu.%d.out", MEC_SOURCE_DIR,a);
    org::esb::io::File infile(file);
    if (!infile.exists())break;
    org::esb::io::FileInputStream fis(&infile);
    org::esb::io::ObjectInputStream ois(&fis);
    org::esb::hive::job::ProcessUnit pu;
    ois.readObject(pu);
    std::list<boost::shared_ptr<Packet> >::iterator it = pu._output_packets.begin();
    for (; it != pu._output_packets.end(); it++) {
//      (*it)->setStreamIndex((*it)->getStreamIndex());
      if((*it)->getStreamIndex()==0){
        (*it)->setPts(pts++);
        (*it)->setDts(AV_NOPTS_VALUE);
      }
      (pos.writePacket(*((*it).get()))==0);
    }
  }
  pos.close();
  fos.close();
  delete video_codec;
  delete audio_codec;
  delete []file;
}

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open("");
  build_process_units(argc, argv);
  process_units();
  write_file(argc, argv);
  Log::close();
  return (EXIT_SUCCESS);
}

