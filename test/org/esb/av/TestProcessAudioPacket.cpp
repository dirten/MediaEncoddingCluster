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

#include "org/esb/hive/job/Packetizer.h"
#include <boost/shared_ptr.hpp>
#include <list>
#include <deque>

using namespace org::esb::av;
using namespace org::esb::hive::job;
using namespace org::esb::io;

int pu_count = 50;

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

  std::map<int, Packetizer::StreamData> stream_data;
  /*ProcessUnit*/
  ProcessUnit p;
  /*find first audio Stream,Create and open the input Codec*/
  int c = fis.getStreamCount();
  for (int i = 0; i < c; i++) {
    if (fis.getStreamInfo(i)->getCodecType() != CODEC_TYPE_AUDIO) continue;
    if (stream_index == -1 || fis.getStreamInfo(i)->getIndex() == stream_index) {
      p._decoder = boost::shared_ptr<Decoder>(new Decoder(fis.getAVStream(i)));
      p._source_stream = fis.getStreamInfo(i)->getIndex();
      p._target_stream = 0;
      /**
       * collecting data for the Packetizer
       */
      stream_data[p._source_stream].decoder = p._decoder;
//      stream_data[p._source_stream].codec_id = p._decoder->getCodecId();

      break;
    }
  }

  /*Creating the Audio Encoder*/
  boost::shared_ptr<Encoder> enc = boost::shared_ptr<Encoder>(new Encoder(CODEC_ID_MP2));
  enc->setChannels(2);
  enc->setBitRate(128000);
  enc->setSampleRate(48000);
  enc->setTimeBase(1,48000);
  enc->setSampleFormat(p._decoder->getSampleFormat());
//  enc->setFlag(CODEC_FLAG_GLOBAL_HEADER);
  if(!enc->open()){
    assert(false);
  }

  p._encoder = enc;
  stream_data[p._source_stream].encoder = p._encoder;

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
  Packetizer packetizer(stream_data);
  bool more_packets = true;
  for (int a = 0; more_packets; a++) {
    Packet pac;
    Packet * prev_packet = NULL;

    for (int b = 1; more_packets;) {
      boost::shared_ptr<Packet> pPacket;
      if (pis.readPacket(pac) < 0) {
        more_packets = false;
        packetizer.flushStreams();
        goto flush;
      }
      if (pac.getStreamIndex() != p._source_stream)continue;
      pPacket=boost::shared_ptr<Packet>(new Packet(pac));

      if (packetizer.putPacket(pPacket)) {
        flush:
        PacketListPtr packets = packetizer.removePacketList();

        p._input_packets = std::list<boost::shared_ptr<Packet> >(packets.begin(), packets.end());
        p._discard_audio_bytes = last_bytes_offset;


//        std::list<boost::shared_ptr<Packet> > testlist= std::list<boost::shared_ptr<Packet> >(packets.begin(), packets.begin()+1);



//        std::cout << "testlistsize:"<<testlist.size()<<std::endl;
        /**
         * calculating decoded sample size
         */
        int64_t in_frame_size = av_rescale_q(pac.getDuration()*4, pac.getTimeBase(), enc->getTimeBase());
        int64_t out_frame_size = enc->getFrameBytes();


        std::cout << "Sample Delay:" << enc->ctx->delay << std::endl;
        std::cout << "Last Bytes Offset:" << last_bytes_offset << std::endl;
        std::cout << "in_frame_size:" << in_frame_size << std::endl;
        std::cout << "out_frame_size:" << out_frame_size << std::endl;

        /**
         * calculating number of bytes to discard
         */
        int64_t in_bytes = (packets.size() * in_frame_size)+last_bytes_offset;

        int64_t out_packet_count = in_bytes / out_frame_size;

        std::cout << "in_packet_count:" << packets.size() << std::endl;
        std::cout << "out_packet_count:" << out_packet_count << std::endl;

        int64_t out_bytes = out_packet_count*out_frame_size;

        int64_t remaining_bytes = in_bytes - out_bytes;

        last_bytes_offset = remaining_bytes;//(in_frame_size - remaining_bytes);

        //        last_bytes_offset += (in_frame_size * (q.size()-2)) ;
        sprintf(outfile, "packet-%d.pkt", a);
        FileOutputStream fos(outfile);
        ObjectOutputStream oos(&fos);

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

  for (int a = 0; true; a++) {
    sprintf(file, "packet-%d.pkt", a);
    sprintf(outfile, "packet-%d.out", a);
    org::esb::io::File infile(file);
    if (!infile.exists())break;
    org::esb::io::FileInputStream fis(&infile);
    org::esb::io::ObjectInputStream ois(&fis);
    org::esb::hive::job::ProcessUnit pu;
    ois.readObject(pu);
    pu.process();
    FileOutputStream fos(outfile);
    ObjectOutputStream oos(&fos);
    oos.writeObject(pu);
  }
}

void write_audio_to_file() {
  char * file = new char[100];
  org::esb::io::File outfile("test.mp3");
  boost::shared_ptr<Encoder> enc;
  FormatOutputStream fos(&outfile);
  PacketOutputStream pos(&fos);
  bool isInit = false;
  for (int a = 0; true; a++) {
    sprintf(file, "packet-%d.out", a);
    org::esb::io::File infile(file);
    if (!infile.exists())break;
    org::esb::io::FileInputStream fis(&infile);
    org::esb::io::ObjectInputStream ois(&fis);
    org::esb::hive::job::ProcessUnit pu;
    ois.readObject(pu);
    if (!isInit) {
      enc=pu._encoder;
      enc->open();
      pos.setEncoder(*enc);
      pos.init();
      isInit = true;
    }
    LOGDEBUG(enc->toString());
    std::list<boost::shared_ptr<Packet> >::iterator it = pu._output_packets.begin();
    for (; it != pu._output_packets.end(); it++) {
      (*it)->setStreamIndex(0);
      (*it)->setPts(AV_NOPTS_VALUE);
      pos.writePacket(*(*it));
    }
  }
}

int main(int argc, char ** argv) {
  Log::open("");
  av_register_all();
  avcodec_init();
  avcodec_register_all();

  char * infile = NULL;
  if(argc>1)
    infile=argv[1];
  int sidx = -1;
  build_audio_packet(infile, sidx);
  compute_audio_packets();
  write_audio_to_file();
  return 0;
}
