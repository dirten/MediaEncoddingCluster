#include "PacketOutputStream.h"
#include "org/esb/io/BufferedOutputStream.h"
#include "org/esb/av/FormatOutputStream.h"
#include "Packet.h"
#include "introspec.h"

using namespace org::esb::av;
using namespace org::esb::io;

PacketOutputStream::PacketOutputStream(OutputStream * os){
    if(instanceOf(*os, FormatOutputStream)){
	  _fmtCtx=((FormatOutputStream*)os)->_fmtCtx;
    }else{
	  _target=new BufferedOutputStream(os,32000);
    }
}
void PacketOutputStream::close(){
//    av_write_trailer(_fmtCtx);

}

PacketOutputStream::~PacketOutputStream(){
//    av_write_trailer(_fmtCtx);
//	delete _target;
}

void PacketOutputStream::writePacket(Packet & packet){
    if(!_isInitialized)
      throw runtime_error("PacketOutputStream not initialized!!! You must call init() before using writePacket(Packet & packet)");
    av_interleaved_write_frame(_fmtCtx,packet.packet);
//    int result=av_write_frame(_fmtCtx,&packet);
}

void PacketOutputStream::setEncoder(Codec & encoder){
    setEncoder(encoder,0);
}

void PacketOutputStream::setEncoder(Codec & encoder, int stream_id){
    AVStream * st=av_new_stream(_fmtCtx,0);
    st->codec=encoder.ctx;
    st->time_base=st->codec->time_base;
	cout << "TimeBase #"<<"\tnum:"<<st->time_base.num<<"\tden"<<st->time_base.den<<endl;

//    av_write_header(_fmtCtx);

}

void PacketOutputStream::init(){
    if(av_write_header(_fmtCtx)<0){
		cout <<"av_write_header(_fmtCtx) failed"<<endl;
		exit(1);
    }
    _isInitialized=true;
    dump_format(_fmtCtx, 0, NULL, 1);
    
    int streams=_fmtCtx->nb_streams;
	for (int a=0;a<streams;a++){
		AVStream * stream=_fmtCtx->streams[a];
		stream->time_base=stream->codec->time_base;

		cout << "TimeBase #"<<a<<"\tnum:"<<stream->codec->time_base.num<<"\tden"<<stream->codec->time_base.den<<endl;
		cout << "TimeBase Stream#"<<a<<"\tnum:"<<stream->time_base.num<<"\tden"<<stream->time_base.den<<endl;	
	}

}
/*
void PacketOutputStream::writePacket(Packet * packet){
	write((char*)&packet->pts,sizeof(int64_t));
	write((char*)&packet->dts,sizeof(int64_t));
	write((char*)&packet->size,sizeof(int));
	write((char*)&packet->stream_index,sizeof(int));
	write((char*)&packet->flags,sizeof(int));
	write((char*)&packet->duration,sizeof(int));
	write((char*)&packet->pos,sizeof(int64_t));
	write((char*)packet->data,packet->size);
	flush();
}
*/
void PacketOutputStream::write(char * buffer, int length){
	_target->write(buffer, length);
}

void PacketOutputStream::write(vector<unsigned char>&buffer){
	_target->write(buffer);
}

void PacketOutputStream::flush(){
	_target->flush();
}
