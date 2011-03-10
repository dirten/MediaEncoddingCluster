/* 
 * File:   TestOwnFFMpegUrl.cpp
 * Author: HoelscJ
 *
 * Created on 9. März 2011, 12:32
 */

#include <cstdlib>
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/io/File.h"
#include "org/esb/util/Log.h"
#include "org/esb/config/config.h"
using namespace std;
using namespace org::esb::av;
/*
 * 
 */
int main(int argc, char** argv) {
  Log::open();
  std::string src = "test://";
  src.append("/home/jan.hoelscher/devel/mec/test.dvd");
  
  FormatInputStream fis(src);
/*
  assert(fis.isValid() == true);
  assert(fis.getFileSize() == 3409920);
  assert(fis.getStreamCount() == 2);
 */
  org::esb::config::Config::close();
  Log::close();
  return 0;
}

