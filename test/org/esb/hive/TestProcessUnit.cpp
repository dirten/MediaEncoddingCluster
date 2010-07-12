#include "org/esb/io/File.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/FormatOutputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/PacketOutputStream.h"
#include "org/esb/av/Decoder.h"
#include "org/esb/av/Encoder.h"
#include "org/esb/hive/job/ProcessUnit.h"
#include "org/esb/hive/DatabaseService.h"
#include "org/esb/util/Log.h"
#include <string>

using namespace org::esb::av;
using namespace org::esb::io;
using namespace org::esb::hive;
using namespace org::esb::hive::job;

void test_process_video(int argc, char ** argv) {

  std::string src_file;
  if (argc == 1) {
    src_file = MEC_SOURCE_DIR;
    src_file.append("/test.dvd");
  } else {
    src_file = argv[1];
  }
  int stream_id = 0;
  if (argc == 3) {
    stream_id = atoi(argv[2]);
  }
  //  src_file="d:/windows/clock.avi";



  File infile(src_file);

  std::string trg_file = MEC_SOURCE_DIR;
  trg_file.append("/test.avi");
  File outfile(trg_file);
//   DatabaseService::start(MEC_SOURCE_DIR);

  if (true) {

    FormatInputStream fis(&infile);
    fis.dumpFormat();
    PacketInputStream pis(&fis);

    FormatOutputStream fos(&outfile);
    PacketOutputStream pos(&fos);

    AVCodecContext * c = fis.getFormatContext()->streams[stream_id]->codec;
    boost::shared_ptr<Decoder> dec = boost::shared_ptr<Decoder>(new Decoder(c->codec_id));
    LOGDEBUG("ChannelLayout:" << fis.getFormatContext()->streams[stream_id]->codec->channel_layout)
    //  Decoder dec(c);

    //  dec.setChannels(c->channels);
    dec->setChannels(2);
    dec->setBitRate(c->bit_rate);
    dec->setPixelFormat(c->pix_fmt);
    dec->setTimeBase(c->time_base);
    dec->setWidth(c->width);
    dec->setHeight(c->height);
    dec->ctx->request_channel_layout = 2;
    //  dec.ctx->request_channels = 2;
    dec->open();
    LOGDEBUG(dec->toString());




    //  Encoder enc(CODEC_ID_MSMPEG4V1);
    boost::shared_ptr<Encoder> enc = boost::shared_ptr<Encoder>(new Encoder(CODEC_ID_H264));
    enc->setChannels(2);
    enc->setBitRate(1024000);
    //  enc.setSampleRate(44100);
    enc->setPixelFormat(dec->getPixelFormat());
    AVRational ar;
    ar.num = 1;
    ar.den = 25;
    enc->setGopSize(20);
    enc->setTimeBase(ar);
    enc->setWidth(320);
    enc->setHeight(240);
    //  enc.setFlag(CODEC_FLAG_GLOBAL_HEADER);
    //  enc.setPixelFormat(PIX_FMT_YUV420P);
    //  enc.ctx->bits_per_raw_sample=dec.ctx
    enc->open();

    LOGDEBUG(enc->toString());
    LOGDEBUG("Encoder Frame Size:" << enc->ctx->frame_size);
    Packet p;
    ProcessUnit u;
    u._decoder = dec;
    u._encoder = enc;

    for (int a = 0; a < 12;) {
      pis.readPacket(p);
      if (p.getStreamIndex() == stream_id) {
        a++;
        LOGDEBUG(p.toString());
        boost::shared_ptr<Packet> pptr(new Packet(p));
        u._input_packets.push_back(pptr);
      }
    }

    FileOutputStream fs("test.unit");
    ObjectOutputStream oos(&fs);
    oos.writeObject(u);
    oos.close();
    fs.close();
//    delete dec;
//    delete enc;
//    dec = NULL;
//    enc = NULL;
  }


  if (true) {
    FileInputStream fiis("test.unit");
    ObjectInputStream oois(&fiis);

    ProcessUnit puin;
    oois.readObject(puin);
    puin.process();

    // u.process();
    std::cout<<"Ouput Packet size"<<puin._output_packets.size()<<std::endl;

    FileOutputStream foos("test-out.unit");
    ObjectOutputStream ooos(&foos);
    ooos.writeObject(puin);
//    delete puin._decoder;
//    delete puin._encoder;
  }

}

