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

int audio(){
  File infile("/media/video/ChocolateFactory.ts");
  File outfile("/media/out/ChocolateFactory.ogg");
  if (!infile.exists()) {
    logdebug("file does not exit");
    return 127;
  }
  FormatInputStream fis(&infile);
  PacketInputStream pis(&fis);

  FormatOutputStream fos(&outfile);
  PacketOutputStream pos(&fos);


  Decoder dec(CODEC_ID_MP3);
  dec.setChannels(2);
  dec.setBitRate(192000);
  dec.setSampleRate(48000);

//  dec.setHeight(576);
  

//  dec.setTimeBase((AVRational) {1, 25});
//  dec.setBitRate(4000000);
//  dec.setGopSize(12);
  //  	dec.setPixelFormat (PIX_FMT_YUV420P);
  dec.open();

  Encoder enc(CODEC_ID_VORBIS);
  enc.setChannels(2);
  enc.setBitRate(128000);
  enc.setSampleRate(44100);
  enc.setSampleFormat(dec.getSampleFormat());
//  enc.setGopSize(25);
//  enc.setPixelFormat(PIX_FMT_YUV420P);
  enc.open();
  logdebug("Decoder:"<<dec.toString());
  logdebug("Encoder:"<<enc.toString());

  pos.setEncoder(enc);
  pos.init();


  FrameConverter conv(&dec, &enc);
  for (int i = 0; i < 100; i++) {
    Packet p;
    pis.readPacket(p);
    if (p.getStreamIndex() != 1)continue;
    p.toString();
    Frame f = dec.decode(p);
    f.toString();
    if (f.isFinished()) {
      Frame f2=conv.convert(f);
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

int video() {

  File infile("/media/video/ChocolateFactory.ts");
  File outfile("/media/out/ChocolateFactory.avi");
  if (!infile.exists()) {
    logdebug("file does not exit");
    return 127;
  }
  FormatInputStream fis(&infile);
  PacketInputStream pis(&fis);

  FormatOutputStream fos(&outfile);
  PacketOutputStream pos(&fos);


  Decoder dec(CODEC_ID_MPEG2VIDEO);
  dec.setWidth(720);
  dec.setHeight(576);

  dec.setTimeBase((AVRational) {
    1, 25});
  dec.setBitRate(4000000);
  dec.setGopSize(12);
  //  	dec.setPixelFormat (PIX_FMT_YUV420P);
  dec.open();

  Encoder enc(CODEC_ID_MSMPEG4V3);
  enc.setWidth(320);
  enc.setHeight(240);

  enc.setTimeBase((AVRational) {
    1, 25});
  enc.setBitRate(4000000);
  enc.setGopSize(25);
  enc.setPixelFormat(PIX_FMT_YUV420P);
  enc.open();
  logdebug("Decoder:"<<dec.toString());
  logdebug("Encoder:"<<enc.toString());

  pos.setEncoder(enc);
  pos.init();
  FrameConverter conv(&dec, &enc);
  for (int i = 0; i < 100; i++) {
    Packet p;
    pis.readPacket(p);
    if (p.getStreamIndex() != 0)continue;
    p.toString();
    Frame f = dec.decode(p);
//    f.toString();
    if (f.isFinished()) {
      Frame f2=conv.convert(f);
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

int main(){
  audio();
//  video();
  
}
