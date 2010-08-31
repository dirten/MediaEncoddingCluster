/* 
 * File:   BufferTest.cpp
 * Author: jholscher
 *
 * Created on 29. August 2010, 18:06
 */

#include <cstdlib>
#include <valarray>
#include "org/esb/av/Buffer.h"
#include "org/esb/av/VideoFormat.h"
#include "org/esb/av/AudioFormat.h"
#include "org/esb/util/Log.h"
using namespace std;
using namespace org::esb::av;
/*
 * 
 */
int main(int argc, char** argv) {
  Log::open("");
  Buffer b;
  
  b.setAVPacket(Ptr<AVPacket>(new AVPacket()));
  b.setData(new uint8_t[10]);
  b.setDiscard(false);
  b.setDuration(Duration(100,Rational()));
  b.setFlags(1);
  b.setFormat(VideoFormat(PIX_FMT_RGB32, 320,240,Rational(25,1)));
  b.setLength(10000);
  b.setSequenceNumber(1212);
  b.setTimeStamp(TimeStamp(12000));

  assert(b.getAVPacket().get());
  assert(b.getData());
  assert(b.isDiscard()==false);
  assert(b.getDuration()==Duration(100));
  assert(b.getFlags()==1);
  assert(b.getLength()==10000);
  assert(b.getSequenceNumber()==1212);
  assert(b.getTimeStamp()==TimeStamp(12000));

  const Format f=b.getFormat();
  if(!f.getType()==Format::FORMAT_VIDEO){
    assert(false);
  }
  VideoFormat vf=(VideoFormat)f;
  assert(vf==VideoFormat(PIX_FMT_RGB32, 320,240,Rational(25,1)));

  delete []b.getData();
  
  b.setFormat(AudioFormat(SAMPLE_FMT_S16,44100,8,2,1,1152));
  const Format f2=b.getFormat();
  if(!f2.getType()==Format::FORMAT_AUDIO){
    assert(false);
  }
  AudioFormat af=(AudioFormat)f2;
  assert(af==AudioFormat(SAMPLE_FMT_S16,44100,8,2,1,1152));
  return 0;
}

