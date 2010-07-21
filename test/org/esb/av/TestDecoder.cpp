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
#include "org/esb/av/PGMUtil.h"
#include "org/esb/av/Decoder.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"

#include "org/esb/util/Log.h"

void testDecodeVideo() {
  int offset = 0;
  int printout = 0;
  std::string filepath = MEC_SOURCE_DIR;
//  filepath.append("/test/fixtures/TITLE01-ANGLE1.VOB");
  filepath.append("/test.dvd");
//filepath="/home/HoelscJ/old/media/video/ChocolateFactory.ts";
  org::esb::io::File file(filepath.c_str());
  org::esb::av::FormatInputStream fis(&file);
  int c = fis.getStreamCount();
  int video_stream = -1;
  for (int a = 0; a < c; a++) {
    if (fis.getStreamInfo(a)->getCodecType() == CODEC_TYPE_VIDEO) {
      video_stream = a;
      break;
    }
  }
  org::esb::av::Decoder dec(fis.getAVStream(video_stream));
  //  dec.setTimeBase(1, 25);
  dec.open();
  LOGDEBUG(dec.toString());


  org::esb::av::PacketInputStream pis(&fis);
  org::esb::av::Packet * p;
  std::list<org::esb::av::Packet*> packetlist;
  int incount = 0;
  int outcount = 0;
  char * outfile=new char[100];
  memset(outfile,0,100);
  for (int i = 0; i < 200;) {
    if ((p = pis.readPacket()) != NULL && p->getStreamIndex() == video_stream) {
      if (i >= offset) {
        org::esb::av::Frame * frame = dec.decode2(*p);
        incount++;
        if (frame->isFinished() && i >= printout) {
          LOGDEBUG("frame finished!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
          outcount++;
          sprintf(outfile,"test-%i.pgm", i);
          org::esb::av::PGMUtil::save(outfile, frame);
//          frame->dumpHex();
        }
        delete frame;
      }
      i++;
    }
    delete p;
  }
  LOGDEBUG("decoding delay")
  org::esb::av::Packet * ptmp = new org::esb::av::Packet();
  ptmp->setDuration(3600);
  ptmp->setTimeBase(1, 90000);
  bool decode_last = true;
  while (decode_last) {
    org::esb::av::Frame * frame = dec.decode2(*ptmp);
    if (frame->isFinished()) {
          LOGDEBUG("frame finished!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
      //      LOGDEBUG("org.esb.av.TestDecoder", frame->toString());
      outcount++;
    } else {
      decode_last = false;
    }
    delete frame;
  }
  LOGDEBUG("incount=" << incount << " outcount=" << outcount);
  delete ptmp;
}

/*
 * 
 */
void testDecodeAudio(){
  std::string filepath = MEC_SOURCE_DIR;
    filepath.append("/test.dvd");
  org::esb::io::File file(filepath.c_str());
  org::esb::av::FormatInputStream fis(&file);
  int offset = 0;
 
  /**
   * retriving the first video stream
   */
  int c = fis.getStreamCount();
  int video_stream = -1;
  for (int a = 0; a < c; a++) {
    if (fis.getStreamInfo(a)->getCodecType() == CODEC_TYPE_AUDIO) {
      video_stream = a;
      break;
    }
  }
  org::esb::av::Decoder dec(fis.getAVStream(video_stream));
  dec.setTimeBase(1, 25);
  dec.open();
  LOGDEBUG(dec.toString());
  org::esb::av::PacketInputStream pis(&fis);
  org::esb::av::Packet * p;
  std::list<org::esb::av::Packet*> packetlist;
  int incount = 0;
  int outcount = 0;
  for (int i = 0; i < 25;) {
    if ((p = pis.readPacket()) != NULL && p->getStreamIndex() == video_stream) {
      if (i >= offset) {
        org::esb::av::Frame * frame = dec.decode2(*p);
        incount++;
        if (frame->isFinished() ) {
          LOGDEBUG("frame finished!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

          outcount++;
//          frame->dumpHex();
        }
        delete frame;
      }
      i++;
    }
      delete p;
  }

  org::esb::av::Packet * ptmp = new org::esb::av::Packet();
  ptmp->setDuration(3600);
  ptmp->setTimeBase(1, 90000);
  bool decode_last = true;
  while (decode_last) {
    org::esb::av::Frame * frame = dec.decode2(*ptmp);
    if (frame->isFinished()) {
      //      LOGDEBUG("org.esb.av.TestDecoder", frame->toString());
      outcount++;
    } else {
      decode_last = false;
    }
    delete frame;
  }
  LOGDEBUG("incount=" << incount << " outcount=" << outcount);
  delete ptmp;
}

int main(int argc, char** argv) {
  Log::open("");
  testDecodeVideo();
//  testDecodeAudio();
  Log::close();
  return (EXIT_SUCCESS);

}

