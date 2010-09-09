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
#include "org/esb/av/BMPUtil.h"
#include "org/esb/av/Decoder.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"

#include "org/esb/util/Log.h"

org::esb::av::Frame * getRawFrame(){
  int height=240;
  int width=320;
  int i=0;
  org::esb::av::Frame * frame =new org::esb::av::Frame(PIX_FMT_YUV422P, width, height);
  /* prepare a dummy image */
        /* Y */
        for(int y=0;y<height;y++) {
            for(int x=0;x<width;x++) {
                frame->getAVFrame()->data[0][y * frame->getAVFrame()->linesize[0] + x] = x + y + i * 3;
            }
        }

        /* Cb and Cr */
        for(int y=0;y<height/2;y++) {
            for(int x=0;x<width/2;x++) {
                frame->getAVFrame()->data[1][y * frame->getAVFrame()->linesize[1] + x] = 128 + y + i * 2;
                frame->getAVFrame()->data[2][y * frame->getAVFrame()->linesize[2] + x] = 64 + x + i * 5;
            }
        }
  return frame;
}

void testDecodeRawVideo(){
  org::esb::av::Decoder dec(CODEC_ID_FFV1);
  dec.setTimeBase(1, 25);
  dec.open();

}

void testDecodeVideo(std::string filepath) {
  int offset = 0;
  int printout = 0;
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
  LOGDEBUG(dec.getPixelFormat());
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
          assert(frame->_pixFormat==PIX_FMT_YUV444P);
//          sprintf(outfile,"test-%i.bmp", i);
//          org::esb::av::BMPUtil::save("test-%d.bmp", frame);
//          frame->dumpHex();
        }
        delete frame;
      }
      i++;
    }
    delete p;
  }
  delete [] outfile;
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
  std::string filename=MEC_SOURCE_DIR;
  filename.append("/test.dvd");
  if(argc>1){
    filename=argv[1];
  }
//  testDecodeRawVideo();
  testDecodeVideo(filename);
  testDecodeAudio();
  Log::close();
  return (EXIT_SUCCESS);

}

