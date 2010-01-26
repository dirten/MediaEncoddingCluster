/* 
 * File:   TestEncoder.cpp
 * Author: jhoelscher
 *
 * Created on 26. Januar 2010, 10:31
 */

#include <stdlib.h>

#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/FormatOutputStream.h"
#include "org/esb/av/PacketOutputStream.h"
#include "org/esb/av/Encoder.h"
#include "org/esb/av/Decoder.h"
#include "org/esb/av/FrameConverter.h"
#include "org/esb/av/Frame.h"

#include "org/esb/av/PGMUtil.h"
#include "org/esb/util/Log.h"
/*
 * 
 */
int main(int argc, char** argv) {
  Log::open("");
  std::string path=MEC_SOURCE_DIR;
  org::esb::io::File file(path+"/test/images/number-%d.pgm");
  org::esb::av::FormatInputStream fis(&file);
  org::esb::av::PacketInputStream pis(&fis);


  org::esb::io::File ofile(path+"/test/images/number.avi");
  org::esb::av::FormatOutputStream fos(&ofile);
  org::esb::av::PacketOutputStream pos(&fos);

  org::esb::av::Encoder enc(CODEC_ID_MPEG4);

  enc.setCodecId(CODEC_ID_MPEG4);
  enc.setWidth(720);
  enc.setHeight(576);
//      _sdata[i].enc->setWidth(320);
//      _sdata[i].enc->setHeight(240);
  enc.setGopSize(200);
  AVRational ar;
  ar.num = 1;
  ar.den = 10;
  enc.setTimeBase(ar);
  enc.setBitRate(1500000);
  enc.open();
  pos.setEncoder(enc);
  pos.init();
  enc.setOutputStream(&pos);
  org::esb::av::Decoder dec(fis.getAVStream(0)->codec);
  ar.den = 1;
  dec.setTimeBase(ar);
  dec.open();
  org::esb::av::FrameConverter conv(&dec,&enc);
  org::esb::av::Packet * p;
  while((p=pis.readPacket())!=NULL){
    p->setTimeBase(ar);
    org::esb::av::Frame * f=dec.decode2(*p);

    if(!f->isFinished())continue;
    org::esb::av::Frame out(enc.getPixelFormat(),enc.getWidth(),enc.getHeight());
    conv.convert(*f, out);
    enc.encode(out);
    delete p;
    delete f;
  }
  Log::close();
  return (EXIT_SUCCESS);
}

