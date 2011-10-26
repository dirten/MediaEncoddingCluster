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
#include "org/esb/io/FileInputStream.h"
#include "org/esb/util/Log.h"
#include "org/esb/util/StringUtil.h"
#include "org/esb/lang/Ptr.h"

#include "org/esb/hive/PresetReaderJson.h"
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
    if (decoder[idx]->ctx->codec_type == AVMEDIA_TYPE_VIDEO)
      f = new Frame(encoder[idx]->getInputFormat().pixel_format, encoder[idx]->getWidth(), encoder[idx]->getHeight());
    if (decoder[idx]->ctx->codec_type == AVMEDIA_TYPE_AUDIO)
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

int encode(int argc, char** argv) {
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
  File preset_file(argv[1]);
  if(!preset_file.exists()){
    LOGERROR("could not find preset file "<<preset_file.getFilePath());
  }
  FileInputStream pfis(&preset_file);

  std::string preset_data;
  pfis.read(preset_data);
  PresetReaderJson pr(preset_data);
  PresetReaderJson::CodecList clist = pr.getCodecList();

  /*tranforming the filter parameter*/
  PresetReaderJson::FilterList flist = pr.getFilterList();
  PresetReaderJson::FilterList::iterator flist_it = flist.begin();
  for (; flist_it != flist.end(); flist_it++) {
    std::map<std::string, std::string> param = (*flist_it).second;
    if((*flist_it).first=="resize"){
      clist["video"]["width"]=param.find("width")->second;//.insert(std::pair<std::string, std::string>("width",param.find("width")->second));
      clist["video"]["height"]=param.find("height")->second;//.insert(std::pair<std::string, std::string>("height",param.find("height")->second));
      //clist["video"]["height"]=param["height"];
    }
  }
  AVCodec * vcodec=avcodec_find_encoder_by_name((*clist["video"].find("codec_id")).second.c_str());
  AVCodec * acodec=avcodec_find_encoder_by_name((*clist["audio"].find("codec_id")).second.c_str());

  (*clist["video"].find("codec_id")).second=org::esb::util::StringUtil::toString(vcodec->id);
  (*clist["audio"].find("codec_id")).second=org::esb::util::StringUtil::toString(acodec->id);
  boost::shared_ptr<Encoder> videoEncoder2 = boost::shared_ptr<Encoder>(new Encoder(clist["video"]["codec_id"]));//CodecFactory::getStreamEncoder(clist["video"]);
  if(!videoEncoder2->open())
    exit(1);
  boost::shared_ptr<Encoder> audioEncoder2 = boost::shared_ptr<Encoder>(new Encoder(clist["audio"]["codec_id"]));//CodecFactory::getStreamEncoder(clist["audio"]);
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
       stream_map[a]=0;
      }
      if(decoder[a]->getCodecType()==AVMEDIA_TYPE_AUDIO){
        encoder[a]=audioEncoder2;
        have_audio=true;
        stream_map[a]=1;
      }
      converter[a]=new FrameConverter(decoder[a].get(),encoder[a].get());
      //stream_map[a]=i++;
      LOGDEBUG("Decoder:"<<decoder[a]->toString());
      LOGDEBUG("Encoder:"<<encoder[a]->toString());
      /*only encoding the first audio and video stream here*/
      if(have_video&&have_audio)break;
    }
  }
  //return 0;
  File outfile(videofile.getFileName());
  PresetReaderJson::Preset preset=pr.getPreset();
  outfile.changeExtension("mp4");
  LOGDEBUG("writing to outfile "<<outfile.getPath());
  FormatOutputStream fos(&outfile);
  PacketOutputStream pos(&fos);
  pos.setEncoder(*videoEncoder2.get(),0);
  pos.setEncoder(*audioEncoder2.get(),1);

  if(!pos.init()){
    exit(0);
  }
  videoEncoder2->setOutputStream(&pos);
  audioEncoder2->setOutputStream(&pos);
  int c=0;
  while (pis.readPacket(p)==0/*&&c++<5000*/) {
    Packet * encodedPacket = convert(p);
  }
  pos.close();
  fos.close();
  encoder.clear();
  decoder.clear();

/*
  std::list<boost::shared_ptr<Packet> > packets=sink.getList();
  std::list<boost::shared_ptr<Packet> >::iterator it=packets.begin();
  for(;it!=packets.end();it++){
    pos.writePacket(*(*it).get());
  }*/


  return 0;
}

int check(int argc, char**argv){

  /*loading the preset*/
  File preset_file(argv[2]);
  if(!preset_file.exists()){
    LOGERROR("could not find preset file "<<preset_file.getFilePath());
  }
  FileInputStream pfis(&preset_file);

  std::string preset_data;
  pfis.read(preset_data);
  PresetReaderJson pr(preset_data);
  PresetReaderJson::CodecList clist = pr.getCodecList();
  PresetReaderJson::CodecList::iterator cit=clist.begin();
  for(;cit!=clist.end();cit++){
    std::string codec_name=(*(*cit).second.find("codec_id")).second;
    LOGDEBUG("try resolving codec for "<<(*cit).first <<" ->"<<codec_name);
    AVCodec * vcodec=avcodec_find_encoder_by_name(codec_name.c_str());
    if(!vcodec)
      LOGDEBUG("could not find codec by name");
    (*(*cit).second.find("codec_id")).second=org::esb::util::StringUtil::toString(vcodec->id);
    boost::shared_ptr<Encoder> encoder = boost::shared_ptr<Encoder>(new Encoder((*(*cit).second.find("codec_id")).second));//CodecFactory::getStreamEncoder((*cit).second);
    //encoder->ctx->crf=0.0;
    //encoder->setFlag(CODEC_FLAG_PASS2);
    if(!encoder->open())
      exit(1);
    std::map<std::string, std::string> options=encoder->getCodecOptions();
    std::map<std::string, std::string>::iterator oit=options.begin();
    for(;oit!=options.end();oit++){
      std::cout << (*oit).first<<"="<<(*oit).second<<std::endl;
    }
  }
  return 0;
}

int main(int argc, char** argv) {
  Log::open();
  FormatBaseStream::initialize();
  if(argc>1&&strcmp(argv[1],"check")==0){
    return check(argc, argv);
  }
  return encode(argc,argv);

}


