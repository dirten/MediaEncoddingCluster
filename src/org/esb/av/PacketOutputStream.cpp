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

PacketOutputStream::~PacketOutputStream(){
    av_write_trailer(_fmtCtx);
//	delete _target;
}

void PacketOutputStream::writePacket(Packet & packet){
    int result=av_write_frame(_fmtCtx,&packet);
}

void PacketOutputStream::setEncoder(Encoder & encoder){
    setEncoder(encoder,0);
}

void PacketOutputStream::setEncoder(Encoder & encoder, int stream_id){
    AVStream * st=av_new_stream(_fmtCtx,stream_id);
    st->codec=&encoder;
//    av_write_header(_fmtCtx);

}

void PacketOutputStream::init(){
    av_write_header(_fmtCtx);
}

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

void PacketOutputStream::write(char * buffer, int length){
	_target->write(buffer, length);
}

void PacketOutputStream::write(vector<unsigned char>&buffer){
	_target->write(buffer);
}

void PacketOutputStream::flush(){
	_target->flush();
}
