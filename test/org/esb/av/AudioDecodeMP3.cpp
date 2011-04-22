/* 
 * File:   AudioDecodeMP3.cpp
 * Author: HoelscJ
 *
 * Created on 13. Juli 2010, 13:46
 */

#include <cstdlib>


#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/Decoder.h"
#include "org/esb/av/Frame.h"
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
  std::string filepath;
  if (argc < 2) {
    filepath.append(MEC_SOURCE_DIR);
    filepath.append("/test.dvd");
  } else {
    filepath.append("/");
    filepath.append(argv[1]);
  }

  File file(filepath);
  FormatInputStream fis(&file);
  int c = fis.getStreamCount();
  int audio_stream = -1;
  for (int a = 0; a < c; a++) {
    if (fis.getStreamInfo(a)->getCodecType() == AVMEDIA_TYPE_AUDIO) {
      audio_stream = a;
      break;
    }
  }
  Decoder dec(fis.getAVStream(audio_stream));
  //  dec.setTimeBase(1, 25);
  dec.open();
  LOGDEBUG(dec.toString());


  org::esb::av::PacketInputStream pis(&fis);
  org::esb::av::Packet * p;

  for (int i = 0; i < 200;) {
    if ((p = pis.readPacket()) != NULL && p->getStreamIndex() == audio_stream) {
      org::esb::av::Frame * frame = dec.decode2(*p);
      if (frame->isFinished()) {
        LOGDEBUG(frame->toString());
      }
      delete frame;
      i++;
    }
    delete p;
  }

  return 0;
}