void test_process_audio(char * file){


  std::string src_file = MEC_SOURCE_DIR;
  src_file.append("/test.dvd");
  File infile(src_file);
  int stream_id = 1;

  std::string trg_file = MEC_SOURCE_DIR;
  trg_file.append("/test.avi");
  File outfile(trg_file);

  if (true) {

    FormatInputStream fis(&infile);
    fis.dumpFormat();
    PacketInputStream pis(&fis);

    FormatOutputStream fos(&outfile);
    PacketOutputStream pos(&fos);

    AVCodecContext * c = fis.getFormatContext()->streams[stream_id]->codec;
    boost::shared_ptr<Decoder> dec = boost::shared_ptr<Decoder>(new Decoder(c->codec_id));
    LOGDEBUG("ChannelLayout:" << fis.getFormatContext()->streams[stream_id]->codec->channel_layout)
    //  Decoder dec(c);

    //  dec.setChannels(c->channels);
    dec->setChannels(2);
    dec->setBitRate(c->bit_rate);
    dec->setPixelFormat(c->pix_fmt);
    dec->setTimeBase(c->time_base);
    dec->setWidth(c->width);
    dec->setHeight(c->height);
    dec->setSampleFormat(c->sample_fmt);
    dec->setSampleRate(c->sample_rate);
    dec->ctx->request_channel_layout = 2;
    //  dec.ctx->request_channels = 2;
    dec->open();



    LOGDEBUG(dec->toString());

    //  Encoder enc(CODEC_ID_MSMPEG4V1);
    boost::shared_ptr<Encoder> enc = boost::shared_ptr<Encoder>(new Encoder(CODEC_ID_VORBIS));
    enc->setChannels(2);
    enc->setBitRate(128000);
    enc->setSampleRate(44100);
    enc->setSampleFormat(dec->getSampleFormat());
    AVRational ar;
    ar.num = 1;
    ar.den = 44100;
    //	enc->setGopSize(20);
    enc->setTimeBase(ar);
    //    enc->setWidth(320);
    //    enc->setHeight(240);
    //  enc.setFlag(CODEC_FLAG_GLOBAL_HEADER);
    //  enc.setPixelFormat(PIX_FMT_YUV420P);
    //  enc.ctx->bits_per_raw_sample=dec.ctx
    enc->open();

    LOGDEBUG(enc->toString());
    LOGDEBUG("Encoder Frame Size:" << enc->ctx->frame_size);
    Packet p;
    ProcessUnit u;
    u._decoder = dec;
    u._encoder = enc;

    for (int a = 0; a < 10;) {
      pis.readPacket(p);
      if (p.getStreamIndex() == stream_id) {
        a++;
        LOGDEBUG(p.toString());
        boost::shared_ptr<Packet> pptr(new Packet(p));
        u._input_packets.push_back(pptr);
      }
    }

    FileOutputStream fs("test.audio");
    ObjectOutputStream oos(&fs);
    oos.writeObject(u);
    oos.close();
    fs.close();
  }


  if (true) {
    FileInputStream fiis("test.audio");
    ObjectInputStream oois(&fiis);

    ProcessUnit puin;
    oois.readObject(puin);
    puin.process();

    // u.process();

    FileOutputStream foos("test-out.unit");
    ObjectOutputStream ooos(&foos);
    ooos.writeObject(puin);
    ooos.close();
  }


}
int main(int argc, char**argv) {


  Log::open("");

  av_register_all();
  avcodec_init();
  avcodec_register_all();

  test_process_video(argc, argv);
  test_process_audio(argv[1]);
  Log::close();
}

