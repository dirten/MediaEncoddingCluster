/* 
 * File:   TestPass1.cpp
 * Author: jhoelscher
 *
 * Created on 3. November 2009, 14:06
 */

#include <stdlib.h>

#include "org/esb/io/File.h"
#include "org/esb/io/FileOutputStream.h"

#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/Frame.h"
#include "org/esb/av/Decoder.h"
#include "org/esb/av/Encoder.h"
#include "org/esb/av/FrameConverter.h"


using namespace org::esb::av;
using namespace org::esb::io;

/*
 * 
 */
int main(int argc, char** argv) {

  /*open the fixed test File or the file from command line input*/
  std::string src;
  if (argc == 1) {
    src = MEC_SOURCE_DIR;
    src.append("/test.dvd");
  } else {
    src = argv[1];
  }


  /*opening the input file and Packet Input Stream*/
  File f(src.c_str());
  FormatInputStream fis(&f);
  PacketInputStream pis(&fis);

  Decoder * dec;
  Encoder * enc;
  FrameConverter * conv;
  /*Create and open the input and output Codecs*/
  int c = fis.getStreamCount();
  int vidx = 0;
  for (int i = 0; i < c; i++) {
    if (fis.getStreamInfo(i)->getCodecType() != CODEC_TYPE_VIDEO) continue;
    vidx = i; //fis.getStreamInfo(i)->getStreamIndex();
    dec = new Decoder(fis.getAVStream(i));
    dec->open();
    enc = new Encoder();
    //enc->setCodecId(CODEC_ID_MPEG4);

    enc->setCodecId(CODEC_ID_H264);
    enc->setBitRate(1024000);
    enc->setWidth(320);
    enc->setHeight(240);
    enc->setGopSize(12);
    enc->setPixelFormat(dec->getPixelFormat());
    AVRational ar;
    ar.num = 1;
    ar.den = 25;
    enc->setTimeBase(ar);
    enc->setFlag(CODEC_FLAG_PASS1);
    enc->open();
    conv = new FrameConverter(dec, enc);

  }

  /*stringbuffer for the statistics*/
  std::string stats;

  FileOutputStream fos("stats.out");
  /*main loop to encode the packets*/
  for (int i = 0; i < 7000; i++) {
    Packet p;
    //reading a packet from the Stream
    if (pis.readPacket(p) < 0)break; //when no more packets available(EOF) then it return <0
    //its not a video packet
    if (p.getStreamIndex() != vidx)continue;

    //Decoding a Video Packet
    Frame * src_frame = dec->decode2(p);

    if (!src_frame->isFinished())continue;


    //Convert an Audio or Video Packet
    Frame * trg_frame = new Frame(
        enc->getPixelFormat(),
        enc->getWidth(),
        enc->getHeight());


    conv->convert(*src_frame, *trg_frame);
    int ret = enc->encode(*trg_frame);
    stats.append(enc->getStatistics());

    delete src_frame;
    delete trg_frame;

  }
  fos.write(stats);
  return (EXIT_SUCCESS);
}

