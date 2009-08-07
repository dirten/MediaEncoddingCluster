#include "org/esb/av/Packet.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/io/ObjectOutputStream.h"


using namespace org::esb::av;
using namespace org::esb::io;
int main(){
  Packet p(5);
  p.setPts(1000);
  p.setDts(2000);
  p.setFlags(5);
  p.setStreamIndex(2);
  p.setDuration(3600);

  memcpy(p.packet->data,"12345",5);
  
  assert(strcmp((const char*)p.getData(),"12345")==0);
  assert(p.getSize()==5);
  assert(p.getPts()==1000);
  assert(p.getDts()==2000);
  assert(p.getFlags()==5);
  assert(p.getStreamIndex()==2);
  assert(p.getDuration()==3600);

  FileOutputStream fos("test.packet");
  ObjectOutputStream oos(&fos);
  oos.writeObject(p);
}

