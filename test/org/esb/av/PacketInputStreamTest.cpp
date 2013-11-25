

#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"

using org::esb::av::FormatInputStream;
using org::esb::av::PacketInputStream;

int main(int argc, char**argv){
  std::string filename(argv[1]);
  FormatInputStream fis(filename);

  return 0;
}
