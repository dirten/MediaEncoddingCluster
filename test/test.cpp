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


#include "org/esb/io/FileInputStream.h"
#include "org/esb/io/FileOutputStream.h"


using namespace org::esb::io;
using namespace org::esb::av;

using namespace std;

int audio() {
  //
  //  File infile("/media/video/ChocolateFactory.ts");
//  File infile("/media/disk/big_buck_bunny_1080p_surround.avi");
  File infile("/Users/jholscher/media/ChocolateFactory.ts");
  File outfile("/media/out/test.mp4");
  if (!infile.exists()) {
    logdebug("file does not exit");
    //    return 127;
  }
  FormatInputStream fis(&infile);
  PacketInputStream pis(&fis);

  FormatOutputStream fos(&outfile);
  PacketOutputStream pos(&fos);

  AVCodecContext * c = fis.getFormatContext()->streams[1]->codec;
  Decoder dec(c->codec_id);
  dec.setChannels(c->channels);
  dec.setBitRate(c->bit_rate);
  dec.setSampleRate(c->sample_rate);
  //  dec.setHeight(576);


  //  dec.setTimeBase((AVRational) {1, 25});
  //  dec.setBitRate(4000000);
  //  dec.setGopSize(12);
  //  	dec.setPixelFormat (PIX_FMT_YUV420P);
  dec.ctx->request_channel_layout = 2;
  dec.open();

  Encoder enc(CODEC_ID_MP2);
  enc.setChannels(2);
  enc.setBitRate(192000);
  enc.setSampleRate(48000);
  enc.setSampleFormat(dec.getSampleFormat());
  enc.setTimeBase((AVRational) {
    1, 25
  });

  //  enc.setGopSize(25);
  //  enc.setPixelFormat(PIX_FMT_YUV420P);
  enc.open();
  logdebug("Decoder:" << dec.toString());
  logdebug("Encoder:" << enc.toString());

  pos.setEncoder(enc);
  pos.init();


  FrameConverter conv(&dec, &enc);
  for (int i = 0; i < 5000; i++) {
    Packet p;
    pis.readPacket(p);
    if (p.getStreamIndex() != 1)continue;
//    logdebug("InputPacket:");
//    p.toString();
    Frame f = dec.decode(p);
    //    f.toString();
    if (f.isFinished()) {
      Frame f2 = conv.convert(f);
      f2.setPts(f2.getDts());
      Packet pt = enc.encode(f2);
      pt.setPts(0);
      pt.setDts(0);
      pt.setStreamIndex(0);
//      logdebug("OutputPacket:");
//      pt.toString();
      pos.writePacket(pt);
    }
  }

  //        Frame f((PixelFormat)1,320,240);

  return 0;

}

int video() {

  File infile("/Users/jholscher/media/ChocolateFactory.ts");
  File outfile("/media/out/ChocolateFactory.mp4");
  if (!infile.exists()) {
    logdebug("file does not exit");
    return 127;
  }

  FormatInputStream fis(&infile);
  PacketInputStream pis(&fis);

  FormatOutputStream fos(&outfile);
  PacketOutputStream pos(&fos);

  logdebug("oformat_name:" << fos._fmt->name);
  Decoder dec(CODEC_ID_MPEG2VIDEO);
  dec.setWidth(720);
  dec.setHeight(576);

  dec.setTimeBase((AVRational) {
    1, 25
  });
  dec.setBitRate(4000000);
  dec.setGopSize(12);
  //  	dec.setPixelFormat (PIX_FMT_YUV420P);
  dec.open();

  Encoder enc(CODEC_ID_H264);
  enc.setWidth(320);
  enc.setHeight(240);

  enc.setTimeBase((AVRational) {
    1, 25
  });
  enc.setBitRate(1024000);
  enc.setGopSize(25);
  enc.ctx->max_b_frames = 0;
  //  enc.setPixelFormat(PIX_FMT_YUV420P);
  //  enc.setFlag(CODEC_FLAG_GLOBAL_HEADER);
//  logdebug("Flags:" << CODEC_FLAG_GLOBAL_HEADER)
  enc.setFlag(4194304);
  //  enc.ctx->codec_tag=MKTAG('m','p','4','v');
  enc.open();
  logdebug("Decoder:" << dec.toString());
  logdebug("Encoder:" << enc.toString());

  pos.setEncoder(enc);
  pos.init();
  FrameConverter conv(&dec, &enc);
  for (int i = 0; i < 5000; i++) {
    Packet p;
    pis.readPacket(p);
    if (p.getStreamIndex() != 0)continue;
    //    p.toString();

    Frame f = dec.decode(p);
    //    f.toString();
    if (f.isFinished()) {
      Frame f2 = conv.convert(f);
      f2.setPts(f2.getDts());
      Packet pt = enc.encode(f2);
      pt.setPts(0);
      pt.setDts(0);
      pos.writePacket(pt);
    }
  }

  //        Frame f((PixelFormat)1,320,240);

  return 0;
}

