/* 
 * File:   TestDecoder.cpp
 * Author: jhoelscher
 *
 * Created on 26. Januar 2010, 09:00
 */

#include <stdlib.h>
#include <list>
#include "org/esb/io/File.h"
#include "org/esb/av/Packet.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"

#include "org/esb/util/Log.h"
/*
 * 
 */
int main(int argc, char** argv) {
  Log::open("");
  std::string filepath=MEC_SOURCE_DIR;
  filepath.append("/test.dvd");
  org::esb::io::File file(filepath.c_str());
  org::esb::av::FormatInputStream fis(&file);
  org::esb::av::PacketInputStream pis(&fis);
  org::esb::av::Packet * p;
  std::list<org::esb::av::Packet*> packetlist;
  for(int i=0;i<10;){
    if((p=pis.readPacket())!=NULL&&p->getStreamIndex()==0){
      packetlist.push_back(p);
      i++;
    }
  }


  Log::close();
  return (EXIT_SUCCESS);
}

