/* 
 * File:   CCodecTest.cpp
 * Author: HoelscJ
 *
 * Created on 30. August 2010, 13:27
 */

#include <cstdlib>
#include "org/esb/util/Log.h"
#include "org/esb/av/MediaLocator.h"
#include "org/esb/av/DataSource.h"
#include "org/esb/av/CCodec.h"
#include "org/esb/av/VideoFormat.h"
#include "org/esb/av/AudioFormat.h"
#include "org/esb/av/Track.h"
#include "org/esb/av/Demultiplexer.h"
using namespace std;
using namespace org::esb::av;

/*
 * 
 */
int main(int argc, char** argv) {


  Log::open("");
  std::string file;
  if (argc == 1) {
    std::string basepath = MEC_SOURCE_DIR;
    std::string path = "file://";
    file = path + basepath + "/test.dvd";
  } else {
    file = argv[1];
  }
  MediaLocator locator(file);
  DataSource source(locator);
  source.connect();
  Demultiplexer plex(source);

  Ptr<Track> video_track = plex.getTracks().front();

  VideoFormat out(PIX_FMT_RGB32, 320, 240, Rational(25,1));

  CCodec codec;
  VideoFormat in=video_track->getFormat();
//  VideoFormat test=in;
  
  in.setFormatOption("test", "test");
  codec.setInputFormat(in);
  codec.setOutputFormat(out);
  codec.open();
  return 0;
}

