


#ifndef ERL_ERL
#define ERL_ERL

#include <erl_interface.h>
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
  byte li;

  li = (len >> 24) & 0xff;
  write_exact(&li, 1);

  li = (len >> 16) & 0xff;
  write_exact(&li, 1);

  li = (len >> 8) & 0xff;
  write_exact(&li, 1);

  li = len & 0xff;
  write_exact(&li, 1);

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
using namespace org::esb::av;
using namespace org::esb::util;
ETERM * buildTermFromPacket(Packet & p){
    std::vector<ETERM *> terms;
    terms.push_back(erl_mk_int(p.getStreamIndex()));
    terms.push_back(erl_mk_int(p.isKeyFrame()));
    terms.push_back(erl_mk_atom(Decimal(p.getPts()).toString().c_str()));
    terms.push_back(erl_mk_atom(Decimal(p.getDts()).toString().c_str()));
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
  p->packet->flags = ERL_INT_VALUE(flags);
  p->packet->duration = ERL_INT_UVALUE(duration);
  memcpy(p->packet->data, ERL_BIN_PTR(data), p->getSize());
  return p;
}


#endif

