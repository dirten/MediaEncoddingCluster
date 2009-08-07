#include "org/esb/av/FormatInputStream.h"
#include "org/esb/io/File.h"
#include "org/esb/util/Log.h"
using namespace org::esb::av;
using namespace org::esb::io;


int main(){

  File f("../../../../../test.dvd");
  FormatInputStream fis(&f);
  assert(fis.isValid()==true);
  assert(fis.getFileSize()==3409920);
  assert(fis.getStreamCount()==2);
  return 0;
}
