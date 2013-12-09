#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/FormatOutputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/PacketOutputStream.h"
#include "org/esb/av/Decoder.h"
#include "org/esb/av/Encoder.h"
#include "org/esb/io/File.h"

using namespace org::esb::av;
using namespace org::esb::io;


int main(){

  int stream_id=0;


  std::string src = MEC_SOURCE_DIR;
        src.append("/test-data/test.dvd");
  File f(src.c_str());
  FormatInputStream fis(&f);
  PacketInputStream pis(&fis);

std::string trg = MEC_SOURCE_DIR;
trg.append("/test-out.dvd");
  File fout(trg);
  FormatOutputStream fos(&fout);
  PacketOutputStream pos(&fos);

    AVCodecContext * c = fis.getFormatContext()->streams[stream_id]->codec;
    Decoder dec(c->codec_id);
    dec.setChannels(c->channels);
    dec.setBitRate(c->bit_rate);
    dec.setSampleRate(c->sample_rate);
    dec.setSampleFormat(c->sample_fmt);
    dec.setPixelFormat(c->pix_fmt);
    dec.setPixelFormat(PIX_FMT_YUV420P);
    dec.setTimeBase(c->time_base);
    dec.setWidth(c->width);
    dec.setHeight(c->height);
    dec.ctx->request_channel_layout = 2;
    dec.open();
    LOGDEBUG(dec.toString());

    Encoder enc(c->codec_id);
    enc.setChannels(c->channels);
    enc.setBitRate(c->bit_rate);
    enc.setSampleRate(c->sample_rate);
    enc.setSampleFormat(c->sample_fmt);
    enc.setPixelFormat(c->pix_fmt);
    enc.setPixelFormat(PIX_FMT_YUV420P);
    enc.setTimeBase(c->time_base);
    enc.setWidth(c->width);
    enc.setHeight(c->height);
    enc.ctx->request_channel_layout = 2;
    enc.open();
    LOGDEBUG(enc.toString());

    pos.setEncoder(enc,0);
    pos.init();






  Packet p;

  for(int a=0;a<100;a++){
    if(pis.readPacket(p)==0){
//p.toString();
//      if(p.getStreamIndex()==stream_id)
//        pos.writePacket(p);
    }
  }
}