int av() {

  File infile("/Users/jholscher/media/ChocolateFactory.ts");
  File outfile("/media/out/ChocolateFactory.mp4");
  if (!infile.exists()) {
    logdebug("file does not exit");
    return 127;
  }

  FormatInputStream fis(&infile);
  PacketInputStream pis(&fis);

  FormatOutputStream fos(&outfile);
  PacketOutputStream pos(&fos);

  AVCodecContext * c = fis.getFormatContext()->streams[1]->codec;

  logdebug("oformat_name:" << fos._fmt->name);
  Decoder dec(CODEC_ID_MPEG2VIDEO);
  dec.setWidth(720);
  dec.setHeight(576);

  dec.setTimeBase((AVRational) {
    1, 25
  });
  dec.setBitRate(4000000);
  dec.setGopSize(12);
  //  	dec.setPixelFormat (PIX_FMT_YUV420P);
  dec.open();

  Encoder enc(CODEC_ID_H264);
  enc.setWidth(320);
  enc.setHeight(240);

  enc.setTimeBase((AVRational) {
    1, 25
  });
  enc.setBitRate(1024000);
  enc.setGopSize(25);
  enc.ctx->max_b_frames = 0;
  //  enc.setPixelFormat(PIX_FMT_YUV420P);
  //  enc.setFlag(CODEC_FLAG_GLOBAL_HEADER);
  logdebug("Flags:" << CODEC_FLAG_GLOBAL_HEADER)
  enc.setFlag(4194304);
  //  enc.ctx->codec_tag=MKTAG('m','p','4','v');
  enc.open();
  logdebug("Decoder:" << dec.toString());
  logdebug("Encoder:" << enc.toString());




  Decoder adec(c->codec_id);
  adec.setChannels(c->channels);
  adec.setBitRate(c->bit_rate);
  adec.setSampleRate(c->sample_rate);
  //  dec.setHeight(576);


  //  dec.setTimeBase((AVRational) {1, 25});
  //  dec.setBitRate(4000000);
  //  dec.setGopSize(12);
  //  	dec.setPixelFormat (PIX_FMT_YUV420P);
  adec.ctx->request_channel_layout = 2;
  adec.open();

  Encoder aenc(CODEC_ID_MP3);
  aenc.setChannels(2);
  aenc.setBitRate(192000);
  aenc.setSampleRate(48000);
  aenc.setSampleFormat(dec.getSampleFormat());
  //  enc.setGopSize(25);
  //  enc.setPixelFormat(PIX_FMT_YUV420P);
  aenc.open();




  pos.setEncoder(enc);
  pos.setEncoder(aenc);
  pos.init();
  FrameConverter conv(&dec, &enc);
  FrameConverter aconv(&adec, &aenc);
  for (int i = 0; i < 5000; i++) {
    Packet p;
    pis.readPacket(p);
    if (p.getStreamIndex() == 0) {
      Frame f = dec.decode(p);
      if (f.isFinished()) {
        Frame f2 = conv.convert(f);
        f2.setPts(f2.getDts());
        Packet pt = enc.encode(f2);
        pt.setPts(0);
        pt.setDts(0);
        pos.writePacket(pt);
      }
    } else
      if (p.getStreamIndex() == 1) {
      Frame f = adec.decode(p);
      if (f.isFinished()) {
        Frame f2 = aconv.convert(f);
        f2.setPts(f2.getDts());
        Packet pt = aenc.encode(f2);
        pt.setPts(0);
        pt.setDts(0);
        pos.writePacket(pt);
      }
    }
  }
}

int main() {
    audio();
//    video();
//  av();
}
