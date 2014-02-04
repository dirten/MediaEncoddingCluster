/* 
 * File:   pu.cpp
 * Author: jhoelscher
 *
 * Created on 29. September 2009, 13:33
 */
#include "org/esb/av/Packet.h"
#include "org/esb/av/FormatBaseStream.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/Packet.h"
#include "org/esb/av/FormatInputStream.h"

#include "org/esb/io/FileOutputStream.h"
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/hive/job/ProcessUnit.h"
#include <stdlib.h>
#include <string>
#include "org/esb/util/Log.h"
#include "org/esb/util/Foreach.h"
#include "org/esb/util/StringUtil.h"

#include "org/esb/hive/Environment.h"
/*
 * 
 */
using namespace org::esb::io;
using namespace org::esb::av;
using namespace org::esb::hive::job;

class PacketSink : public Sink {
public:

  PacketSink() {
  }

  void write(void * p) {
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

void help() {
  std::cout << "usage: pu <view|execute> infile [outfile]" << std::endl;
}

void writeFrame(Frame * frame, int count, std::string suffix) {
  LOGDEBUG("write frame count=" << count << " sufix " << suffix);
  PacketSink s;
  org::esb::av::Encoder enc(CODEC_ID_MJPEG);
  enc.setPixelFormat(PIX_FMT_YUVJ420P);
  enc.setBitRate(1000000);
  enc.setTimeBase(1, 1);
  enc.setSink(&s);
  enc.setWidth(frame->getWidth());
  enc.setHeight(frame->getHeight());
  enc.open();
  enc.encode(*frame);
  std::string path = "./";
  path += org::esb::util::StringUtil::toString(count);
  path += "-" + suffix;
  path += ".jpg";
  FILE *jpegImgFile = fopen(path.c_str(), "wb");
  fwrite((char*) s.getList().front()->getAVPacket()->data, 1, s.getList().front()->getAVPacket()->size, jpegImgFile);
  fclose(jpegImgFile);
}

void writeProcessUnit(ProcessUnit & unit) {
  LOGDEBUG("Write Images")
  unit._decoder->open();
  unit._encoder->open();
  boost::shared_ptr<Decoder >_refdecoder;
  if (unit._encoder->getCodecType() == AVMEDIA_TYPE_VIDEO) {
    LOGDEBUG("create reference decoder")
    std::map<std::string, std::string>opt = unit._encoder->getCodecOptions();
    _refdecoder = boost::shared_ptr<Decoder > (new Decoder(unit._encoder->getCodecId()));
    std::map<std::string, std::string>::iterator opit = opt.begin();
    _refdecoder->setWidth(unit._encoder->getWidth());
    _refdecoder->setHeight(unit._encoder->getHeight());
    _refdecoder->setPixelFormat(unit._encoder->getPixelFormat());

    for (; opit != opt.end(); opit++) {
      if ((*opit).first != "extradata" || (*opit).first != "extradata_size")
        _refdecoder->setCodecOption((*opit).first, (*opit).second);
    }
    LOGDEBUG("EncoderExtrdataSize:" << unit._encoder->ctx->extradata_size);
    LOGDEBUG("RefDecoderExtrdataSize:" << _refdecoder->ctx->extradata_size);
    //std::cout << _encoder->ctx->extradata;
    _refdecoder->ctx->extradata = static_cast<uint8_t*> (av_malloc(unit._encoder->ctx->extradata_size));
    memcpy(_refdecoder->ctx->extradata, unit._encoder->ctx->extradata, unit._encoder->ctx->extradata_size);
    //_refdecoder->ctx->extradata[0] = 2;
    //_refdecoder->ctx->extradata_size=0;
    _refdecoder->open();
  }
  int count = 0;
  unit._decoder->reset();

  foreach(std::list<boost::shared_ptr<Packet> >::value_type & packet, unit._input_packets) {
    Frame *tmp = unit._decoder->decode2(*packet);
    writeFrame(tmp, count++, "in");
  }
  count = 0;

  foreach(std::list<boost::shared_ptr<Packet> >::value_type & packet, unit._output_packets) {
    Frame *tmp = _refdecoder->decode2(*packet);
    if (tmp->isFinished())
      writeFrame(tmp, count++, "out");
  }
  bool last_packets = true;
  while (last_packets) {
    boost::shared_ptr<Packet >p = boost::shared_ptr<Packet > (new Packet());
    p->setTimeBase(unit._input_packets.front()->getTimeBase());
    p->setDuration(unit._input_packets.front()->getDuration());
    p->setStreamIndex(unit._input_packets.front()->getStreamIndex());
    Frame * lastf = _refdecoder->decode2(*p);
    if (!lastf->isFinished()) {
      last_packets = false;
    } else {
      writeFrame(lastf, count++, "out");
    }
  }


}

void execute(char * infile, char * outfile) {
  FileInputStream fis(infile);
  ObjectInputStream ois(&fis);
  //ProcessUnit pu;
  boost::shared_ptr<ProcessUnit> pu;
  ois.readObject(pu);
  //pu->_encoder->setCodecOption("multipass", "1");
  //pu->_encoder->reset();
  //pu->_decoder->reset();
  pu->process();
  delete pu->_converter;

  LOGDEBUG("output packets"<<pu->_output_packets.size())
  FileOutputStream fos(outfile);
  ObjectOutputStream oos(&fos);
  oos.writeObject(pu);
  //writeProcessUnit(*pu.get());
}

void view_codec_data(boost::shared_ptr<Codec>c) {
  c->open();
  printf("%10ld|", c->getCodecType());
  printf("%10x|", c->getCodecId());
  printf("%10s|", c->ctx->codec_name);
  printf("%10s|", c->ctx->av_class->class_name);
  printf("%10ld|", c->ctx->frame_size);

}

void view_packet_data(Packet * p) {
  int idx = 0;
  int64_t inpts = 0;
  int64_t indts = 0;
  AVRational intb;
  int64_t indur = 0;
  int64_t size=0;
  memset(&intb, 0, sizeof (intb));
  bool isKey = false;
  if (p != NULL) {
    idx = p->getStreamIndex();
    inpts = p->getPts();
    indts = p->getDts();
    intb = p->getTimeBase();
    indur = p->getDuration();
    isKey = p->isKeyFrame();
    size = p->packetPtr->size;
  }
  printf("%3d|", idx);
  printf("%20lld|", inpts);
  printf("%20lld|", indts);
  printf("%6d/", intb.num);
  printf("%6d/", intb.den);
  printf("%6d", indur);
  printf("%1s", isKey ? "x" : " ");
  printf("%6d", size);

}

void viewPuData(ProcessUnit & pu) {
  printf("----------------------------------------------------------------------------------------------------------\n");
  //printf("%30s=%30s\n", "compensatebase",pu._frameRateCompensateBase);
  //printf("%30s=%30s", "gop",pu._gop_size);
  //printf("%30s=%30s", "expected",pu._expected_frame_count);

  printf("----------------------------------------------------------------------------------------------------------\n");

}

void view(char * filename) {
  FileInputStream fis(filename);
  ObjectInputStream ois(&fis);
  boost::shared_ptr<ProcessUnit> pu_get;// = new ProcessUnit();
  ois.readObject(pu_get);
  LOGDEBUG("readed");
  viewPuData(*pu_get.get());
  ProcessUnit pu = *pu_get.get();
  //  logdebug(pu._decoder->toString());
  //  logdebug(pu._encoder->toString());
  printf("----------------------------------------------------------------------------------------------------------");
  cout << endl;
  printf("%10s|", "inframes");
  printf("%10s|", "outframes");
  printf("%10s|", "decoder");
  printf("%10s|", "encoder");
  cout << endl;
  printf("%10ld|", pu._input_packets.size());
  printf("%10ld|", pu._output_packets.size());
  printf("%10ld|", pu._decoder->getCodecId());
  printf("%10ld|", pu._encoder->getCodecId());
  cout << endl;
  printf("----------------------------------------------------------------------------------------------------------");
  cout << endl;
  printf("%10s|", "Type");
  printf("%10s|", "ID");
  printf("%10s|", "CodecName");
  printf("%10s|", "AvClass");
  printf("%10s|", "FrameSize");
  cout << endl;
  printf("----------------------------------------------------------------------------------------------------------");
  cout << endl;
  view_codec_data(pu._decoder);
  cout << endl;
  view_codec_data(pu._encoder);
  cout << endl;
  printf("----------------------------------------------------------------------------------------------------------");
  cout << endl;
  int c = max(pu._input_packets.size(), pu._output_packets.size());
  std::vector<boost::shared_ptr<Packet> > in(pu._input_packets.begin(), pu._input_packets.end());
  std::vector<boost::shared_ptr<Packet> > out(pu._output_packets.begin(), pu._output_packets.end());
  printf("%10s|", "count");
  printf("%3s|", "idx");
  printf("%20s|", "pts");
  printf("%20s|", "dts");
  printf("%6s/", ".num");
  printf("%6s/", ".den");
  printf("%6s", "dur");
  printf("%ss", "K");
  printf("%3s|", "idx");
  printf("%20s|", "pts");
  printf("%20s|", "dts");
  printf("%6s/", ".num");
  printf("%6s/", ".den");
  printf("%6s", "dur");
  printf("%ss", "K");
  cout << endl;
  printf("----------------------------------------------------------------------------------------------------------");
  cout << endl;
  for (int a = 0; a < c; a++) {
    printf("%10d|", a + 1);
    if (in.size() > a)
      view_packet_data(in[a].get());
    else
      view_packet_data(NULL);
    if (out.size() > a)
      view_packet_data(out[a].get());
    else
      view_packet_data(NULL);
    cout << endl;
  }
  cout << endl;
}

void packet_count(char * filename, char * stream_index) {
  int idx = atoi(stream_index);
  File file(filename);
  FormatInputStream fis(&file);
  fis.dumpFormat();
  PacketInputStream pis(&fis);
  Packet pac;
  int c = 0;
  while (pis.readPacket(pac) >= 0) {
    if (pac.getStreamIndex() == idx)c++;
  }
  LOGDEBUG("Packet Count from stream :" << idx << " = " << c);
}

int main(int argc, char** argv) {
  if (argc < 3) {
    help();
    exit(0);
  }
  org::esb::hive::Environment::build(argc, argv);
  std::string logconfigpath = MEC_SOURCE_DIR;
  logconfigpath.append("/res");
  //  Log::open(logconfigpath);
  Log::open("");
  org::esb::av::FormatBaseStream::initialize();

  std::string command = argv[1];
  char * infilename = argv[2];
  if (command == "view")view(infilename);
  if (command == "execute")execute(infilename, argv[3]);
  if (command == "pc")packet_count(infilename, argv[3]);
  org::esb::hive::Environment::close();
  return (EXIT_SUCCESS);
}

