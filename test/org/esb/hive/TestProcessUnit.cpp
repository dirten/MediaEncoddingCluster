#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/FormatOutputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/PacketOutputStream.h"
#include "org/esb/av/Decoder.h"
#include "org/esb/av/Encoder.h"
#include "org/esb/hive/job/ProcessUnit.h"
#include <string>

using namespace org::esb::av;
using namespace org::esb::io;
using namespace org::esb::hive::job;
int main()	{

  av_register_all();
  avcodec_init();
  avcodec_register_all();
 
  std::string src_file = MEC_SOURCE_DIR;
  src_file.append("/test.dvd");
  File infile(src_file);
  int stream_id = 0;

  std::string trg_file = MEC_SOURCE_DIR;
  trg_file.append("/test.avi");
  File outfile(trg_file);



  FormatInputStream fis(&infile);
  PacketInputStream pis(&fis);

  FormatOutputStream fos(&outfile);
  PacketOutputStream pos(&fos);

  AVCodecContext * c = fis.getFormatContext()->streams[stream_id]->codec;
  Decoder dec(c->codec_id);
  logdebug("ChannelLayout:"<<fis.getFormatContext()->streams[stream_id]->codec->channel_layout)
  //  Decoder dec(c);

//  dec.setChannels(c->channels);
  dec.setChannels(2);
  dec.setBitRate(c->bit_rate);
  dec.setPixelFormat(c->pix_fmt);
  dec.setTimeBase(c->time_base);
  dec.setWidth(c->width);
  dec.setHeight(c->height);
  dec.ctx->request_channel_layout = 2;
//  dec.ctx->request_channels = 2;
  dec.open();



  logdebug(dec.toString());

  Encoder enc(CODEC_ID_MSMPEG4V3);
  enc.setChannels(2);
  enc.setBitRate(1024000);
//  enc.setSampleRate(44100);
  enc.setPixelFormat(dec.getPixelFormat());
  AVRational ar;
  ar.num=1;
  ar.den=25;
  enc.setTimeBase(ar);
  enc.setWidth(320);
  enc.setHeight(240);
//  enc.setFlag(CODEC_FLAG_GLOBAL_HEADER);
//  enc.setPixelFormat(PIX_FMT_YUV420P);
//  enc.ctx->bits_per_raw_sample=dec.ctx
  enc.open();

  logdebug(enc.toString());
  logdebug("Encoder Frame Size:"<<enc.ctx->frame_size);
  Packet p;
  ProcessUnit u;
  u._decoder=&dec;
  u._encoder=&enc;

  for (int a = 0; a < 10; ) {
	pis.readPacket(p);
    if (p.getStreamIndex() == stream_id) {
		a++;
	boost::shared_ptr<Packet> pptr(new Packet(p));
	u._input_packets.push_back(pptr);
	}
   }
  u.process();
}
