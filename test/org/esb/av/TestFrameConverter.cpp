/*
 * TestFrameConverter.cpp
 *
 *  Created on: 09.03.2010
 *      Author: jhoelscher
 */

#include "TestFrameConverter.h"

#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/FrameConverter.h"
#include "org/esb/av/Decoder.h"
#include "org/esb/av/Encoder.h"
#include "org/esb/av/Frame.h"

#include "org/esb/util/Log.h"
#include <iostream>

TestFrameConverter::TestFrameConverter() {
  // TODO Auto-generated constructor stub

}

TestFrameConverter::~TestFrameConverter() {
  // TODO Auto-generated destructor stub
}

using namespace org::esb::av;
using namespace org::esb::io;

void test_swscale(Decoder & dec, Encoder & enc) {

  FrameConverter conv(&dec,&enc);

  Frame in_frame(dec.getPixelFormat(), dec.getWidth(), dec.getHeight());
  in_frame.setTimeBase(dec.getTimeBase());
  in_frame.setDts(1);
  in_frame.setPts(1);

  Frame out_frame(enc.getPixelFormat(), enc.getWidth(), enc.getHeight());

  conv.convert(in_frame, out_frame);

}
void test_deinterlace(Decoder & dec, Encoder & enc) {

  FrameConverter conv(&dec,&enc);

  Frame in_frame(dec.getPixelFormat(), dec.getWidth(), dec.getHeight());
  in_frame.setDts(1);
  in_frame.setPts(1);
  in_frame.setTimeBase(dec.getTimeBase());


  Frame out_frame(enc.getPixelFormat(), enc.getWidth(), enc.getHeight());
  conv.setDeinterlace(true);
  conv.convert(in_frame, out_frame);

}

int main(int argc, char ** argv) {
  if (argc == 1) {
    std::cout << "no filename given" << std::endl;
    exit(1);
  }
  Log::open("");
  std::string filename = argv[1];

  File file(filename);
  FormatInputStream fis(&file);
  PacketInputStream pis(&fis);

  /**
   * retriving the first video stream
   */
  int c = fis.getStreamCount();
  int video_stream = -1;
  for (int a = 0; a < c; a++) {
    if (fis.getStreamInfo(a)->getCodecType() == CODEC_TYPE_VIDEO) {
      video_stream = a;
      break;
    }
  }
  Decoder dec(fis.getAVStream(video_stream));
  dec.open();

  std::cout << dec.toString()<<std::endl;
  Encoder enc(CODEC_ID_MPEG4);
  enc.setCodecId(CODEC_ID_H264);
  enc.setWidth(160);
  enc.setHeight(120);
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


  test_swscale(dec, enc);
  test_deinterlace(dec, enc);

  return 0;
}
