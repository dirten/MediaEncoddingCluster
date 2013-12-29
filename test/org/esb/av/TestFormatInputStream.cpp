#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"

#include "org/esb/io/File.h"
#include "org/esb/util/Log.h"
using namespace org::esb::av;
using namespace org::esb::io;

void test_stream(std::string & filename){
  std::ifstream stream(filename.c_str(), std::ifstream::in);
  FormatInputStream fis(stream);
  fis.dumpFormat();
  PacketInputStream pis(&fis);
  Packet p;
  for(int a = 0; a<10 ;a++){
    int s=pis.readPacket(p);
    LOGDEBUG("Status:"<<s);
    LOGDEBUG("Packet:"<<p.toString());
  }
}

int main(int argc, char ** argv) {
  Log::open();
  std::string src = MEC_SOURCE_DIR;
  src.append("/test-data/test.dvd");
  if(argc>1){
    src=argv[1];
  }
  File f(src.c_str());
  FormatInputStream fis(&f);
  assert(fis.isValid() == true);
  LOGDEBUG(fis.getStreamCount());
  //assert(fis.getFileSize() == 3409920);
  //assert(fis.getStreamCount() == 3);
  test_stream(src);

  return 0;
}
