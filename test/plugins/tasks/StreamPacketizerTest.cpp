/* 
 * File:   TestPacketizer.cpp
 * Author: jhoelscher
 *
 * Created on 28. Oktober 2009, 11:09
 */

#include <stdlib.h>

#include <string>
#include "org/esb/config/config.h"
#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"

#include "org/esb/av/FrameConverter.h"
#include "org/esb/util/Log.h"
#include "org/esb/util/StringUtil.h"
#include "org/esb/av/PGMUtil.h"
#include "org/esb/av/Frame.h"
#include "plugins/tasks/encodingtask/StreamPacketizer.h"

using namespace org::esb::av;
using namespace org::esb::io;
using namespace encodingtask;
//using namespace org::esb::hive::job;
using org::esb::util::StringUtil;
/*
 * 
 */
int i = 0;

void decode_packet_list(PacketListPtr list, Ptr<Decoder> dec) {
  dec->reset();
  PacketListPtr::iterator it = list.begin();
  int a=0;
  char * outfile = new char[100];
  for (; it != list.end(); it++) {
    LOGDEBUG("frame count:"<<++a);
    dec->newPacket((*it));
  }
  while(dec->newPacket(new Packet()));
}

void print_packet_list(PacketListPtr list) {
  PacketListPtr::iterator it = list.begin();
  int a=0;
  for (; it != list.end(); it++) {
    LOGDEBUG(StringUtil::toString(++a)<<" : "<<(*it)->toString());
  }
}

int main(int argc, char** argv) {
  org::esb::config::Config::init();
  Log::open("");
  std::map<int, StreamData> stream_data;

  /*open the fixed test File or the file from command line input*/
  std::string src;
  std::string trg;
  if (argc == 1) {
    src = MEC_SOURCE_DIR;
    //src.append("/target/dependency/fixtures/mpeg2_mp2.ts");
        src.append("/test-data/test.dvd");
  } else {
    src = argv[1];
  }


  /*opening the input file and Packet Input Stream*/
  File f(src.c_str());
  FormatInputStream fis(&f);
  PacketInputStream pis(&fis);
  //return 0;


  /*get input format parameter*/
  int c = fis.getStreamCount();
  int s = 0;

  for (int i = 0; i < c; i++) {
    if (fis.getStreamInfo(i)->getCodecType() != AVMEDIA_TYPE_VIDEO &&
            fis.getStreamInfo(i)->getCodecType() != AVMEDIA_TYPE_AUDIO) continue;
    stream_data[i].decoder = boost::shared_ptr<Decoder > (new Decoder(fis.getAVStream(i)));
    stream_data[i].decoder->open();
    stream_data[i].min_packet_count = 5;
  }
  std::map<int,StreamPacketizer> packetizer;
  //StreamPacketizer pti(stream_data);
  for (int a = 0; a < 150;) {
    Packet p;
    //reading a packet from the Stream
    //when no more packets available(EOF) then it return <0
    if (pis.readPacket(p) < 0)break;
    if (stream_data.find(p.getStreamIndex()) == stream_data.end())continue;
    if (stream_data[p.getStreamIndex()].decoder->getCodecType() != AVMEDIA_TYPE_VIDEO)continue;
    if(!packetizer.count(p.getStreamIndex())){

      Decoder * dec=new Decoder(fis.getAVStream(p.getStreamIndex()));
      dec->open();

      packetizer[p.getStreamIndex()]=StreamPacketizer(10,Ptr<Decoder>(dec));
    }
    StreamPacketizer & pti=packetizer[p.getStreamIndex()];

    //if(p.getDts()<3760262033)continue;
    org::esb::av::PacketPtr pPacket(new Packet(p));
    if (pti.putPacket(pPacket)) {
      PacketListPtr list = pti.removePacketList();

      LOGDEBUG("--------------------------------------------------------------------------------------------");
      print_packet_list(list);
      pti.getDecoder()->reset();
      decode_packet_list(list, pti.getDecoder());
      pti.getDecoder()->reset();
      LOGDEBUG("finish decoding test--continue with next packets------------------------------------------------------------------------------------------")
    }
    a++;
  }
  LOGDEBUG("Flush Packetitzer");
  /*pti.flushStreams();
  PacketListPtr list;
  list = pti.removePacketList();
  LOGDEBUG("--------------------------------------------------------------------------------------------");
  print_packet_list(list);
  return 0;
  //  assert(list.size() == 27);
  //  assert(list.front()->isKeyFrame() == true);

  list = pti.removePacketList();
  LOGDEBUG("--------------------------------------------------------------------------------------------");
  print_packet_list(list);
  //  assert(list.size() == 50);


  list = pti.removePacketList();
  LOGDEBUG("--------------------------------------------------------------------------------------------");
  print_packet_list(list);
  //  assert(list.size() == 27);
  //  assert(list.front()->isKeyFrame() == true);

  list = pti.removePacketList();
  LOGDEBUG("--------------------------------------------------------------------------------------------");
  print_packet_list(list);
  //  assert(list.size() == 50);

  list = pti.removePacketList();
  LOGDEBUG("--------------------------------------------------------------------------------------------");
  print_packet_list(list);
  assert(list.size() == 27);
  assert(list.front()->isKeyFrame() == true);


  list = pti.removePacketList();

  LOGDEBUG("--------------------------------------------------------------------------------------------");
  print_packet_list(list);
  assert(list.size() == 12);
  assert(list.front()->isKeyFrame() == true);

  list = pti.removePacketList();
  LOGDEBUG("--------------------------------------------------------------------------------------------");
  print_packet_list(list);
  //  assert(list.size() == 15);
*/
  Log::close();

  return (EXIT_SUCCESS);
}

