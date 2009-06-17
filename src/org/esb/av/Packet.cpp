#include "Packet.h"
#include "AV.h"
#include "org/esb/util/Log.h"
#include <iostream>
using namespace std;
using namespace org::esb::av;

Packet::Packet() {
  //	cout << "Packet()"<<endl;
  isCopy = false;
  packetPtr = boost::shared_ptr<AVPacket > (new AVPacket());
  packet = packetPtr.get();
  av_init_packet(packet);
  callDestruct = false;
  packet->data = NULL;
  //	data=0;
  packet->size=0;

}

Packet::Packet(const Packet & p) {
  //	cout << "Packet(const Packet & p)"<<endl;
  //	callDestruct=false;
  packetPtr = boost::shared_ptr<AVPacket > (new AVPacket());
  packet = packetPtr.get();

  av_init_packet(packet);
  packet->data = 0;
  packet->pts = p.packet->pts;
  packet->dts = p.packet->dts;
  if (p.packet->size > 0) {
    packet->data = new uint8_t[p.packet->size];
//    packet->data = new uint8_t[p.packet->size + FF_INPUT_BUFFER_PADDING_SIZE];
//    memset(packet->data, 0, packet->size + FF_INPUT_BUFFER_PADDING_SIZE);
    memcpy(packet->data, p.packet->data, p.packet->size);
  }
  packet->size = p.packet->size;
  packet->stream_index = p.packet->stream_index;
  packet->flags = p.packet->flags;
  packet->duration = p.packet->duration;
  packet->destruct = p.packet->destruct;
  packet->priv = p.packet->priv;
  packet->pos = p.packet->pos;
  callDestruct = true;
  
}

Packet Packet::operator=(Packet & p) {
  //	cout << "Packet::operator="<<endl;
  packetPtr = boost::shared_ptr<AVPacket > (new AVPacket());
  packet = packetPtr.get();
  //	packet=new AVPacket();
  av_init_packet(packet);
  packet->pts = p.packet->pts;
  packet->dts = p.packet->dts;
  packet->data = 0;
    packet->data = new uint8_t[p.packet->size];
//  packet->data = new uint8_t[p.packet->size + FF_INPUT_BUFFER_PADDING_SIZE ];
//  memset(packet->data, 0, packet->size + FF_INPUT_BUFFER_PADDING_SIZE);
  memcpy(packet->data, p.packet->data, p.packet->size);
  packet->size = p.packet->size;
  packet->stream_index = p.packet->stream_index;
  packet->flags = p.packet->flags;
  packet->duration = p.packet->duration;
  packet->destruct = p.packet->destruct;
  packet->priv = p.packet->priv;
  packet->pos = p.packet->pos;
  callDestruct = true;
  return *this;
}

Packet::Packet(int s) {
  //	cout << "Packet(int s)"<<endl;
  isCopy = false;
  packetPtr = boost::shared_ptr<AVPacket > (new AVPacket());
  packet = packetPtr.get();
  av_init_packet(packet);
  packet->size = s;
  if (s > 0) {
//    packet->data = new uint8_t[s];
    packet->data = new uint8_t[s + FF_INPUT_BUFFER_PADDING_SIZE];
    memset(packet->data, 0, s + FF_INPUT_BUFFER_PADDING_SIZE);
    callDestruct = true;
  }else {
//    logerror("Try to Allocate Packet buffer <0")
    packet->data=NULL;
        callDestruct = false;
  }
}

Packet::~Packet() {
  //    cout << "delete Packet"<<endl;
  //  	av_free_packet(packetPtr.get());
  if (callDestruct)
    delete [] packet->data;

  else
    av_free_packet(packet);

  //    packet->data=0;
  //    delete packet;
  //    packet=0;
}

uint8_t * Packet::getData() {
  return packet->data;
}

int Packet::getSize() {
  return packet->size;
}

void Packet::setPts(long long int ts){
  packet->pts=ts;
}
void Packet::setDts(long long int ts){
  packet->dts=ts;
}
long long int Packet::getPts() {
  return packet->pts;
}

long long int Packet::getDts() {
  return packet->dts;
}

int Packet::getFlags() {
  return packet->flags;
}

int Packet::getStreamIndex() {
  return packet->stream_index;
}
void Packet::setStreamIndex(int idx) {
  packet->stream_index=idx;
}

int Packet::getDuration() {
  return packet->duration;
}
void Packet::setDuration(int d) {
  packet->duration=d;
}

bool Packet::isKeyFrame() {
  return packet->flags & PKT_FLAG_KEY;
}

void * Packet::getPriv() {
  return packet->priv;
}

int64_t Packet::getPosition() {
  return packet->pos;
}
AVPacket* Packet::getAVPacket() {
  return packetPtr.get();
}
void Packet::setTimeBase(AVRational t){
  _time_base=t;
}
AVRational Packet::getTimeBase(){
  return _time_base;
}

void Packet::toString(){
  logdebug("Packet->Size:"<<getSize()<<
      ":Pts:"<<getPts()<<
      ":Dts:"<<getDts()<<
      ":StreamIndex:"<<getStreamIndex()<<
      ":Duration:"<<getDuration()<<
      ":Position:"<<getPosition()<<
      ":TimeBase:"<<getTimeBase().num<<":"<<getTimeBase().den);
//  logdebug("Packet->Pts:"<<getPts());
//  logdebug("Packet->Dts:"<<getDts());
//  logdebug("Packet->StreamIndex:"<<getStreamIndex());
//  logdebug("Packet->Duration:"<<getDuration());
//  logdebug("Packet->Position:"<<getPosition());
//  logdebug("Packet->TimeBase:"<<getTimeBase().num<<":"<<getTimeBase().den);

}

