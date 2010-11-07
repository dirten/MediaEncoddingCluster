#include "org/esb/av/Packet.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/io/ObjectInputStream.h"


using namespace org::esb::av;
using namespace org::esb::io;

int main() {
  {
  FileInputStream fis("test.packet");
  ObjectInputStream ois(&fis);
  Packet p;
  ois.readObject(p);

  assert(strcmp((const char*)p.getData(),"12345")==0);
  assert(p.getSize()==5);
  assert(p.getPts()==1000);
  assert(p.getDts()==2000);
  assert(p.getFlags()==5);
  assert(p.getStreamIndex()==2);
  assert(p.getDuration()==3600);
  assert(p.getPtsTimeStamp()==TimeStamp(1, Rational(1,25)));
  assert(p.getDtsTimeStamp()==TimeStamp(3, Rational(1,25)));
  assert(p.getTimeDuration()==TimeStamp(5, Rational(3,25)));
  }
  if(false){
  FileInputStream fis("../../../../../test_data/packet-1-1032.pkt");
  ObjectInputStream ois(&fis);
  Packet p;
  ois.readObject(p);
  p.toString();
  }
}
