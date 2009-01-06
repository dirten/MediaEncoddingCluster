//#include <ei.h>
#include <erl_interface.h>

//#include <unistd.h>
//#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
#include <io.h>
#include <fcntl.h>
#endif

#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"

#include "org/esb/util/Log.h"
#include "org/esb/util/Decimal.h"
using namespace org::esb::av;
using namespace org::esb::util;
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

void file_import(){
  byte buf[100000];
  ETERM *intuple, *outtuple,*fnp, *argp, *argp2;
  ETERM * fileinfo[5];
  ETERM * streaminfo[12];
  ETERM * filenotfound[1];
 logdebug("waiting for Command");
// return;
  while (read_cmd(buf) > 0) {
	logdebug("Command readed");
    intuple = erl_decode(buf);
    fnp = erl_element(1, intuple);
    argp = erl_element(2, intuple);
	if (strncmp((const char*)ERL_ATOM_PTR(fnp), "fileinfo", 8) == 0) {
		logdebug("FileInfo on:"<<(const char*)ERL_ATOM_PTR(argp));
		File f((const char*)ERL_ATOM_PTR(argp));
		if(f.exists()){
		FormatInputStream fis(&f);
		fileinfo[0]=erl_mk_atom(f.getFileName().c_str());
		fileinfo[1]=erl_mk_atom(f.getFilePath().c_str());
		fileinfo[2]=erl_mk_atom(Decimal(fis.getFileSize()).toString().c_str());
		fileinfo[3]=erl_mk_atom(fis.getFormatContext()->iformat->name);
		fileinfo[4]=erl_mk_int(fis.getStreamCount());
	    outtuple=erl_mk_tuple(fileinfo,5);
		}else{
			filenotfound[0]=erl_mk_atom("filenotfound");
		    outtuple=erl_mk_tuple(filenotfound,1);
		}
		erl_encode(outtuple, buf);
		write_cmd(buf, erl_term_len(outtuple));
    } else if (strncmp((const char*)ERL_ATOM_PTR(fnp), "streaminfo", 10) == 0) {
		//-record(stream,{id,fileid,streamidx,streamtype,codec,rate,num, den, width, height,channels,gop,format}).
		argp2 = erl_element(3, intuple);
		int s=ERL_INT_UVALUE(argp2); 
		File f((const char*)ERL_ATOM_PTR(argp));
		if(f.exists()){
			FormatInputStream fis(&f);
			AVStream *str=fis.getFormatContext()->streams[s];
			if(s<0||str==NULL){
				filenotfound[0]=erl_mk_atom("streamnotfound");
			    outtuple=erl_mk_tuple(filenotfound,1);
			}else{
			streaminfo[0]=erl_mk_int(0);
			streaminfo[1]=erl_mk_int(str->index);
			streaminfo[2]=erl_mk_int(str->codec->codec_type);
			streaminfo[3]=erl_mk_int(str->codec->codec_id);
			streaminfo[4]=erl_mk_int(str->codec->codec_type==CODEC_TYPE_VIDEO?av_q2d(str->r_frame_rate):str->codec->sample_rate);
			streaminfo[5]=erl_mk_int(str->time_base.num);
			streaminfo[6]=erl_mk_int(str->time_base.den);
			streaminfo[7]=erl_mk_int(str->codec->width);
			streaminfo[8]=erl_mk_int(str->codec->height);
			streaminfo[9]=erl_mk_int(str->codec->channels);
			streaminfo[10]=erl_mk_int(str->codec->gop_size);
			streaminfo[11]=erl_mk_int(str->codec->codec_type==CODEC_TYPE_VIDEO?str->codec->pix_fmt:str->codec->sample_fmt);
			outtuple=erl_mk_tuple(streaminfo,12);
			}
		}else{
			filenotfound[0]=erl_mk_atom("filenotfound");
		    outtuple=erl_mk_tuple(filenotfound,1);
		}
		erl_encode(outtuple, buf);
		write_cmd(buf, erl_term_len(outtuple));
    }
  }
}


int main() {
  erl_init(NULL, 0);

#ifdef WIN32

  /* Attention Windows programmers: you need to explicitly set
   * mode of stdin/stdout to binary or else the port program won't work
   */
  setmode(fileno(stdout), O_BINARY);
  setmode(fileno(stdin), O_BINARY);
#endif

  file_import();
    logdebug("Program End");
	return 0;
  ETERM *tuplep, *intp;
  ETERM *fnp, *argp;
  ETERM * arr[3], *tuple;
  int res;
  byte buf[100000];
  long allocated, freed;

  erl_init(NULL, 0);
//	File f("/home/jhoelscher/pfad.avi");
//	File f("/home/jhoelscher/dracula.ts");
	File f("c:/1video/Der Blutige Pfad Gottes - German (DVD-Quali).avi");
	FormatInputStream fis(&f);
	PacketInputStream pis(&fis);
  int a=0;
  while (read_cmd(buf) > 0) {
//    logdebug("Message received");
      if(++a>20)break;
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
            logdebug("Program End");

}
