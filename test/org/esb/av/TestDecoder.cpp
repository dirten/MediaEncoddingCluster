/* 
 * File:   TestDecoder.cpp
 * Author: jhoelscher
 *
 * Created on 26. Januar 2010, 09:00
 */

#include <stdlib.h>
#include <list>
#include "org/esb/io/File.h"
#include "org/esb/av/Packet.h"
#include "org/esb/av/Frame.h"
#include "org/esb/av/Decoder.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"

#include "org/esb/util/Log.h"

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open("");
  std::string filepath = MEC_SOURCE_DIR;
  if (argc > 1) {
    filepath = argv[1];
  } else
    filepath.append("/test.dvd");
  org::esb::io::File file(filepath.c_str());
  org::esb::av::FormatInputStream fis(&file);

  /**
   * retriving the first video stream
   */
  int c = fis.getStreamCount();
  int video_stream = -1;
  for (int a = 0; a < c; a++) {
    if (fis.getStreamInfo(a)->getCodecType() == CODEC_TYPE_VIDEO) {
      video_stream = a;
      break;
    }
  }
  org::esb::av::Decoder dec(fis.getStreamInfo(video_stream)->getCodec());
  dec.open();
  LOGDEBUG("org.esb.av.TestDecoder",dec.toString());
  org::esb::av::PacketInputStream pis(&fis);
  org::esb::av::Packet * p;
  std::list<org::esb::av::Packet*> packetlist;
  int incount=0;
  int outcount=0;
  for (int i = 0; i < 14;) {
    if ((p = pis.readPacket()) != NULL && p->getStreamIndex() == video_stream) {
      org::esb::av::Frame * frame = dec.decode2(*p);
      incount++;
      if (frame->isFinished()){
        LOGDEBUG("org.esb.av.TestDecoder", "frame finished!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        outcount++;

      }
      i++;
    }
  }
  org::esb::av::Packet * ptmp = new org::esb::av::Packet();
  ptmp->setDuration(3600);
  ptmp->setTimeBase(1, 90000);
  org::esb::av::Frame * frame = dec.decode2(*ptmp);
  if (frame->isFinished()){
    LOGDEBUG("org.esb.av.TestDecoder", frame->toString());
    outcount++;
  }
  frame = dec.decode2(*ptmp);
  if (frame->isFinished()){
    LOGDEBUG("org.esb.av.TestDecoder", frame->toString());
    outcount++;
  }
  frame = dec.decode2(*ptmp);
  if (frame->isFinished()){
    LOGDEBUG("org.esb.av.TestDecoder", frame->toString());
    outcount++;
  }
  LOGDEBUG("org.esb.av.TestDecoder","incount="<<incount<<" outcount="<<outcount);

  Log::close();
  return (EXIT_SUCCESS);
}

