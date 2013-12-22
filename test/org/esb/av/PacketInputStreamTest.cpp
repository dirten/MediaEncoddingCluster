

#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/Packet.h"

using org::esb::av::FormatInputStream;
using org::esb::av::PacketInputStream;
using org::esb::av::Packet;

int main(int argc, char**argv){
  std::string filename(argv[1]);
  FormatInputStream fis(filename);
  PacketInputStream pis(&fis);
  Packet * packet;
  while(packet=pis.readPacket()){

  }
  return 0;
}
