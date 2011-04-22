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
#include "org/esb/av/PixelFormatConverter.h"
#include "org/esb/av/ResizeFilter.h"
#include "org/esb/av/Frame.h"

#include "org/esb/av/PGMUtil.h"
#include "org/esb/util/Log.h"

#include <boost/shared_ptr.hpp>
#include <list>
/*
 * 
 */
int main(int argc, char** argv) {
  Log::open("");


  double t=2.1;
  AVRational ar1;
  ar1.num=1;
  ar1.den=90000;
  AVRational ar2;
  ar2.num=1;
  ar2.den=30;
  int frames=24*3600;
  int base=((int)frames*av_q2d(ar1)/av_q2d(ar2));
  double a=frames*av_q2d(ar1)/av_q2d(ar2)-base;
  double b=30/av_q2d(ar2);
  LOGDEBUG("Double val "<<t-(int)t);
  LOGDEBUG("a val "<<a);
  LOGDEBUG("b val "<<b);
  
  std::string path=MEC_SOURCE_DIR;
  org::esb::io::File file(path+"/test/images/number-%d.pgm");
  org::esb::av::FormatInputStream fis(&file);
  org::esb::av::PacketInputStream pis(&fis);


  org::esb::io::File ofile(path+"/test/images/number.avi");
  org::esb::av::FormatOutputStream fos(&ofile);
  org::esb::av::PacketOutputStream pos(&fos);

  org::esb::av::Encoder enc(CODEC_ID_MPEG4);

  enc.setCodecId(CODEC_ID_H264);
  enc.setWidth(160);
  enc.setHeight(120);
  enc.setGopSize(200);
//  enc.setTimeBase(1000,23976);
  enc.setTimeBase(1,10);
  enc.setCodecOption("coder","1");
  enc.setCodecOption("flags","+loop");
  enc.setCodecOption("cmp","+chroma");
  enc.setCodecOption("partitions","+parti8x8+parti4x4+partp8x8+partb8x8");
  enc.setCodecOption("me_method","umh");
  enc.setCodecOption("subq","8");
  enc.setCodecOption("me_range","16");
  enc.setCodecOption("g","250");
  enc.setCodecOption("keyint_min","25");
  enc.setCodecOption("sc_threshold","40");
  enc.setCodecOption("i_qfactor","0.71");
  enc.setCodecOption("b_strategy","2");
  enc.setCodecOption("qcomp","0.6");
  enc.setCodecOption("qmin","10");
  enc.setCodecOption("qmax","51");
  enc.setCodecOption("qdiff","4");
  enc.setCodecOption("bf","3");
  enc.setCodecOption("refs","4");
  enc.setCodecOption("directpred","3");
  enc.setCodecOption("trellis","1");
  enc.setCodecOption("flags2","+wpred+mixed_refs+dct8x8+fastpskip+mbtree");
  enc.setCodecOption("wpredp","2");


//  enc.setTimeBase(100,2997);
  enc.setBitRate(1500000);
  enc.open();
  LOGDEBUG("encoder ready")

  pos.setEncoder(enc);
  LOGDEBUG("pos setencoder ready")
  pos.init();
  LOGDEBUG("pos ready")
  enc.setOutputStream(&pos);

  org::esb::av::Decoder dec(fis.getAVStream(0));
  dec.setTimeBase(1,10);
  dec.open();
  LOGDEBUG("dec ready")


//  org::esb::av::PixelFormatConverter conv(dec.getOutputFormat(),enc.getInputFormat());
  org::esb::av::ResizeFilter resizer(dec.getOutputFormat(),enc.getInputFormat());
  resizer.open();
  LOGDEBUG("resizer ready")
//  conv.setFrameRateCompensateBase(-0.6);
  org::esb::av::Packet * p;
  std::list<boost::shared_ptr<org::esb::av::Packet> > packet_list;
  LOGDEBUG("reading Packet")
  while((p=pis.readPacket())!=NULL){
    p->setTimeBase(dec.getTimeBase());
    boost::shared_ptr<org::esb::av::Packet> packet(p);
    packet_list.push_back(packet);
  }
  LOGDEBUG("reading Packet finished")
  for(int a=0;a<1;a++){
    std::list<boost::shared_ptr<org::esb::av::Packet> >::iterator it=packet_list.begin();
    for(;it!=packet_list.end();it++){
    org::esb::av::Frame * f=dec.decode2(*(*it).get());
    if(!f->isFinished())continue;
    org::esb::av::Frame out(enc.getInputFormat().pixel_format,enc.getWidth(),enc.getHeight());
    resizer.process(*f, out);
    enc.encode(out);
//    delete p;
    delete f;
    }
  }
  LOGTRACE("Encode Packet delay");
  bool have_more_frames=enc.getCodecType()==AVMEDIA_TYPE_VIDEO;
  while(have_more_frames){
    if(enc.encode()<=0){
      have_more_frames=false;
    }
  }
  LOGTRACE("delay encoded");
  resizer.close();
  pos.close();
  fos.close();
  Log::close();
  return (EXIT_SUCCESS);
}

