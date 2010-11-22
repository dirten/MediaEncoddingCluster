/* 
 * File:   ProcessUnitBuilderTest.cpp
 * Author: HoelscJ
 *
 * Created on 16. Juli 2010, 17:17
 */

#include <cstdlib>

#include "org/esb/hive/job/ProcessUnitBuilder.h"
#include "org/esb/hive/job/StreamData.h"
#include <map>
#include "org/esb/util/Log.h"
using namespace std;
using namespace org::esb::hive::job;

/*
 * 
 */
//classlogger("ProcessUnitBuilderTest");
void testAudioSameSamplerate() {
  int packet_count = 500;
  map<int, StreamData> data;
  data[0] = StreamData();
  data[0].decoder = boost::shared_ptr<Decoder > (new Decoder((CodecID) 86016));
  data[0].decoder->setSampleRate(48000);
  data[0].decoder->setChannels(2);
  data[0].decoder->open();
  data[0].encoder = boost::shared_ptr<Encoder > (new Encoder((CodecID) 86016));

  data[0].encoder->setSampleRate(48000);
  data[0].encoder->setChannels(2);
  data[0].encoder->setSampleFormat(SAMPLE_FMT_S16);
  data[0].encoder->open();
  data[0].instream=1;
  data[0].outstream=2;


  data[0].instream=1;
  data[0].outstream=2;

  ProcessUnitBuilder builder(data);

  PacketListPtr packetList;
  for (int a = 0; a < packet_count; a++) {
    boost::shared_ptr<Packet > p = boost::shared_ptr<Packet > (new Packet());
    p->setDuration(2160);
    p->setTimeBase(1,90000);
    packetList.push_back(p);
  }

  for (int a = 0; a < 1000; a++) {
    boost::shared_ptr<ProcessUnit> pu = builder.build(packetList);
    assert(pu->_source_stream==1);
    assert(pu->_target_stream==2);

    assert(pu->getDecoder());
    assert(pu->getDecoder()->getCodecId() == data[0].decoder->getCodecId());

    assert(pu->getEncoder());
    assert(pu->getEncoder()->getCodecId() == data[0].encoder->getCodecId());

    assert(pu->getInputPacketList().size() == packet_count);
    assert(pu->getGopSize() - packet_count<1);
    assert(pu->getExpectedFrameCount() - packet_count<1);
    /*this must be always 0, dont what what it is when cycle is over 100000(very very long video >270h)*/
    assert(fabs(data[0].frameRateCompensateBase - 0.00) < 0.00001);
  }
}

