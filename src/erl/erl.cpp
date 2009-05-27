


#ifndef ERL_ERL
#define ERL_ERL
extern "C" {
#include <erl_interface.h>
}
#include "org/esb/av/Packet.h"
#include "org/esb/util/Decimal.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#ifdef WIN32
#include <io.h>
#include <fcntl.h>
#else
#include <unistd.h>
#endif
#include "org/esb/av/Decoder.h"
#include "org/esb/av/Encoder.h"

typedef unsigned char byte;

int read_exact(byte *buf, int len) {
  int i, got = 0;

  do {
    if ((i = read(0, buf + got, len - got)) <= 0)
      return (i);
    got += i;
  } while (got < len);
//  logdebug("Bytes readed"<<len);
  return (len);
}

int write_exact(byte *buf, int len) {
  int i, wrote = 0;

  do {
    if ((i = write(1, buf + wrote, len - wrote)) <= 0)
      return (i);
    wrote += i;
  } while (wrote < len);

  return (len);
}

int read_cmd(byte *buf) {
  int len;

  if (read_exact(buf, 4) != 4)
    return (-1);
  len = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
  return read_exact(buf, len);
}

int write_cmd(byte *buf, int len) {
//  logdebug("Try Write Command:"<<buf);
  byte li;

  li = (len >> 24) & 0xff;
  write_exact(&li, 1);

  li = (len >> 16) & 0xff;
  write_exact(&li, 1);

  li = (len >> 8) & 0xff;
  write_exact(&li, 1);

  li = len & 0xff;
  write_exact(&li, 1);
//  logdebug("Write Buffer with Length "<<len);
  return write_exact(buf, len);
}


ETERM * vector2term(std::vector<ETERM*> & v) {
  const int s = static_cast<const int> (v.size());
  ETERM ** term = new ETERM*[(const int) s];
  std::vector<ETERM*>::iterator it = v.begin();
  for (int a = 0; it != v.end(); it++) {
    term[a++] = *it;
  }
  return erl_mk_tuple(term, v.size());
}

ETERM * vector2tuple(std::vector<ETERM*> & v) {
  const int s = static_cast<const int> (v.size());
  ETERM ** term = new ETERM*[(const int) s];
  std::vector<ETERM*>::iterator it = v.begin();
  for (int a = 0; it != v.end(); it++) {
    term[a++] = *it;
  }
  return erl_mk_tuple(term, v.size());
}

ETERM * vector2list(std::vector<ETERM*> & v) {
  const int s = static_cast<const int> (v.size());
  ETERM ** term = new ETERM*[(const int) s];
  std::vector<ETERM*>::iterator it = v.begin();
  for (int a = 0; it != v.end(); it++) {
    term[a++] = *it;
  }
  return erl_mk_list(term, v.size());
}
using namespace org::esb::av;
using namespace org::esb::util;

std::string toString(long long int num) {
  char c[25];
  memset(&c, 0, 25);
  sprintf(c, "%d", num);
  return std::string(c);
}
//{StreamIndex, KeyFrame, Pts, Dts, Flags, Duration, Size, Data}
ETERM * buildTermFromPacket(Packet & p){
    std::vector<ETERM *> terms;
    terms.push_back(erl_mk_int(p.getStreamIndex()));
    terms.push_back(erl_mk_int(p.isKeyFrame()));
//    terms.push_back(erl_mk_string(toString(p.getPts()).c_str()));
//    terms.push_back(erl_mk_string(toString(p.getDts()).c_str()));
    terms.push_back(erl_mk_string(Decimal(p.getPts()).toString().c_str()));
    terms.push_back(erl_mk_string(Decimal(p.getDts()).toString().c_str()));
//    terms.push_back(erl_mk_binary((const char *)p.getPts(),8));
//    terms.push_back(erl_mk_binary((const char*)p.getDts(),8));
    terms.push_back(erl_mk_int(p.getFlags()));
    terms.push_back(erl_mk_int(p.getDuration()));
    terms.push_back(erl_mk_int(p.getSize()));
    terms.push_back(erl_mk_binary((char*) p.getData(), p.getSize()));
    return vector2term(terms);
}

