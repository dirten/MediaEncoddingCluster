#include "org/esb/hive/job/ProcessUnit.h"
#include "org/esb/av/Packet.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/FormatOutputStream.h"
#include "org/esb/av/PacketOutputStream.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/io/ObjectOutputStream.h"

#include "org/esb/av/Decoder.h"
#include "org/esb/av/Encoder.h"
#include "org/esb/av/Frame.h"
#include "org/esb/av/FrameConverter.h"

#include <boost/shared_ptr.hpp>
#include <list>
#include <deque>

using namespace org::esb::av;
using namespace org::esb::hive::job;
using namespace org::esb::io;

int pu_count = 80;

void build_audio_packet(const char * filename, int sidx) {

  std::list<boost::shared_ptr<Packet> > packet_list;
  /*open the fixed test File or the file from command line input*/
  std::string src;
  int stream_index = -1;
  if (filename == NULL) {
    src = MEC_SOURCE_DIR;
    src.append("/test.dvd");
  } else {
    src = filename;
  }
  if (sidx>-1) {
    stream_index = sidx;
  }
  /*opening the input file and Packet Input Stream*/
  File f(src.c_str());
  FormatInputStream fis(&f);
  PacketInputStream pis(&fis);
  fis.dumpFormat();


  /*ProcessUnit*/
  ProcessUnit p;
  /*find first audio Stream,Create and open the input Codec*/
  int c = fis.getStreamCount();
  for (int i = 0; i < c; i++) {
    if (fis.getStreamInfo(i)->getCodecType() != CODEC_TYPE_AUDIO) continue;
    if (stream_index == -1 || fis.getStreamInfo(i)->getIndex() == stream_index) {
      p._decoder = new Decoder(fis.getStreamInfo(i)->getCodec());
      p._source_stream = fis.getStreamInfo(i)->getIndex();
      p._target_stream = 0;
      break;
    }
  }

  /*Creating the Audio Encoder*/
  Encoder * enc = new Encoder(CODEC_ID_MP3);
  enc->setChannels(2);
  enc->setBitRate(128000);
  enc->setSampleRate(44100);
  enc->setSampleFormat(p._decoder->getSampleFormat());
  enc->setFlag(CODEC_FLAG_GLOBAL_HEADER);
  enc->setPixelFormat(PIX_FMT_YUV420P);
  enc->open();
  p._encoder = enc;

  char * outfile = new char[100];


  int pu_duration = 1000000;
  AVRational putb;
  putb.num = 1;
  putb.den = 1000000;


  AVRational enctb;
  enctb.num = 1;
  enctb.den = enc->getSampleRate();

  int64_t duration_count = 0;
  int64_t byte_count = 0;

  int64_t last_bytes_offset = 0;
  //  int frame_size=enc->ctx->frame_size;

  std::deque<boost::shared_ptr<Packet> > q;
  for (int a = 0; a < pu_count; a++) {
    sprintf(outfile, "packet-%d.pkt", a);
    FileOutputStream fos(outfile);
    ObjectOutputStream oos(&fos);
    Packet pac;
    Packet * prev_packet = NULL;

    for (int b = 1; pis.readPacket(pac) >= 0;) {
      if (pac.getStreamIndex() != p._source_stream)continue;
      boost::shared_ptr<Packet> pPacket(new Packet(pac));

      q.push_front(pPacket);


      if (q.size() > 4)
        q.pop_back();
      packet_list.push_back(pPacket);

      duration_count += av_rescale_q(pac.getDuration(), pac.getTimeBase(), enctb);
      byte_count += av_rescale_q(pac.getDuration(), pac.getTimeBase(), enctb)*4;


      //      std::cout<<"DurationCount:" << duration_count<<std::endl;
      //      std::cout<<"ByteCount:" << byte_count<<std::endl;

      if (b == 20) {



        p._input_packets = packet_list;

        p._encoder->_bytes_discard = last_bytes_offset;

        /**
         * calculating decoded sample size
         */
        int64_t in_frame_size = av_rescale_q(pac.getDuration(), pac.getTimeBase(), enctb)*4;
        int64_t out_frame_size = enc->getFrameBytes();
        std::cout << "Sample Delay:" << enc->ctx->delay << std::endl;
        std::cout << "Last Bytes Offset:" << last_bytes_offset << std::endl;
        std::cout << "in_frame_size:" << in_frame_size << std::endl;
        std::cout << "out_frame_size:" << out_frame_size << std::endl;

        /**
         * calculating number of bytes to discard
         */
        int64_t out_packet_count = ((in_frame_size * packet_list.size()) - last_bytes_offset) / out_frame_size;
        std::cout << "in_packet_count:" << packet_list.size() << std::endl;
        std::cout << "out_packet_count:" << out_packet_count << std::endl;
        int64_t in_bytes=packet_list.size()*in_frame_size;
        int64_t out_bytes=out_packet_count*out_frame_size;
        int64_t remaining_bytes=in_bytes-out_bytes;
        
        last_bytes_offset = (in_frame_size - remaining_bytes);

//        last_bytes_offset += (in_frame_size * (q.size()-2)) ;

        oos.writeObject(p);
        oos.close();
        packet_list.clear();
        int qsize = q.size();
        for (int a = 0; a < qsize; a++) {
          packet_list.push_back(q.back());
          q.pop_back();
        }
        duration_count = 0;
        duration_count = av_rescale_q(pac.getDuration(), pac.getTimeBase(), enctb);
        //        byte_count+=av_rescale_q(pac.getDuration(), pac.getTimeBase(),enctb)*4;
        break;
      }
      b++;
    }
  }
}

void compute_audio_packets() {
  char * file = new char[100];
  char * outfile = new char[100];

  for (int a = 0; a < pu_count; a++) {
    sprintf(file, "packet-%d.pkt", a);
    sprintf(outfile, "packet-%d.out", a);
    org::esb::io::File infile(file);
    org::esb::io::FileInputStream fis(&infile);
    org::esb::io::ObjectInputStream ois(&fis);
    org::esb::hive::job::ProcessUnit pu;
    ois.readObject(pu);
    pu.process();
    FileOutputStream fos(outfile);
    ObjectOutputStream oos(&fos);
    oos.writeObject(pu);
    delete pu._decoder;
    delete pu._encoder;
  }
}

void write_audio_to_file() {
  char * file = new char[100];
  org::esb::io::File outfile("test.mp3");
  FormatOutputStream fos(&outfile);
  PacketOutputStream pos(&fos);
  bool isInit = false;
  for (int a = 0; a < pu_count; a++) {
    sprintf(file, "packet-%d.out", a);
    org::esb::io::File infile(file);
    org::esb::io::FileInputStream fis(&infile);
    org::esb::io::ObjectInputStream ois(&fis);
    org::esb::hive::job::ProcessUnit pu;
    ois.readObject(pu);
    if (!isInit) {
      pu._encoder->open();
      pos.setEncoder(*pu._encoder);
      pos.init();
      isInit = true;
    }
    std::list<boost::shared_ptr<Packet> >::iterator it = pu._output_packets.begin();
    for (; it != pu._output_packets.end(); it++) {
      (*it)->setStreamIndex(0);
      (*it)->setPts(AV_NOPTS_VALUE);
      pos.writePacket(**it);
    }
  }
}

int main(int argc, char ** argv) {
  av_register_all();
  avcodec_init();
  avcodec_register_all();

  char * infile = NULL;
  int sidx = -1;
  build_audio_packet(infile, sidx);
  compute_audio_packets();
  write_audio_to_file();
  return 0;
}