void testAudioResample48000to44100() {
  int packet_count = 500;

  map<int, float> comp_map;
  map<int, int> count_map;
  comp_map[0] = 2304.0;
  comp_map[1] = 0.0;
  comp_map[2] = 2304.0;
  comp_map[3] = 0.0;
  comp_map[4] = 2304.0;

  count_map[0] = 612;
  count_map[1] = 613;
  count_map[2] = 612;
  count_map[3] = 613;
  count_map[4] = 612;

  map<int, StreamData> data;
  data[0] = StreamData();
  data[0].decoder = boost::shared_ptr<Decoder > (new Decoder((CodecID) 86016));
  data[0].decoder->setSampleRate(48000);
  data[0].decoder->setChannels(2);
  data[0].decoder->open();
  data[0].encoder = boost::shared_ptr<Encoder > (new Encoder((CodecID) 86016));

  data[0].encoder->setSampleRate(44100);
  data[0].encoder->setChannels(2);
  data[0].encoder->setSampleFormat(SAMPLE_FMT_S16);
  data[0].encoder->open();
  data[0].instream=1;
  data[0].outstream=2;


  data[0].instream=1;
  data[0].outstream=2;

  ProcessUnitBuilder builder(data);

  PacketListPtr packetList;
  for (int a = 0; a < packet_count; a++) {
    boost::shared_ptr<Packet > p = boost::shared_ptr<Packet > (new Packet());
    p->setDuration(1792);
    p->setTimeBase(1,56000);
    packetList.push_back(p);
  }

  for (int a = 0; a < 5; a++) {
    boost::shared_ptr<ProcessUnit> pu = builder.build(packetList);
    assert(pu->_source_stream==1);
    assert(pu->_target_stream==2);

    assert(pu->getDecoder());
    assert(pu->getDecoder()->getCodecId() == data[0].decoder->getCodecId());

    assert(pu->getEncoder());
    assert(pu->getEncoder()->getCodecId() == data[0].encoder->getCodecId());

    assert(pu->getInputPacketList().size() == packet_count);
    assert(pu->getGopSize() - packet_count<1);
    assert(pu->getExpectedFrameCount() - count_map[a]<1);
    /*this must be always 0, dont what what it is when cycle is over 100000(very very long video >270h)*/
    assert(fabs(data[0].frameRateCompensateBase - comp_map[a]) < 0.00001);
  }
}
void testVideoSameFramerate() {
  int packet_count = 112;
  map<int, StreamData> data;
  data[0] = StreamData();
  data[0].decoder = boost::shared_ptr<Decoder > (new Decoder((CodecID) 13));
  data[0].decoder->setFrameRate(25, 1);

  data[0].encoder = boost::shared_ptr<Encoder > (new Encoder((CodecID) 13));

  data[0].encoder->setFrameRate(25, 1);
  data[0].instream=1;
  data[0].outstream=2;


  data[0].instream=1;
  data[0].outstream=2;

  ProcessUnitBuilder builder(data);

  PacketListPtr packetList;
  for (int a = 0; a < packet_count; a++) {
    boost::shared_ptr<Packet > p = boost::shared_ptr<Packet > (new Packet());
    if (a % 10 == 0)
      p->setKeyPacket(true);
    packetList.push_back(p);
  }

  for (int a = 0; a < 1000; a++) {
    boost::shared_ptr<ProcessUnit> pu = builder.build(packetList);
    assert(pu->_source_stream==1);
    assert(pu->_target_stream==2);

    assert(pu->getDecoder());
    assert(pu->getDecoder()->getCodecId() == data[0].decoder->getCodecId());

    assert(pu->getEncoder());
    assert(pu->getEncoder()->getCodecId() == data[0].encoder->getCodecId());

    assert(pu->getInputPacketList().size() == packet_count);
    assert(pu->getGopSize() - packet_count<1);
    assert(pu->getExpectedFrameCount() - packet_count<1);
    /*this must be always 0, dont what what it is when cycle is over 100000(very very long video >270h)*/
    assert(fabs(data[0].frameRateCompensateBase - 0.00) < 0.00001);
  }
}

void testVideoFramerateResample15to20() {

  map<int, float> comp_map;
  map<int, int> count_map;
  comp_map[0] = 0.333333;
  comp_map[1] = 0.666667;
  comp_map[2] = 0.000001;
  comp_map[3] = 0.333333;
  comp_map[4] = 0.666667;

  count_map[0] = 149;
  count_map[1] = 149;
  count_map[2] = 150;
  count_map[3] = 149;
  count_map[4] = 149;

  int packet_count = 112;
  map<int, StreamData> data;
  data[0] = StreamData();
  data[0].decoder = boost::shared_ptr<Decoder > (new Decoder((CodecID) 13));
  data[0].decoder->setFrameRate(15, 1);

  data[0].encoder = boost::shared_ptr<Encoder > (new Encoder((CodecID) 13));

  data[0].encoder->setFrameRate(20, 1);
  data[0].instream=1;
  data[0].outstream=2;

  ProcessUnitBuilder builder(data);

  PacketListPtr packetList;
  for (int a = 0; a < packet_count; a++) {
    boost::shared_ptr<Packet > p = boost::shared_ptr<Packet > (new Packet());
    if (a % 10 == 0)
      p->setKeyPacket(true);
    packetList.push_back(p);
  }

  for (int a = 0; a < 5; a++) {
    boost::shared_ptr<ProcessUnit> pu = builder.build(packetList);
    assert(pu->_source_stream==1);
    assert(pu->_target_stream==2);
    assert(pu->getDecoder());
    assert(pu->getDecoder()->getCodecId() == data[0].decoder->getCodecId());

    assert(pu->getEncoder());
    assert(pu->getEncoder()->getCodecId() == data[0].encoder->getCodecId());

    assert(pu->getInputPacketList().size() == packet_count);
    assert(pu->getGopSize() == packet_count);
    assert(pu->getExpectedFrameCount() == count_map[a]);
    assert(fabs(data[0].frameRateCompensateBase - comp_map[a]) < 0.00001);
  }
}