Packet * buildPacketFromTerm(ETERM * in){
  ETERM * streamidx = erl_element(1, in);
  ETERM * pts = erl_element(3, in);
  ETERM * dts = erl_element(4, in);
  ETERM * flags = erl_element(5, in);
  ETERM * duration = erl_element(6, in);
  ETERM * size = erl_element(7, in);
  ETERM * data = erl_element(8, in);
  Packet * p = new Packet(ERL_INT_UVALUE(size));
  p->packet->stream_index = ERL_INT_UVALUE(streamidx);
  sscanf((const char *)ERL_ATOM_PTR(pts),"%llu",&p->packet->pts);
  sscanf((const char *)ERL_ATOM_PTR(dts),"%llu",&p->packet->dts);
//  memcpy((char *)p->packet->pts, ERL_BIN_PTR(pts), 8);
//  memcpy((char *)p->packet->dts, ERL_BIN_PTR(dts), 8);
  p->packet->flags = ERL_INT_VALUE(flags);
  p->packet->duration = ERL_INT_UVALUE(duration);
  memcpy(p->packet->data, ERL_BIN_PTR(data), p->getSize());
  return p;
}

Decoder * buildDecoderFromTerm(ETERM* in) {
  ETERM * codecid = erl_element(6, in);
  ETERM * bitrate = erl_element(8, in);
  ETERM * rate = erl_element(9, in);
  ETERM * num = erl_element(10, in);
  ETERM * den = erl_element(11, in);
  ETERM * width = erl_element(12, in);
  ETERM * height = erl_element(13, in);
  ETERM * channels = erl_element(14, in);
  ETERM * gop = erl_element(15, in);
  ETERM * fmt = erl_element(16, in);

  Decoder * d = new Decoder(static_cast<CodecID> (ERL_INT_UVALUE(codecid)));
  d->findCodec(Codec::DECODER);
  d->setPixelFormat(d->_codec->type == CODEC_TYPE_VIDEO ? static_cast<PixelFormat> (ERL_INT_UVALUE(fmt)) : static_cast<PixelFormat> (0));
  //	d->setPixelFormat((PixelFormat)0);
  d->setHeight(ERL_INT_UVALUE(height));
  d->setWidth(ERL_INT_UVALUE(width));
  AVRational r;
  r.num = ERL_INT_UVALUE(num);
  r.den = ERL_INT_UVALUE(den);
  d->setTimeBase(r);
//  d->_time_base.num=ERL_INT_UVALUE(num);
//  d->_time_base.den=ERL_INT_UVALUE(den);
  d->setGopSize(ERL_INT_UVALUE(gop));
  d->setBitRate(ERL_INT_UVALUE(bitrate));
  d->setChannels(ERL_INT_UVALUE(channels));
  d->setSampleRate(d->_codec->type == CODEC_TYPE_AUDIO ? ERL_INT_UVALUE(rate) : 0);
  d->setSampleFormat(d->_codec->type == CODEC_TYPE_AUDIO ? static_cast<SampleFormat> (ERL_INT_UVALUE(fmt)) : static_cast<SampleFormat> (1));
  d->open();

  return d;
}

Encoder * buildEncoderFromTerm(ETERM * in){
  ETERM * codecid = erl_element(6, in);
  ETERM * bitrate = erl_element(8, in);
  ETERM * rate = erl_element(9, in);
  ETERM * num = erl_element(10, in);
  ETERM * den = erl_element(11, in);
  ETERM * width = erl_element(12, in);
  ETERM * height = erl_element(13, in);
  ETERM * channels = erl_element(14, in);
  ETERM * gop = erl_element(15, in);
  ETERM * fmt = erl_element(16, in);

  Encoder * d = new Encoder(static_cast<CodecID> (ERL_INT_UVALUE(codecid)));
  d->findCodec(Codec::ENCODER);
  d->setPixelFormat(d->_codec->type == CODEC_TYPE_VIDEO ? static_cast<PixelFormat> (ERL_INT_UVALUE(fmt)) : static_cast<PixelFormat> (0));
  d->setHeight(ERL_INT_UVALUE(height));
  d->setWidth(ERL_INT_UVALUE(width));
  AVRational r;
  r.num = ERL_INT_UVALUE(num);
  r.den = ERL_INT_UVALUE(den);
  d->setTimeBase(r);
//  d->_time_base.num=ERL_INT_UVALUE(num);
//  d->_time_base.den=ERL_INT_UVALUE(den);
//  d->setFlag(0);
  d->setGopSize(ERL_INT_UVALUE(gop));
  d->setBitRate(ERL_INT_UVALUE(bitrate));
  d->setChannels(ERL_INT_UVALUE(channels));
  d->setSampleRate(ERL_INT_UVALUE(rate));
  d->setSampleFormat(d->_codec->type == CODEC_TYPE_AUDIO ? static_cast<SampleFormat> (ERL_INT_UVALUE(fmt)) : static_cast<SampleFormat> (0));
  d->open();
  return d;
}
#endif

