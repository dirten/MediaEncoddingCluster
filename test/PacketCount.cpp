/* 
 * File:   PacketCount.cpp
 * Author: jhoelscher
 *
 * Created on 11. November 2009, 11:01
 */

#include <stdlib.h>

#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/Packet.h"

using namespace org::esb::io;
using namespace org::esb::av;
/*
 * 
 */

int main(int argc, char** argv) {

  File f(argv[1]);
  int str_idx=atoi(argv[2]);

  FormatInputStream fis(&f);
  PacketInputStream pis(&fis);

  int64_t count=0;
  int64_t start_dts=0;
  int64_t start_pts=0;
  int64_t end_dts=0;
  int64_t end_pts=0;
  int64_t all_size=0;
  int64_t key_frames=0;
  Packet p;

  while(pis.readPacket(p)>=0){
    if(p.getStreamIndex()==str_idx){
      if(start_dts==0){
        start_dts=p.packet->dts;
      }
      if(start_pts==0){
        start_pts=p.packet->pts;
      }
      if(p.isKeyFrame()){
        key_frames++;
      }
      end_dts=p.packet->dts;
      end_pts=p.packet->pts;
      all_size+=p.packet->size;
      count++;
    }
  }
  printf("Stream # %d have %lld packets\n",str_idx, count);
  printf("Stream # %d have as start PTS %lld \n",str_idx, start_pts);
  printf("Stream # %d have as start DTS %lld \n",str_idx, start_dts);
  printf("Stream # %d have as end PTS %lld \n",str_idx, end_pts);
  printf("Stream # %d have as end DTS %lld \n",str_idx, end_dts);
  printf("Stream # %d have %lld Bytes\n",str_idx, all_size);
  printf("Stream # %d have %lld Key Frames\n",str_idx, key_frames);
  return (EXIT_SUCCESS);
}

