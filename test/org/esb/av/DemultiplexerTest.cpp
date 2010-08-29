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
#include "boost/thread.hpp"
#include "org/esb/util/Log.h"
#include "org/esb/lang/Thread.h"
using namespace std;
using namespace org::esb::av;
using namespace org::esb::lang;
void readTrack(Ptr<Track> track){
  while(true){
  Buffer b;
  track->readFrame(b);
  cout << "Buffer readed: "<<b.getAVPacket()->stream_index<<endl;
  cout << "Buffer readed: "<<b.getSequenceNumber()<<endl;
  }
}
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
  list<Ptr<Track> > track_list=plex.getTracks();
  assert(track_list.size()==2);
  list<Ptr<Track> >::iterator it=track_list.begin();
  for(;it!=track_list.end();it++){
 //   boost::thread t(readTrack,(*it));
  }
//  Thread::sleep2(10000);
  Log::close();
  return 0;
}

