#include <erl_interface.h>
#include <unistd.h>
#include <string.h>

#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"

#include "org/esb/util/Log.h"
using namespace org::esb::av;
using namespace org::esb::io;
int foo(int x) {
  return x+1;
}

int bar(int y) {
  return y*2;
}






typedef unsigned char byte;

int read_exact(byte *buf, int len)
{
  int i, got=0;

  do {
    if ((i = read(0, buf+got, len-got)) <= 0)
      return(i);
    got += i;
  } while (got<len);

  return(len);
}

int write_exact(byte *buf, int len)
{
  int i, wrote = 0;

  do {
    if ((i = write(1, buf+wrote, len-wrote)) <= 0)
      return (i);
    wrote += i;
  } while (wrote<len);

  return (len);
}


int read_cmd(byte *buf)
{
  int len;

  if (read_exact(buf, 2) != 2)
    return(-1);
  len = (buf[0] << 8) | buf[1];
  return read_exact(buf, len);
}

int write_cmd(byte *buf, int len)
{
  byte li;

  li = (len >> 8) & 0xff;
  write_exact(&li, 1);
  
  li = len & 0xff;
  write_exact(&li, 1);

  return write_exact(buf, len);
}




int main() {
  ETERM *tuplep, *intp;
  ETERM *fnp, *argp;
  ETERM * arr[3], *tuple;
  int res;
  byte buf[100000];
  long allocated, freed;

  erl_init(NULL, 0);
//	File f("/home/jhoelscher/pfad.avi");
	File f("/home/jhoelscher/dracula.ts");
	FormatInputStream fis(&f);
	PacketInputStream pis(&fis);
  while (read_cmd(buf) > 0) {
//    logdebug("Message received");
    Packet p;
    if(pis.readPacket(p)<0)break;
//	pis.readPacket(p);
    tuplep = erl_decode(buf);
    fnp = erl_element(1, tuplep);
    argp = erl_element(2, tuplep);
    
    if (strncmp((const char*)ERL_ATOM_PTR(fnp), "foo", 3) == 0) {
      res = foo(ERL_INT_VALUE(argp));
    } else if (strncmp((const char*)ERL_ATOM_PTR(fnp), "bar", 17) == 0) {
      res = bar(ERL_INT_VALUE(argp));
    }

//    intp = erl_mk_int(p.getPts());
    arr[0]=erl_mk_int(p.getPts());
    arr[1]=erl_mk_int(p.getDts());
//     logdebug("PacketSize:"<<p.packet->size);
    arr[2]=erl_mk_binary((const char*)p.packet->data,p.packet->size>30000?30000:p.packet->size );
    tuple=erl_mk_tuple(arr,3);
    erl_encode(tuple, buf);
    write_cmd(buf, erl_term_len(tuple));

    erl_free_compound(tuplep);
    erl_free_compound(tuple);
//    erl_free_compound(arr);
    erl_free_term(arr[0]);
    erl_free_term(arr[1]);
    erl_free_term(arr[2]);
//    erl_free_term(arr[0]);
    erl_free_term(fnp);
    erl_free_term(argp);
//    erl_free_term(intp);
  }
            logdebug("Message received");

}
