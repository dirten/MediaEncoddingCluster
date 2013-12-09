#include "org/esb/av/FormatInputStream.h"
#include "org/esb/io/File.h"
#include "org/esb/util/Log.h"
using namespace org::esb::av;
using namespace org::esb::io;

int main() {
  Log::open();
  std::string src = MEC_SOURCE_DIR;
  src.append("/test-data/test.dvd");
  File f(src.c_str());
  FormatInputStream fis(&f);
  assert(fis.isValid() == true);
  LOGDEBUG(fis.getStreamCount());
  //assert(fis.getFileSize() == 3409920);
  assert(fis.getStreamCount() == 3);
  return 0;
}
