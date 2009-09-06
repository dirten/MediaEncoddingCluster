#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/io/File.h"

using namespace org::esb::av;
using namespace org::esb::io;


int main(){
  std::string src = MEC_SOURCE_DIR;
	src.append("/test.dvd");
  File f(src.c_str());
  FormatInputStream fis(&f);
  PacketInputStream pis(&fis);
  Packet p;
  for(int a=0;a<100;a++){
    if(pis.readPacket(p)){
      
    }
  }
  for(int a=0;a<100;a++){
    Packet p2;
    if(pis.readPacket(p2)){
      
    }
  }
}
