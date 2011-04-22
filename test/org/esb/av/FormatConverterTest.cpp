/* 
 * File:   FormatConverterTest.cpp
 * Author: jhoelscher
 *
 * Created on 26. Januar 2010, 09:00
 */

#include <stdlib.h>
#include <list>
#include "org/esb/io/File.h"
#include "org/esb/av/Packet.h"
#include "org/esb/av/Frame.h"
#include "org/esb/av/PGMUtil.h"
#include "org/esb/av/Decoder.h"
#include "org/esb/av/Encoder.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/FrameConverter.h"
#include "org/esb/util/Log.h"


#include "org/esb/util/ScopedTimeCounter.h"

using namespace org::esb::av;
int main(){
  {
  org::esb::util::ScopedTimeCounter runtimetest("runtimetest");
//  Log::open("");
  std::string filepath = MEC_SOURCE_DIR;
  filepath.append("/test.dvd");
  org::esb::io::File file(filepath.c_str());
  org::esb::av::FormatInputStream fis(&file);
  int c = fis.getStreamCount();
  int video_stream = -1;
  for (int a = 0; a < c; a++) {
    if (fis.getStreamInfo(a)->getCodecType() == AVMEDIA_TYPE_VIDEO) {
      video_stream = a;
      break;
    }
  }
  org::esb::av::Decoder dec(fis.getAVStream(video_stream));
  //  dec.setTimeBase(1, 25);
  dec.open();
  LOGDEBUG(dec.toString());


Encoder enc(CODEC_ID_MPEG4);
  enc.setCodecId(CODEC_ID_H264);
  enc.setWidth(dec.getWidth());
  enc.setHeight(dec.getHeight());
  enc.setGopSize(200);
  enc.setTimeBase(1,25);
  enc.setFrameRate(25,1);
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
  enc.setBitRate(1500000);
  enc.open();
std::cout << enc.toString()<<std::endl;
enc.setPixelFormat(PIX_FMT_RGB24);
std::cout << enc.toString()<<std::endl;
  org::esb::av::PacketInputStream pis(&fis);
  org::esb::av::Packet * p;
  FrameConverter conv(&dec,&enc);
  Frame out_frame(enc.getPixelFormat(), enc.getWidth(), enc.getHeight());
  for (int i = 0; i < 4500;) {
    if ((p = pis.readPacket()) != NULL){
      if(p->getStreamIndex() == video_stream) {
        org::esb::av::Frame * frame = dec.decode2(*p);
        if (frame->isFinished()) {
          conv.convert(*frame, out_frame);
         // LOGDEBUG("frame finished!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
//          frame->dumpHex();
        }
        delete frame;
      }
      i++;
    }else{
      break;
    }
    delete p;
  }
  }
}