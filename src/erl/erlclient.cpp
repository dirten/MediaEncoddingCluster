#include "erl.cpp"

#include "org/esb/av/Decoder.h"
#include "org/esb/av/Encoder.h"
#include "org/esb/av/Frame.h"
#include "org/esb/lang/Thread.h"
#include <string>
#include <vector>
using namespace org::esb::av;
Packet * create_packet(ETERM* in){
  ETERM * streamidx=erl_element(1, in);
  ETERM * isKey=erl_element(2, in);
  ETERM * pts=erl_element(3, in);
  ETERM * dts=erl_element(4, in);
  ETERM * flags=erl_element(5, in);
  ETERM * duration=erl_element(6, in);
  ETERM * size=erl_element(7, in);
  ETERM * data=erl_element(8, in);
  Packet * p=new Packet(ERL_INT_UVALUE(size));
  p->packet->stream_index=ERL_INT_UVALUE(streamidx);
  p->packet->pts=ERL_INT_UVALUE(pts);
  p->packet->dts=ERL_INT_UVALUE(dts);
  p->packet->flags=ERL_INT_UVALUE(flags);
  p->packet->duration=ERL_INT_UVALUE(duration);
  memcpy(p->packet->data,ERL_ATOM_PTR(data),p->getSize());
  return p;
}
/*
    ar & _codec_id;
    ar & _mode;
    ar & _flags;
    ar & _pix_fmt;
    ar & _width;
    ar & _height;
    ar & _time_base.num;
    ar & _time_base.den;
    ar & _gop_size;
    ar & _bit_rate;
    ar & _channels;
    ar & _sample_rate;
    ar & _sample_format;

*/
Decoder * create_decoder(ETERM* in){
/*-record(stream,{
	2id,
	3fileid,
	4streamidx,
	5streamtype,
	6codec,
	7codecname,
	8rate,
	9num,
	10den,
	11width,
	12height,
	13channels,
	14gop,
	15format}).*/

//        {stream,1,1,0,0,2,undefined,25,1,90000,720,576,0,12,0}
  ETERM * codecid=erl_element(6, in);
  ETERM * rate=erl_element(8, in);
  ETERM * num=erl_element(9, in);
  ETERM * den=erl_element(10, in);
  ETERM * width=erl_element(11, in);
  ETERM * height=erl_element(12, in);
  ETERM * channels=erl_element(13, in);
  ETERM * gop=erl_element(14, in);
  ETERM * fmt=erl_element(15, in);
//  ETERM * sample_rate=erl_element(10, in);
//  ETERM * sample=erl_element(1, in);
	Decoder * d=new Decoder(static_cast<CodecID>(ERL_INT_UVALUE(codecid)));
	d->findCodec(Codec::DECODER);
	d->setPixelFormat(d->_codec->type== CODEC_TYPE_VIDEO ?static_cast<PixelFormat>(ERL_INT_UVALUE(fmt)):static_cast<PixelFormat>(0));
	d->setHeight(ERL_INT_UVALUE(height));
	d->setWidth(ERL_INT_UVALUE(width));
	AVRational r;
	r.num=ERL_INT_UVALUE(num);
	r.den=ERL_INT_UVALUE(den);
	d->setTimeBase(r);
	d->setGopSize(ERL_INT_UVALUE(gop));
	d->setBitRate(d->_codec->type== CODEC_TYPE_VIDEO?ERL_INT_UVALUE(rate):0);
	d->setChannels(ERL_INT_UVALUE(channels));
	d->setSampleRate(d->_codec->type== CODEC_TYPE_AUDIO?ERL_INT_UVALUE(rate):0);
	d->setSampleFormat(d->_codec->type== CODEC_TYPE_AUDIO ?static_cast<SampleFormat>(ERL_INT_UVALUE(fmt)):static_cast<SampleFormat>(0));
	d->open();

	return d;
}
Encoder * create_encoder(ETERM* in){
  ETERM * codecid=erl_element(6, in);
  ETERM * fmt=erl_element(2, in);
  ETERM * width=erl_element(3, in);
  ETERM * height=erl_element(4, in);
  ETERM * num=erl_element(5, in);
  ETERM * den=erl_element(6, in);
  ETERM * gop=erl_element(7, in);
  ETERM * bit_rate=erl_element(8, in);
  ETERM * channels=erl_element(9, in);
  ETERM * sample_rate=erl_element(10, in);
	Encoder * d=new Encoder(static_cast<CodecID>(ERL_INT_UVALUE(codecid)));
	d->findCodec(Codec::DECODER);
	d->setPixelFormat(d->_codec->type== CODEC_TYPE_VIDEO ?static_cast<PixelFormat>(ERL_INT_UVALUE(fmt)):static_cast<PixelFormat>(0));
	d->setHeight(ERL_INT_UVALUE(height));
	d->setWidth(ERL_INT_UVALUE(width));
	AVRational r;
	r.num=ERL_INT_UVALUE(num);
	r.den=ERL_INT_UVALUE(den);
	d->setTimeBase(r);
	d->setGopSize(ERL_INT_UVALUE(gop));
	d->setBitRate(ERL_INT_UVALUE(bit_rate));
	d->setChannels(ERL_INT_UVALUE(channels));
	d->setSampleRate(ERL_INT_UVALUE(sample_rate));
	d->setSampleFormat(d->_codec->type== CODEC_TYPE_AUDIO ?static_cast<SampleFormat>(ERL_INT_UVALUE(fmt)):static_cast<SampleFormat>(0));
	d->open();

	return d;


//	return enc;
}

