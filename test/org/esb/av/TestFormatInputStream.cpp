#include "org/esb/av/FormatInputStream.h"
#include "org/esb/io/File.h"
#include "org/esb/util/Log.h"
using namespace org::esb::av;
using namespace org::esb::io;

int main() {
  Log::open();
  std::string src = MEC_SOURCE_DIR;
  src.append("/test.dvd");
  File f(src.c_str());
  FormatInputStream fis(&f);
  assert(fis.isValid() == true);
  LOGDEBUG(fis.getFileSize());
  assert(fis.getFileSize() == 3409920);
  assert(fis.getStreamCount() == 2);
  return 0;
}
