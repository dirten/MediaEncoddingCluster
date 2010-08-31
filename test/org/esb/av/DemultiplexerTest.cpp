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

int thread_count=0;
void readTrack(Ptr<Track> track){
  thread_count++;
  while(true){
  Buffer b;
  track->readFrame(b);
//  cout << "Buffer readed: "<<b.getAVPacket()->stream_index<<endl;
//  cout << "Buffer readed: index="<<b.getAVPacket()->stream_index<<":seq="<<b.getSequenceNumber()<<endl;
  if(b.isEOM())
    break;
  }
  cout << "EOM reached, exiting stream "<<track->getStreamIndex()<<endl;
  thread_count--;
}
/*
 * 
 */
int main(int argc, char** argv) {
  Log::open("");
  std::string file;
  if(argc==1){
    std::string basepath = MEC_SOURCE_DIR;
    std::string path="file://";
    file=path+basepath+"/test.dvd";
  }else{
    file=argv[1];
  }
  MediaLocator locator(file);
  DataSource source(locator);
  source.connect();
  Demultiplexer plex(source);
  cout << "Duration:"<<plex.getDuration().toString()<<endl;
  cout << "Duration:"<<plex.getMediaTime().toString()<<endl;
  assert(plex.getDuration().getTime()>0);
  assert(plex.getMediaTime().getTime()==0);
  list<Ptr<Track> > track_list=plex.getTracks();
  assert(track_list.size()>=1);
  list<Ptr<Track> >::iterator it=track_list.begin();
  for(;it!=track_list.end();it++){
    boost::thread t(readTrack,(*it));
  }
  Thread::sleep2(1000);
  while(thread_count>0)
    Thread::sleep2(1000);
  Log::close();
  return 0;
}