void testVideoFramerateResample15to2397() {

  map<int, float> comp_map;
  map<int, int> count_map;
  comp_map[0] = 0.976;
  comp_map[1] = 0.952;
  comp_map[2] = 0.928;
  comp_map[3] = 0.904;
  comp_map[4] = 0.880;

  count_map[0] = 178;
  count_map[1] = 179;
  count_map[2] = 179;
  count_map[3] = 179;
  count_map[4] = 179;

  int packet_count = 112;
  map<int, StreamData> data;
  data[0] = StreamData();
  data[0].decoder = boost::shared_ptr<Decoder > (new Decoder((CodecID) 13));
  data[0].decoder->setFrameRate(15, 1);

  data[0].encoder = boost::shared_ptr<Encoder > (new Encoder((CodecID) 13));

  data[0].encoder->setFrameRate(2397, 100);
  data[0].instream=1;
  data[0].outstream=2;


  ProcessUnitBuilder builder(data);

  PacketListPtr packetList;
  for (int a = 0; a < packet_count; a++) {
    boost::shared_ptr<Packet > p = boost::shared_ptr<Packet > (new Packet());
    if (a % 10 == 0)
      p->setKeyPacket(true);
    packetList.push_back(p);
  }

  for (int a = 0; a < 5; a++) {
    boost::shared_ptr<ProcessUnit> pu = builder.build(packetList);
    assert(pu->_source_stream==1);
    assert(pu->_target_stream==2);

    assert(pu->getDecoder());
    assert(pu->getDecoder()->getCodecId() == data[0].decoder->getCodecId());

    assert(pu->getEncoder());
    assert(pu->getEncoder()->getCodecId() == data[0].encoder->getCodecId());

    assert(pu->getInputPacketList().size() == packet_count);
    assert(pu->getGopSize() == packet_count);
    LOGDEBUG("Expected Frames" << pu->getExpectedFrameCount());
    assert(pu->getExpectedFrameCount() == count_map[a]);
    LOGDEBUG("Compesate:" << data[0].frameRateCompensateBase)
    assert(fabs(data[0].frameRateCompensateBase - comp_map[a]) < 0.00001);
  }
}
void testVideoFramerateResample2397to2997() {

  map<int, float> comp_map;
  map<int, int> count_map;
  comp_map[0] = 0.035043805;
  comp_map[1] = 0.070087610;
  comp_map[2] = 0.105131415;
  comp_map[3] = 0.140175220;
  comp_map[4] = 0.175219025;

  count_map[0] = 140;
  count_map[1] = 140;
  count_map[2] = 140;
  count_map[3] = 140;
  count_map[4] = 140;

  int packet_count = 112;
  map<int, StreamData> data;
  data[0] = StreamData();
  data[0].decoder = boost::shared_ptr<Decoder > (new Decoder((CodecID) 13));
  data[0].decoder->setFrameRate(2397, 100);

  data[0].encoder = boost::shared_ptr<Encoder > (new Encoder((CodecID) 13));

  data[0].encoder->setFrameRate(2997, 100);
  data[0].instream=1;
  data[0].outstream=2;


  ProcessUnitBuilder builder(data);

  PacketListPtr packetList;
  for (int a = 0; a < packet_count; a++) {
    boost::shared_ptr<Packet > p = boost::shared_ptr<Packet > (new Packet());
    if (a % 10 == 0)
      p->setKeyPacket(true);
    packetList.push_back(p);
  }

  for (int a = 0; a < 5; a++) {
    boost::shared_ptr<ProcessUnit> pu = builder.build(packetList);
    assert(pu->_source_stream==1);
    assert(pu->_target_stream==2);

    assert(pu->getDecoder());
    assert(pu->getDecoder()->getCodecId() == data[0].decoder->getCodecId());

    assert(pu->getEncoder());
    assert(pu->getEncoder()->getCodecId() == data[0].encoder->getCodecId());

    assert(pu->getInputPacketList().size() == packet_count);
    assert(pu->getGopSize() == packet_count);
    LOGDEBUG("Expected Frames" << pu->getExpectedFrameCount());
    assert(pu->getExpectedFrameCount() == count_map[a]);
    LOGDEBUG("Compesate:" << data[0].frameRateCompensateBase)
    assert(fabs(data[0].frameRateCompensateBase - comp_map[a]) < 0.00001);
  }
}
void testVideoFramerateResample30to25() {

  map<int, float> comp_map;
  map<int, int> count_map;
  comp_map[0] = 0.3333333333333357;
  comp_map[1] = 0.6666666666666714;
  comp_map[2] = 0.0;
  comp_map[3] = 0.33333333333334281;
  comp_map[4] = 0.6666666666666714;

  count_map[0] = 33;
  count_map[1] = 33;
  count_map[2] = 34;
  count_map[3] = 33;
  count_map[4] = 33;

  int packet_count = 40;
  map<int, StreamData> data;
  data[0] = StreamData();
  data[0].decoder = boost::shared_ptr<Decoder > (new Decoder((CodecID) 13));
  data[0].decoder->setFrameRate(30, 1);

  data[0].encoder = boost::shared_ptr<Encoder > (new Encoder((CodecID) 13));

  data[0].encoder->setFrameRate(25, 1);
  data[0].instream=1;
  data[0].outstream=2;
  data[0].min_packet_count=39;


  ProcessUnitBuilder builder(data);

  PacketListPtr packetList;
  for (int a = 0; a < packet_count; a++) {
    boost::shared_ptr<Packet > p = boost::shared_ptr<Packet > (new Packet());
    if (a % 10 == 0)
      p->setKeyPacket(true);
    packetList.push_back(p);
  }

  for (int a = 0; a < 5; a++) {
    boost::shared_ptr<ProcessUnit> pu = builder.build(packetList);
    assert(pu->_source_stream==1);
    assert(pu->_target_stream==2);

    assert(pu->getDecoder());
    assert(pu->getDecoder()->getCodecId() == data[0].decoder->getCodecId());

    assert(pu->getEncoder());
    assert(pu->getEncoder()->getCodecId() == data[0].encoder->getCodecId());

    assert(pu->getInputPacketList().size() == packet_count);
    assert(pu->getGopSize() == packet_count);
    LOGDEBUG("Expected Frames" << pu->getExpectedFrameCount());
    LOGDEBUG("Compesate:" << data[0].frameRateCompensateBase)
    assert(pu->getExpectedFrameCount() == count_map[a]);
    assert(fabs(data[0].frameRateCompensateBase - comp_map[a]) < 0.00001);
  }
}

int main(int argc, char** argv) {
  av_register_all();
  avcodec_init();
  avcodec_register_all();
  Log::open("");
  testAudioResample48000to44100();
  testAudioSameSamplerate();
  testVideoSameFramerate();
  testVideoFramerateResample15to20();
  testVideoFramerateResample15to2397();
  testVideoFramerateResample2397to2997();
  testVideoFramerateResample30to25();
  
  Log::close();
  return 0;
}

