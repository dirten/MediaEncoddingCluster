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
};

map<int, StreamData> _sdata;
map<int, int> _smap;
/*
 * 
 */
int main(int argc, char** argv) {

  /*open the fixed test File or the file from command line input*/
  std::string src;
  std::string trg;
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

  /*Create and open the input and output Codecs*/
  int c = fis.getStreamCount();
  int s=0;
  for (int i = 0; i < c; i++) {
    if (fis.getStreamInfo(i)->getCodecType() != CODEC_TYPE_VIDEO&&
        fis.getStreamInfo(i)->getCodecType() != CODEC_TYPE_AUDIO) continue;
    _sdata[i].dec = new Decoder(fis.getStreamInfo(i)->getCodec());
    _sdata[i].enc = new Encoder();
    if (_sdata[i].dec->getCodecType() == CODEC_TYPE_VIDEO) {
//      _sdata[i].enc->setCodecId(CODEC_ID_MPEG4);
//      _sdata[i].enc->setCodecId(CODEC_ID_MPEG2VIDEO);
      _sdata[i].enc->setCodecId(CODEC_ID_H264);
      _sdata[i].enc->setBitRate(1024000);
      _sdata[i].enc->setWidth(320);
      _sdata[i].enc->setHeight(240);
      _sdata[i].enc->setGopSize(12);
      AVRational ar;
      ar.num = 1;
      ar.den = 25;
      _sdata[i].enc->setTimeBase(ar);
    } else if (_sdata[i].dec->getCodecType() == CODEC_TYPE_AUDIO) {
      _sdata[i].enc->setCodecId(CODEC_ID_MP3);
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

    _sdata[i].dec->open();
    _sdata[i].enc->open();
    _smap[i]=s++;
    _sdata[i].conv = new FrameConverter(_sdata[i].dec, _sdata[i].enc);
    pos.setEncoder(*_sdata[i].enc, _smap[i]);
    _sdata[i].enc->setOutputStream(&pos);
  }

  if (!pos.init())goto cleanup;
  fos.dumpFormat();
  /*main loop to encode the packets*/
  for (int i = 0; i < 350; i++) {
    Packet p;
    //reading a packet from the Stream
    if (pis.readPacket(p) < 0)break; //when no more packets available then it return <0
    if(_sdata.find(p.getStreamIndex())==_sdata.end())continue;
    //Decoding a Video or Audio Packet
    Frame * src_frame = _sdata[p.getStreamIndex()].dec->decode2(p);
    if(!src_frame->isFinished())continue;
    src_frame->stream_index=_smap[p.getStreamIndex()];

    //Convert an Audio or Video Packet
    Frame * trg_frame = NULL;
    if (_sdata[p.getStreamIndex()].dec->getCodecType() == CODEC_TYPE_VIDEO)
      trg_frame = new Frame(
        _sdata[p.getStreamIndex()].enc->getPixelFormat(),
        _sdata[p.getStreamIndex()].enc->getWidth(),
        _sdata[p.getStreamIndex()].enc->getHeight());
    if (_sdata[p.getStreamIndex()].dec->getCodecType() == CODEC_TYPE_AUDIO)
      trg_frame = new Frame();
    _sdata[p.getStreamIndex()].conv->convert(*src_frame, *trg_frame);
    int ret = _sdata[p.getStreamIndex()].enc->encode(*trg_frame);


    delete src_frame;
    delete trg_frame;

  }


cleanup:
  fos.close();
  map<int, StreamData>::iterator streams = _sdata.begin();
  for (; streams != _sdata.end(); streams++) {
    delete (*streams).second.enc;
    delete (*streams).second.dec;
    delete (*streams).second.conv;
  }
  Log::close();
  return (EXIT_SUCCESS);
}

