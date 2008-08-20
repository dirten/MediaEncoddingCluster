#include <erl_interface.h>
#include <unistd.h>
#include <string.h>

#include "org/esb/io/File.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"

using namespace org::esb::av;
using namespace org::esb::io;
using namespace org::esb::lang;
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
  ETERM * arr[10], *tuple;
  int res;
  byte buf[100000];
  long allocated, freed;

  erl_init(NULL, 0);

	File f("/home/jhoelscher/pfad.avi");
	FormatInputStream fis(&f);
	PacketInputStream pis(&fis);
//    cout << "Programm started"<<endl;
  int loop=1;
  
  while (read_cmd(buf)>0) {
//    cout << "Running loop"<<endl;
//    if(++loop>10)break;
	Packet p;
	if(pis.readPacket(p)<0)break;
	
/*
    tuplep = erl_decode(buf);
    fnp = erl_element(1, tuplep);
    argp = erl_element(2, tuplep);
    
    if (strncmp((const char*)ERL_ATOM_PTR(fnp), "foo", 3) == 0) {
      res = foo(ERL_INT_VALUE(argp));
    } else if (strncmp((const char*)ERL_ATOM_PTR(fnp), "bar", 17) == 0) {
      res = bar(ERL_INT_VALUE(argp));
    }
*/
//    intp = erl_mk_int(p.getPts());
//%-record(packet, {id,stream_id,pts,dts, stream_index, key_frame, frame_group,flags, duration, pos, data}).
    int a=0;
	arr[0]=erl_mk_int(1);
	arr[1]=erl_mk_int(p.getPts());
	arr[2]=erl_mk_int(p.getDts());
	arr[3]=erl_mk_int(p.packet->stream_index);
	arr[4]=erl_mk_int(p.isKeyFrame()==true?1:0);
	arr[5]=erl_mk_int(1);
	arr[6]=erl_mk_int(p.packet->flags);
	arr[7]=erl_mk_int(p.packet->duration);
	arr[8]=erl_mk_int(p.packet->pos);
//	arr[a++]=erl_mk_int(p.getDts());
	arr[9]=erl_mk_binary((const char*)p.packet->data,p.packet->size );
	tuple=erl_mk_tuple(arr,10);
    erl_encode(tuple, buf);
    write_cmd(buf, erl_term_len(tuple));

//    erl_free_compound(tuplep);
    erl_free_compound(tuple);
//    erl_free_compound(arr);
    for(int i=0;i<10;i++){
      erl_free_term(arr[i]);
    }
//    erl_free_term(arr[1]);
//    erl_free_term(arr[2]);
//    erl_free_term(arr[0]);
//    erl_free_term(fnp);
//    erl_free_term(argp);
//    erl_free_term(intp);
  }
//  while(true)Thread::sleep(1000);
  exit(127);
}
