/* 
 * File:   PresetTest.cpp
 * Author: HoelscJ
 *
 * Created on 13. September 2010, 16:12
 */

#include "org/esb/db/hivedb.hpp"
#include <cstdlib>
#include <map>
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/FormatOutputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/PacketOutputStream.h"
#include "org/esb/av/Decoder.h"
#include "org/esb/av/Encoder.h"
#include "org/esb/av/Packet.h"
#include "org/esb/av/Frame.h"
#include "org/esb/av/FrameConverter.h"


#include "org/esb/io/File.h"
#include "org/esb/util/Log.h"
#include "org/esb/lang/Ptr.h"

#include "org/esb/hive/PresetReader.h"
#include "org/esb/hive/CodecPropertyTransformer.h"
#include "org/esb/hive/CodecFactory.h"

using namespace std;
using namespace org::esb::av;
using namespace org::esb::io;
using namespace org::esb::hive;

/*creating the decoder map*/
map<int, Ptr<Decoder> > decoder;

/*creating the FrameConverter map*/
map<int, Ptr<FrameConverter> > converter;

/*creating the encoder map*/
map<int, boost::shared_ptr<Encoder> > encoder;

/*stream index map*/
map<int, int> stream_map;


class PacketSink : public Sink {
public:
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

/**
 * central converter function, called for each packet
 * @param p
 * @return
 */
Packet * convert(Packet & p) {
  if(!decoder.count(p.getStreamIndex())>0)return NULL;
  Frame * decodedFrame = decoder[p.getStreamIndex()]->decode2(p);
  if(!decodedFrame->isFinished()){
    delete decodedFrame;
    return NULL;
  }
  int idx=p.getStreamIndex();
      /*target frame for conversion*/
    Frame * f = NULL;
    /*allocation frame data for specified type*/
    if (decoder[idx]->ctx->codec_type == CODEC_TYPE_VIDEO)
      f = new Frame(encoder[idx]->getInputFormat().pixel_format, encoder[idx]->getWidth(), encoder[idx]->getHeight());
    if (decoder[idx]->ctx->codec_type == CODEC_TYPE_AUDIO)
      f = new Frame();
    LOGTRACE("try Frame Convert");
    /*converting the source frame to target frame*/
    converter[idx]->convert(*decodedFrame, *f);
    f->stream_index=stream_map[f->stream_index];
    encoder[idx]->encode(*f);
    delete f;
    delete decodedFrame;
    return NULL;
}

int main(int argc, char** argv) {
  if (argc < 3) {
    std::cout << "usage: presetfile inputvideo" << std::endl;
    exit(0);
  }
  if (!File(argv[1]).exists()) {
    std::cout << "could not open preset file, filedoes not exist" << std::endl;
    exit(0);
  }
  if (!File(argv[2]).exists()) {
    std::cout << "could not open inputvideo file, file does not exist" << std::endl;
    exit(0);
  }
  Log::open();

  /*open the input video file*/
  File videofile(argv[2]);
  FormatInputStream fis(&videofile);
  PacketInputStream pis(&fis);
  Packet p;

    /*loading the preset*/
  PresetReader pr(argv[1]);
  PresetReader::CodecList clist = pr.getCodecList();

  /*tranforming the filter parameter*/
  PresetReader::FilterList flist = pr.getFilterList();
  PresetReader::FilterList::iterator flist_it = flist.begin();
  for (; flist_it != flist.end(); flist_it++) {
    std::multimap<std::string, std::string> param = (*flist_it).second;
    if((*flist_it).first=="resize"){
      clist["video"].insert(std::pair<std::string, std::string>("width",param.find("width")->second));
      clist["video"].insert(std::pair<std::string, std::string>("height",param.find("height")->second));
      //clist["video"]["height"]=param["height"];
    }
  }
  boost::shared_ptr<Encoder> videoEncoder2 = CodecFactory::getStreamEncoder(clist["video"]);
  if(!videoEncoder2->open())
    exit(1);
  boost::shared_ptr<Encoder> audioEncoder2 = CodecFactory::getStreamEncoder(clist["audio"]);
  if(!audioEncoder2->open())
    exit(1);


  /*creating the decoder*/
  bool have_audio=false, have_video=false;
  int i=0;
  for (int a = 0; a < fis.getStreamCount(); a++) {
    decoder[a] = new Decoder(fis.getAVStream(a));
    if (!decoder[a]->open()) {
      LOGERROR("could not open decoder for input stream # " << a);
      decoder.erase(a);
    } else {
      LOGINFO("Decoder opened for stream # " << a);
      if(decoder[a]->getCodecType()==AVMEDIA_TYPE_VIDEO){
        encoder[a]=videoEncoder2;
        have_video=true;
      }
      if(decoder[a]->getCodecType()==AVMEDIA_TYPE_AUDIO){
        encoder[a]=audioEncoder2;
        have_audio=true;
      }
      converter[a]=new FrameConverter(decoder[a].get(),encoder[a].get());
      stream_map[a]=i++;
      LOGDEBUG("Decoder:"<<decoder[a]->toString());
      LOGDEBUG("Encoder:"<<encoder[a]->toString());
      /*only encoding the first audio and video stream here*/
      if(have_video&&have_audio)break;
    }
  }
  //return 0;
  File outfile(videofile.getFileName());
  PresetReader::Preset preset=pr.getPreset();
  outfile.changeExtension(preset["fileExtension"]);
  FormatOutputStream fos(&outfile);
  PacketOutputStream pos(&fos);
  pos.setEncoder(*videoEncoder2.get(),0);
  pos.setEncoder(*audioEncoder2.get(),1);

  if(!pos.init()){
    exit(0);
  }
  videoEncoder2->setOutputStream(&pos);
  audioEncoder2->setOutputStream(&pos);
  while (pis.readPacket(p)==0) {
    Packet * encodedPacket = convert(p);
  }
  pos.close();
  fos.close();

/*
  std::list<boost::shared_ptr<Packet> > packets=sink.getList();
  std::list<boost::shared_ptr<Packet> >::iterator it=packets.begin();
  for(;it!=packets.end();it++){
    pos.writePacket(*(*it).get());
  }*/


  return 0;
}

