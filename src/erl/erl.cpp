


#ifndef ERL_ERL
#define ERL_ERL

#include <erl_interface.h>


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


#endif