ETERM * encode(ETERM* in){
  std::vector<ETERM *> terms;
  ETERM * input_term=erl_element(2, in);
  ETERM * file_name=erl_element(1, input_term);
  ETERM * decoder=erl_element(3, input_term);
  ETERM * encoder=erl_element(4, input_term);
  ETERM * packet_list=erl_element(5, input_term);
  terms.push_back(erl_mk_atom("encoded"));
//  org::esb::lang::Thread::sleep2(20000);
  Decoder *d=create_decoder(decoder);
	int pc=ERL_TUPLE_SIZE(packet_list);
	for(int a=0;a<pc;a++){
		logdebug("Create Packet"<<a);
		Packet *p=create_packet(erl_element(a+1,packet_list));
		//d->decode(*p);
//		terms.push_back(erl_mk_uint(ERL_TUPLE_SIZE(erl_element(2, packet_list))));
	}
  delete d;

  Encoder *e=create_encoder(decoder);
  delete e;
  
/*
  terms.push_back(erl_element(1, packet_list));
  terms.push_back(erl_element(1,erl_element(1, packet_list)));
  terms.push_back(erl_element(2,erl_element(1, packet_list)));
  terms.push_back(erl_element(3,erl_element(1, packet_list)));
  terms.push_back(erl_element(4,erl_element(1, packet_list)));
  terms.push_back(erl_element(5,erl_element(1, packet_list)));
  terms.push_back(erl_element(6,erl_element(1, packet_list)));
*/
  //  terms.push_back(erl_element(8,erl_element(1, packet_list)));
//  terms.push_back(erl_mk_uint(ERL_TUPLE_SIZE(packet_list)));
//  terms.push_back(decoder);
//  terms.push_back(  in);
/*
  terms.push_back(erl_mk_uint(ERL_TUPLE_SIZE(erl_element(1, packet_list))));
  terms.push_back(erl_mk_uint(ERL_IS_TUPLE(packet_list)));
  terms.push_back(erl_mk_uint(ERL_TUPLE_SIZE(packet_list)));*/
  return vector2term(terms);
}

int main() {
#ifdef WIN32
  /* Attention Windows programmers: you need to explicitly set
   * mode of stdin/stdout to binary or else the port program won't work
   */
  setmode(fileno(stdout), O_BINARY);
  setmode(fileno(stdin), O_BINARY);
#endif
  av_register_all();
  avcodec_init();
  avcodec_register_all();

  erl_init(NULL, 0);
  ETERM *intuple = NULL, *outtuple = NULL;
  byte buf[5000000];
  while (read_cmd(buf) > 0) {
    intuple = erl_decode(buf);
    ETERM* fnp = erl_element(1, intuple);
    if (fnp != NULL) {
      std::string func = (const char*) ERL_ATOM_PTR(fnp);
      if (func == "encode") {
        outtuple = encode(intuple);
      } else {
        std::vector<ETERM *> terms;
        terms.push_back(erl_mk_atom("unknown_command"));
        outtuple = vector2term(terms);
      }
      if (outtuple != NULL) {
        erl_encode(outtuple, buf);
        write_cmd(buf, erl_term_len(outtuple));
        erl_free_compound(outtuple);
        outtuple = NULL;
      }
    }
  }
}
