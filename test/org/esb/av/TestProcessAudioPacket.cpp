#include "org/esb/av/Packet.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/io/ObjectInputStream.h"

#include "org/esb/av/Decoder.h"
#include "org/esb/av/Encoder.h"
#include "org/esb/av/Frame.h"
#include "org/esb/av/FrameConverter.h"

using namespace org::esb::av;
using namespace org::esb::io;

int main() {
  return 0;
    av_register_all();
    avcodec_init();
    avcodec_register_all();
  char * filename = new char[100];

  int start = 1043;
  Decoder dec((CodecID) 86016);
  dec.setChannels(2);
  dec.setBitRate(192000);
  dec.setSampleRate(48000);
  dec.setSampleFormat((SampleFormat) 1);

  dec.setTimeBase((AVRational) {
    1, 48000
  });
  dec.ctx->request_channel_layout = 2;
  dec.open();

  Encoder enc(CODEC_ID_MP3);
  enc.setChannels(2);
  enc.setBitRate(128000);
  enc.setSampleRate(48000);
  enc.setSampleFormat(dec.getSampleFormat());

  enc.setTimeBase((AVRational) {
    1, 48000
  });
  enc.setFlag(CODEC_FLAG_GLOBAL_HEADER);
  enc.setPixelFormat(PIX_FMT_YUV420P);
  enc.open();
  FrameConverter conv(&dec, &enc);

  for (int a = 0; a < 6; a++) {
    sprintf(filename, "../../../../../test_data/packet-2-%d.pkt", start++);
    FileInputStream fis(filename);
    ObjectInputStream ois(&fis);
    Packet p;
    ois.readObject(p);
    p.toString();
    Frame * f=dec.decode2(p);
    Frame f2;
    conv.convert(*f, f2);
    enc.encode(f2);
  }
}
