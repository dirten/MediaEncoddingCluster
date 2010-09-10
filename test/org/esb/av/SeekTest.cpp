/* 
 * File:   SeekTest.cpp
 * Author: HoelscJ
 *
 * Created on 9. September 2010, 12:48
 */

#include <cstdlib>

#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/Packet.h"

#include "org/esb/io/File.h"
#include "org/esb/util/Log.h"
using namespace std;
using namespace org::esb::av;
using namespace org::esb::io;
/*
 * 
 */
int main(int argc, char** argv) {
  Log::open("");
  std::string base_path=MEC_SOURCE_DIR;
  std::string filename=base_path+"/test.dvd";
  if(argc>1){
    filename=argv[1];
  }
  int sidx=0;

  if(argc>2){
    sidx=atoi(argv[2]);
  }
  int64_t ts=0;
  if(argc>3){
    char * end;
    ts=strtoul(argv[3],&end,10);
  }

  File f(filename);
  FormatInputStream fis(&f);
  PacketInputStream pis(&fis);
  Packet * p=NULL;
  for(int a=0;a<150;){
    if((p=pis.readPacket())!=NULL&&p->getStreamIndex()==sidx){
      LOGDEBUG(p->toString());
      a++;
    }
  }

  LOGDEBUG("Seeking to "<<ts<<" on stream index "<<sidx);

  int t=fis.seek(sidx,TimeStamp(ts, Rational(fis.getStreamInfo(sidx)->getTimeBase())),AVSEEK_FLAG_ANY);
  LOGDEBUG("seek return:"<<t);
  for(int a=0;a<10;){
    if((p=pis.readPacket())!=NULL&&p->getStreamIndex()==sidx){
      LOGDEBUG(p->toString());
      a++;
    }
  }
  
  Log::close();
  return 0;
}

