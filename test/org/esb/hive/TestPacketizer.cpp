/* 
 * File:   TestPacketizer.cpp
 * Author: jhoelscher
 *
 * Created on 28. Oktober 2009, 11:09
 */

#include <stdlib.h>

#include <string>

#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/hive/job/Packetizer.h"
#include "org/esb/av/FrameConverter.h"
#include "org/esb/util/Log.h"
#include "org/esb/av/PGMUtil.h"
#include "org/esb/av/Frame.h"
using namespace org::esb::av;
using namespace org::esb::io;
using namespace org::esb::hive::job;

/*
 * 
 */
int i = 0;

void decode_packet_list(PacketListPtr list, std::map<int, Packetizer::StreamData> & stream_data) {
  PacketListPtr::iterator it = list.begin();
  char * outfile = new char[100];

  for (; it != list.end(); it++) {
    Frame * frame = stream_data[(*it)->getStreamIndex()].decoder->decodeVideo2(*(*it).get());
    if(frame->isFinished()){
      sprintf(outfile, "test-%i-%i.pgm", ++i, (*it)->getDts());
      PGMUtil::save(outfile, frame);
    }
  }

}

void print_packet_list(PacketListPtr list) {
  PacketListPtr::iterator it = list.begin();
  for (; it != list.end(); it++) {
    LOGDEBUG((*it)->toString());
  }
}

int main(int argc, char** argv) {
  Log::open("");
  std::map<int, Packetizer::StreamData> stream_data;

  /*open the fixed test File or the file from command line input*/
  std::string src;
  std::string trg;
  if (argc == 1) {
    src = MEC_SOURCE_DIR;
    src.append("/test/fixtures/dvd.mpeg");
    //    src.append("/test.dvd");
  } else {
    src = argv[1];
  }


  /*opening the input file and Packet Input Stream*/
  File f(src.c_str());
  FormatInputStream fis(&f);
  PacketInputStream pis(&fis);



  /*get input format parameter*/
  int c = fis.getStreamCount();
  int s = 0;

  for (int i = 0; i < c; i++) {
    if (fis.getStreamInfo(i)->getCodecType() != CODEC_TYPE_VIDEO &&
            fis.getStreamInfo(i)->getCodecType() != CODEC_TYPE_AUDIO) continue;
    stream_data[i].decoder = boost::shared_ptr<Decoder > (new Decoder(fis.getAVStream(i)));
    stream_data[i].decoder->open();
    stream_data[i].min_packet_count = 5;
  }

  Packetizer pti(stream_data);
  for (int a = 0; a < 800; a++) {
    Packet p;
    //reading a packet from the Stream
    //when no more packets available(EOF) then it return <0
    if (pis.readPacket(p) < 0)break;
    if (stream_data.find(p.getStreamIndex()) == stream_data.end())continue;
    if (stream_data[p.getStreamIndex()].decoder->getCodecType() != CODEC_TYPE_VIDEO)continue;
    boost::shared_ptr<Packet> pPacket(new Packet(p));
    if (pti.putPacket(pPacket)) {
      PacketListPtr list = pti.removePacketList();

      LOGDEBUG("--------------------------------------------------------------------------------------------");
      print_packet_list(list);
      decode_packet_list(list, stream_data);
    }
  }
  LOGDEBUG("Flush Packetitzer");
  pti.flushStreams();
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

  Log::close();

  return (EXIT_SUCCESS);
}

