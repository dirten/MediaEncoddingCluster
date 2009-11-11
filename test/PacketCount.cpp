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
  Packet p;
  while(pis.readPacket(p)>=0){
    if(p.getStreamIndex()==str_idx)
      count++;
  }
  printf("Stream # %d have %lld packets\n",str_idx, count);
  return (EXIT_SUCCESS);
}

