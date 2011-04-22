/*
 * TestAudioEncoder.cpp
 *
 *  Created on: 10.03.2010
 *      Author: jhoelscher
 */



#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/Decoder.h"
#include "org/esb/av/Encoder.h"
#include "org/esb/av/FrameConverter.h"
#include "org/esb/av/Sink.h"
#include "org/esb/av/Frame.h"

#include "org/esb/util/Log.h"


#include <iostream>
using namespace org::esb::av;
using namespace org::esb::io;


void decode(Decoder & dec,Encoder & enc, FrameConverter& conv, std::list<boost::shared_ptr<org::esb::av::Packet> > & packets);
Frame * convert(FrameConverter& conv, Frame *);
void encode(Encoder & enc, Frame * frame);

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
int main(int argc, char** argv){
  if(argc<2){
    std::cout <<"no filename given"<<std::endl;
    exit(1);
  }
  Log::open("");
  File file(argv[1]);
  FormatInputStream fis(&file);
  PacketInputStream pis(&fis);

  /**
   * retriving the first audio stream
   */
  int c = fis.getStreamCount();
  int stream = -1;
  for (int a = 0; a < c; a++) {
    if (fis.getAVStream(a)->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
      stream = a;
      break;
    }
  }
  if(stream<0){
    std::cout <<"no audio stream found"<<std::endl;
  }

  org::esb::av::Decoder dec(fis.getAVStream(stream));
//  dec.setTimeBase(1, 25);
  dec.open();

  /*Creating the Audio Encoder*/
  Encoder enc (CODEC_ID_MP2);
  enc.setChannels(2);
  enc.setBitRate(128000);
//  enc.setSampleRate(dec.getSampleRate());
  enc.setSampleRate(44100);
  enc.setSampleFormat(dec.getSampleFormat());
//  enc.setFlag(CODEC_FLAG_GLOBAL_HEADER);
//  enc.setPixelFormat(PIX_FMT_YUV420P);
  enc.open();
  PacketSink sink;
  enc.setSink(&sink);

  FrameConverter conv(&dec, &enc);

  org::esb::av::Packet * p;
  std::list<boost::shared_ptr<org::esb::av::Packet> > packetlist;

  int incount = 0;
  int outcount = 0;

  for (int i = 0; i < 25;) {
    if ((p = pis.readPacket()) != NULL && p->getStreamIndex() == stream) {
      packetlist.push_back(boost::shared_ptr<org::esb::av::Packet>(p));
      i++;
    }else{
      delete p;
    }
  }
  decode(dec, enc, conv, packetlist);
  LOGDEBUG("Encoded packet count:"<<sink.getList().size());
  Log::close();
}

void decode(Decoder & dec,Encoder & enc,FrameConverter& conv, std::list<boost::shared_ptr<org::esb::av::Packet> > & packets){
  std::list<boost::shared_ptr<org::esb::av::Packet> >::iterator it=packets.begin();
  for(;it!=packets.end();it++){
    Frame * frame=dec.decode2(*(*it));
    Frame * f2=convert(conv,frame);
    encode(enc, f2);
    delete f2;
  }
}

Frame * convert(FrameConverter& conv, Frame * frame){
  Frame * f = new Frame();
  conv.convert(*frame, *f);
  delete frame;
  return f;
}
void encode(Encoder & enc, Frame * frame){
  enc.encode(*frame);
}

