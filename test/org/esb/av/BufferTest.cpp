/* 
 * File:   BufferTest.cpp
 * Author: jholscher
 *
 * Created on 29. August 2010, 18:06
 */

#include <cstdlib>
#include <valarray>
#include "org/esb/av/Buffer.h"
#include "org/esb/av/Format.h"
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
  b.setFormat(Format(Format::FORMAT_VIDEO));
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
  if(f.getType()!=Format::FORMAT_VIDEO){
    assert(false);
  }
//  const Format ft(CODEC_ID_CINEPAK);
//  assert(f==ft);

  delete []b.getData();
  
  b.setFormat(Format(Format::FORMAT_AUDIO));
  const Format f2=b.getFormat();
  if(f2.getType()!=Format::FORMAT_AUDIO){
    assert(false);
  }
//  assert(f==Format(CODEC_ID_AC3));
  return 0;
}

