/* 
 * File:   TestFullTranscoding.cpp
 * Author: jhoelscher
 *
 * Created on 23. Oktober 2009, 10:17
 */

#include <stdlib.h>
#include <map>

#include "org/esb/util/Log.h"

#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/FormatOutputStream.h"

#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/PacketOutputStream.h"

#include "org/esb/av/Decoder.h"
#include "org/esb/av/Encoder.h"

#include "org/esb/av/FrameConverter.h"
#include "org/esb/av/Frame.h"
using namespace org::esb::av;

struct StreamData {
  Decoder * dec;
  Encoder * enc;
  FrameConverter * conv;
  int64_t start_dts;
  bool more_frames;
};

map<int, StreamData> _sdata;
map<int, int> _smap;

/*
 * 
 */
int main(int argc, char** argv) {
Log::open("");
  /*open the fixed test File or the file from command line input*/
  std::string src;
  std::string trg;
  if (argc == 1) {
    src = MEC_SOURCE_DIR;
    src.append("/test.dvd");
    trg = MEC_SOURCE_DIR;
    trg.append("/test2.mp4");
  } else {
    src = argv[1];
    trg = argv[2];
  }


  /*opening the input file and Packet Input Stream*/
  File f(src.c_str());
  FormatInputStream fis(&f);
  PacketInputStream pis(&fis, true, true);


  /*opening the output file and Packet Output Stream*/
  File f_out(trg.c_str());
  FormatOutputStream fos(&f_out);
  PacketOutputStream pos(&fos);

  /*Create and open the input and output Codecs*/
  int c = fis.getStreamCount();
  int s = 0;
  bool video = false, audio = false;
  for (int i = 0; i < c; i++) {
    if (fis.getStreamInfo(i)->getCodecType() != AVMEDIA_TYPE_VIDEO &&
        fis.getStreamInfo(i)->getCodecType() != AVMEDIA_TYPE_AUDIO) continue;
    //if(i!=4&&i!=1)continue;
    if (audio && video)continue;
    fis.dumpFormat();
    _sdata[i].dec = new Decoder(fis.getAVStream(i));
    _sdata[i].dec->setStreamIndex(i);
    _sdata[i].start_dts = fis.getStreamInfo(i)->getFirstDts();
    _sdata[i].enc = new Encoder();
    _sdata[i].more_frames = true;
    if (_sdata[i].dec->getCodecType() == AVMEDIA_TYPE_VIDEO) {
      _sdata[i].enc->setStreamIndex(0);
      video = true;
      _sdata[i].enc->setCodecId(CODEC_ID_MPEG4);
      //_sdata[i].enc->setCodecId(CODEC_ID_THEORA);
      _sdata[i].enc->setWidth(720);
      _sdata[i].enc->setHeight(576);
//      _sdata[i].enc->setWidth(320);
//      _sdata[i].enc->setHeight(240);
      _sdata[i].enc->setGopSize(200);
      _sdata[i].enc->setCodecOption("flags","+psnr");
      AVRational ar;
      ar.num = 1;
      ar.den = 30;
      _sdata[i].enc->setTimeBase(ar);
      _sdata[i].enc->setBitRate(1500000);
      // logdebug(_sdata[i].enc->toString());
    } else if (_sdata[i].dec->getCodecType() == AVMEDIA_TYPE_AUDIO) {
      audio = true;
      _sdata[i].enc->setStreamIndex(1);

      _sdata[i].enc->setCodecId(CODEC_ID_MP2);
      _sdata[i].enc->setBitRate(128000);
      _sdata[i].enc->setSampleRate(48000);
      _sdata[i].enc->setChannels(2);
      _sdata[i].enc->setChannelLayout(AV_CH_LAYOUT_STEREO);

      _sdata[i].enc->setSampleFormat(_sdata[i].dec->getSampleFormat());
      //          _sdata[i].enc->open();
    AVRational ar;
    ar.num = 1;
    ar.den = _sdata[i].dec->getSampleRate();
    _sdata[i].dec->setTimeBase(ar);

    }
    if (fos._fmt->flags & AVFMT_GLOBALHEADER)
      _sdata[i].enc->setFlag(CODEC_FLAG_GLOBAL_HEADER);


    _sdata[i].dec->open();
    _sdata[i].enc->open();
    _smap[i] = s++;
    //if (_sdata[i].dec->getCodecType() == AVMEDIA_TYPE_VIDEO)
    //if (_sdata[i].dec->getCodecType() == AVMEDIA_TYPE_VIDEO)
    _sdata[i].conv = new FrameConverter(_sdata[i].dec, _sdata[i].enc);

    pos.setEncoder(*_sdata[i].enc, _sdata[i].enc->getStreamIndex());
    _sdata[i].enc->setOutputStream(&pos);
    LOGDEBUG(_sdata[i].enc->toString());
    LOGDEBUG(_sdata[i].dec->toString());
  }

  pos.init();
  fos.dumpFormat();
  /*main loop to encode the packets*/
    Packet *packet;
  for (int i = 0; i < 1500 ; i++) {
    //reading a packet from the Stream
    if ((packet=pis.readPacket()) ==NULL )break; //when no more packets available(EOF) then it return <0
    boost::shared_ptr<Packet> p(packet);
    int idx=p->getStreamIndex();
    if (_sdata.count(idx) == 0)continue;
    //    p.setDts(p.getDts() - _sdata[p.getStreamIndex()].start_dts);
    //Decoding a Video or Audio Packet
    //LOGDEBUG("Packet:"<<p->toString());
    Frame * src_frame = _sdata[idx].dec->decode2(*p);

    if (!src_frame->isFinished()) {
      delete src_frame;
  
      continue;
    }
    //mapping input tp output stream
    src_frame->stream_index = _smap[idx];

    //Convert an Audio or Video Packet
    Frame * trg_frame = NULL;
    if (_sdata[idx].dec->getCodecType() == AVMEDIA_TYPE_VIDEO)
      trg_frame = new Frame(
        _sdata[idx].enc->getInputFormat().pixel_format,
        _sdata[idx].enc->getWidth(),
        _sdata[idx].enc->getHeight());
    if (_sdata[idx].dec->getCodecType() == AVMEDIA_TYPE_AUDIO)
      trg_frame = new Frame();

    if (_sdata[idx].conv) {
      _sdata[idx].conv->convert(*src_frame, *trg_frame);
    }else{
      *trg_frame=*src_frame;
    }
    int ret = _sdata[idx].enc->encode(*trg_frame);

    if(src_frame)
      delete src_frame;
    src_frame=NULL;
    if(trg_frame)
      delete trg_frame;
    trg_frame=NULL;
//    delete p;

  }
  map<int, StreamData>::iterator it = _sdata.begin();
  for (; it != _sdata.end(); it++) {
    StreamData s = (*it).second;
    while (s.more_frames) {
      if (s.enc->encode() <= 0) {
        s.more_frames = false;
      }
    }
  }
  //int data_size=0;

cleanup:
  pos.close();
  fos.close();
  map<int, StreamData>::iterator streams = _sdata.begin();
  for (; streams != _sdata.end(); streams++) {
    delete (*streams).second.enc;
    delete (*streams).second.dec;
    delete (*streams).second.conv;
  }
//  Log::close();
  return (EXIT_SUCCESS);
}

