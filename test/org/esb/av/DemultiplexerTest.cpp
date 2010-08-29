/* 
 * File:   DemultiplexerTest.cpp
 * Author: jholscher
 *
 * Created on 29. August 2010, 20:08
 */

#include <cstdlib>
#include "org/esb/av/MediaLocator.h"
#include "org/esb/av/DataSource.h"
#include "org/esb/av/Demultiplexer.h"

#include "org/esb/util/Log.h"
using namespace std;
using namespace org::esb::av;
/*
 * 
 */
int main(int argc, char** argv) {
  Log::open("");
  std::string basepath = MEC_SOURCE_DIR;
  std::string path="file://";
  MediaLocator locator(path+basepath+"/test.dvd");
  DataSource source(locator);
  source.connect();
  Demultiplexer plex(source);
  cout << "Duration:"<<plex.getDuration().toString()<<endl;
  cout << "Duration:"<<plex.getMediaTime().toString()<<endl;
  assert(plex.getDuration().getTime()==79080000);
  assert(plex.getMediaTime().getTime()==0);

  Log::close();
  return 0;
}

