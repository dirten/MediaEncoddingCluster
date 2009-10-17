#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/io/File.h"

using namespace org::esb::av;
using namespace org::esb::io;


int main(int argc, char **argv){
  std::string src;
  if(argc==1){
  src= MEC_SOURCE_DIR;
  src.append("/test.dvd");
  }else{
	src=argv[1];
  }
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
