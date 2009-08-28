/*
 * File:   TestAudioAC3toMP2.cpp
 * Author: jhoelscher
 *
 * Created on 15. Juni 2009, 18:31
 */

#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/FormatOutputStream.h"
#include "org/esb/av/PacketOutputStream.h"
#include "org/esb/av/Packet.h"
#include "org/esb/av/Encoder.h"
#include "org/esb/av/Decoder.h"
#include "org/esb/av/Frame.h"
#include "org/esb/av/FrameConverter.h"
#include "org/esb/util/Decimal.h"


#include "org/esb/io/FileInputStream.h"
#include "org/esb/io/FileOutputStream.h"

#include <list>
using namespace org::esb::io;
using namespace org::esb::av;
using namespace org::esb::util;

class PacketSink : public Sink {
public:

  PacketSink() {
  }

  void write(void * p) {

    logdebug("Write Packet to Term Sink");
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

int main() {
  //  int stream_id = 2;
  //File infile("/media/video/ChocolateFactory.ts");
  File infile("/media/disk/video/big_buck_bunny_1080p_surround.avi");
  int stream_id = 1;
  File outfile("/media/out/test.ogg");

  FormatInputStream fis(&infile);
  PacketInputStream pis(&fis);

  FormatOutputStream fos(&outfile);
  PacketOutputStream pos(&fos);

  AVCodecContext * c = fis.getFormatContext()->streams[stream_id]->codec;
  Decoder dec(c->codec_id);
  dec.setChannels(c->channels);
  dec.setBitRate(c->bit_rate);
  dec.setSampleRate(c->sample_rate);
  dec.setPixelFormat(c->pix_fmt);
  dec.setPixelFormat(PIX_FMT_YUV420P);
  dec.setTimeBase(c->time_base);
  dec.ctx->request_channel_layout = 2;
  dec.open();
  logdebug(dec.toString());

  Encoder enc(CODEC_ID_VORBIS);
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
  logdebug(enc.toString());

  pos.setEncoder(enc, 0);
  pos.init();

  FrameConverter conv(&dec, &enc);
  PacketSink sink;
  enc.setSink(&sink);

  Packet p;
  for (int a = 0; a < 10000; a++) {
    pis.readPacket(p);
    if (p.getStreamIndex() == stream_id) {
      p.setStreamIndex(0);
      Frame * tmp = dec.decode2(p);
      if (!tmp->isFinished()) {
//        continue;
      }
      Frame * f = new Frame();
      conv.convert(*tmp, *f);
      delete tmp;
      Packet ret = enc.encode(*f);
      delete f;

    }
  }
  list< boost::shared_ptr<Packet> > encoded = sink.getList();
  list< boost::shared_ptr<Packet> >::iterator it = encoded.begin();
  for (; it != encoded.end(); it++) {
    pos.writePacket(*(*it).get());
  }

}
