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
#include "org/esb/lang/StackDumper.h"
#include <list>
using namespace org::esb::io;
using namespace org::esb::av;
using namespace org::esb::util;
using namespace org::esb::lang;

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

int main(int argc, char ** argv) {
  StackDumper stack(".");
  //  int stream_id = 2;
  //  File infile("/media/video/ChocolateFactory.ts");
  //  File infile("/home/jhoelscher/MediaEncodingCluster/big_buck_bunny_480p_surround-fix.avi");
  std::string src;

  if (argc == 1) {
    src = MEC_SOURCE_DIR;
    src.append("/test.dvd");
  } else {
    src = argv[1];
  }
  int stream_id = 0;
  if (argc == 3) {
    stream_id = atoi(argv[2]);
  }

  File infile(src);
//  int stream_id = atoi(argv[2]);
  File outfile("test.mp3");

  FormatInputStream fis(&infile);
  fis.dumpFormat();
  PacketInputStream pis(&fis);

  FormatOutputStream fos(&outfile);
  PacketOutputStream pos(&fos);

  AVCodecContext * c = fis.getFormatContext()->streams[stream_id]->codec;
  Decoder dec(c->codec_id); //CODEC_ID_EAC3
  logdebug("ChannelLayout:" << fis.getFormatContext()->streams[stream_id]->codec->channel_layout)
  //    Decoder dec(c);


  dec.setChannels(c->channels);
  //  dec.setChannels(2);
  dec.setBitRate(c->bit_rate);
  dec.setSampleRate(c->sample_rate);
  dec.setSampleFormat(c->sample_fmt);
  dec.setTimeBase(c->time_base);


  dec.ctx->request_channels = 2;
  //  dec.ctx->request_channel_layout = 2;
  dec.open();



  logdebug(dec.toString());

  //  Encoder enc(CODEC_ID_VORBIS);
  Encoder enc(CODEC_ID_MP3);
  enc.setChannels(2);
  enc.setBitRate(128000);
  enc.setSampleRate(48000);
  enc.setSampleFormat(dec.getSampleFormat());
  AVRational ar;
  ar.num = 1;
  ar.den = enc.getSampleRate();
  enc.setTimeBase(ar);
  //  enc.setFlag(CODEC_FLAG_GLOBAL_HEADER);
  //  enc.setPixelFormat(PIX_FMT_YUV420P);
  //  enc.ctx->bits_per_raw_sample=dec.ctx
  enc.open();
  logdebug(enc.toString());
  logdebug("Encoder Frame Size:" << enc.ctx->frame_size);


  pos.setEncoder(enc, 0);
  pos.init();

  FrameConverter conv(&dec, &enc);
  PacketSink sink;
  enc.setSink(&sink);
  //	enc.setOutputStream(&pos);
  Packet p;
  for (int a = 0; a < 10;) {

    pis.readPacket(p);
    if (p.getStreamIndex() == stream_id) {
      a++;
      p.setStreamIndex(0);
      p.toString();
      Frame * tmp = dec.decode2(p);
      tmp->toString();

      //      continue;
      if (!tmp->isFinished()) {
        //        continue;
      }
      Frame * f = new Frame();
      conv.convert(*tmp, *f);
      f->toString();
      //	  pos.writePacket(p);
      enc.encode(*f);
      delete tmp;
      delete f;

    }
  }
  list< boost::shared_ptr<Packet> > encoded = sink.getList();
  list< boost::shared_ptr<Packet> >::iterator it = encoded.begin();
  for (; it != encoded.end(); it++) {
    pos.writePacket(*(*it).get());
  }
  Log::close();
}
