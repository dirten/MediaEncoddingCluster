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
  packet->size = 0;
  _time_base.num = 0;
  _time_base.den = 0;
  _pict_type = 0;

}

/**
 * constructor to build an internal Packet directly from an AVPacket
 */
Packet::Packet(AVPacket * p) {
  packetPtr = boost::shared_ptr<AVPacket > (new AVPacket());
  packetPtr->pts = p->pts;
  packetPtr->dts = p->dts;
  packetPtr->pos = p->pos;
  packetPtr->size = p->size;
  packetPtr->duration = p->duration;
  packetPtr->convergence_duration = p->convergence_duration;
  packetPtr->flags = p->flags;
  packetPtr->priv = p->priv;
  packetPtr->stream_index = p->stream_index;
  _pict_type = 0;
  if (av_dup_packet(p) == 0) {
    //    packetPtr->data = static_cast<uint8_t*>(av_malloc(p->size + FF_INPUT_BUFFER_PADDING_SIZE));
    //    memcpy(packetPtr->data, p->data, p->size);
    //    memset(packetPtr->data + packetPtr->size, 0, FF_INPUT_BUFFER_PADDING_SIZE);
    packetPtr->data = p->data;
  }
  packetPtr->destruct = av_destruct_packet;
  callDestruct = false;
  /**
   * this must be removed in the near future, because this packet field will be hidden in the future
   */
  packet = packetPtr.get();
}

Packet::Packet(const Packet & p) {
  //  	cout << "Packet(const Packet & p)"<<endl;
  //	callDestruct=false;
  packetPtr = boost::shared_ptr<AVPacket > (new AVPacket());
  packet = packetPtr.get();
  //  _time_base.num=p._time_base.num;
  //  _time_base.den=p._time_base.num;
  _time_base = p._time_base;
  _pict_type = p._pict_type;
  //_time_base=p.getTimeBase();

  av_init_packet(packet);
  packet->data = 0;
  packet->pts = p.packet->pts;
  packet->dts = p.packet->dts;
  if (p.packet->size > 0) {
    //    packet->data = new uint8_t[p.packet->size];
    packet->data = new uint8_t[p.packet->size + FF_INPUT_BUFFER_PADDING_SIZE];
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
  cout << "Packet::operator=" << endl;
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
  //  logdebug("Packet(int s)"<<this);
  isCopy = false;
  _time_base.num = 0;
  _time_base.den = 0;
  _pict_type = 0;
  packetPtr = boost::shared_ptr<AVPacket > (new AVPacket());
  packet = packetPtr.get();
  av_init_packet(packet);
  packet->size = s;
  if (s > 0) {
    //    packet->data = new uint8_t[s];
    packet->data = new uint8_t[s + FF_INPUT_BUFFER_PADDING_SIZE];
    //    packet->data = static_cast<uint8_t*>(av_malloc(s + FF_INPUT_BUFFER_PADDING_SIZE));
    memset(packet->data, 0, s + FF_INPUT_BUFFER_PADDING_SIZE);
    callDestruct = true;
  } else {
    //    logerror("Try to Allocate Packet buffer <0")
    packet->data = NULL;
    callDestruct = false;
  }
}

Packet::~Packet() {
  //  logdebug("delete Packet"<<this);
  //  	av_free_packet(packetPtr.get());
  if (callDestruct)
    delete [] packet->data;
  else
    av_free_packet(packet);

  packet->data = 0;
  //    delete packet;
  //    packet=0;
}

uint8_t * Packet::getData() {
  return packet->data;
}

int Packet::getSize() {
  return packet->size;
}

void Packet::setPts(int64_t ts) {
  packet->pts = ts;
}

void Packet::setDts(int64_t ts) {
  packet->dts = ts;
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

void Packet::setFlags(int f) {
  packet->flags = f;
}

int Packet::getStreamIndex() {
  return packet->stream_index;
}

void Packet::setStreamIndex(int idx) {
  packet->stream_index = idx;
}

int Packet::getDuration() {
  return packet->duration;
}

void Packet::setDuration(int d) {
  packet->duration = d;
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

void Packet::setTimeBase(AVRational t) {
  _time_base = t;
}

void Packet::setTimeBase(int num, int den) {
  _time_base.num = num;
  _time_base.den = den;
}

AVRational Packet::getTimeBase() {
  return _time_base;
}
void Packet::setKeyPacket(bool key){
  if(key)
    packet->flags |= PKT_FLAG_KEY;
}

std::string Packet::toString() {
  std::ostringstream oss;
  oss << "P->S:" << getSize() <<
      ":Pts:" << getPts() <<
      ":Dts:" << getDts() <<
      ":Index:" << getStreamIndex() <<
      ":Dur:" << getDuration() <<
      ":Pos:" << getPosition() <<
      ":TBase:" << getTimeBase().num << "/" << getTimeBase().den <<
      ":F:" << getFlags() <<
      ":KF" << isKeyFrame();
  std::string type;
  switch (_pict_type) {
    case FF_I_TYPE: type = "I";
      break;
    case FF_B_TYPE: type = "B";
      break;
    case FF_BI_TYPE: type = "BI";
      break;
    case FF_P_TYPE: type = "P";
      break;
    default:type = "U";
      break;
  }
  oss << ":T=" << type;
  return std::string(oss.str());
}
