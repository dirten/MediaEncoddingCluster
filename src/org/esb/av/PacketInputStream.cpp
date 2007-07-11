#include "org/esb/lang/Exception.h"
#include "PacketInputStream.h"
#include "Packet.h"
#include "Codec.h"
#include "FormatInputStream.h"
#include "org/esb/io/InputStream.h"
#include "introspec.h"
#include <iostream>

using namespace std;
using namespace org::esb::av;
using namespace org::esb::lang;


PacketInputStream::PacketInputStream(InputStream * is){
	_readFrom=0;
    if(instanceOf(*is, FormatInputStream)){
        _formatCtx=((FormatInputStream*)is)->getFormatContext();
        _streamIndex=((FormatInputStream*)is)->selectedStream;
        _codecCtx=_formatCtx->streams[_streamIndex]->codec;	
        _codec=avcodec_find_decoder(_codecCtx->codec_id);
        if(avcodec_open(_codecCtx, _codec)<0) {
            fprintf(stderr, "avcodec_open failed in PacketInputStream\n");
        }
        _readFrom=1;
    }else{
    	_source=is;	
    }
}

PacketInputStream::~PacketInputStream(){
    if(_readFrom==1){
	avcodec_close(_codecCtx);
	av_free_packet(&_packet);
    }
}

Packet & PacketInputStream::readPacket(){
    if(_readFrom==1)
    	return readPacketFromFormatIS();
    return readPacketFromIS();
    
}

Packet & PacketInputStream::readPacketFromFormatIS(){
    do {
        if(_packet.data!=NULL)
            av_free_packet(&_packet);
            if(av_read_frame(_formatCtx, &_packet)<0){
				cout << "Invalid Packet read"<<endl;
				break;
            }
        } while(_packet.stream_index!=_streamIndex);
    return _packet;
}

Packet & PacketInputStream::readPacketFromIS(){
    if(_packet.data!=NULL)
	   delete [] _packet.data;// av_free_packet(&_packet);
	read((unsigned char*)&_packet.pts,sizeof(int64_t));
	read((unsigned char*)&_packet.dts,sizeof(int64_t));
	read((unsigned char*)&_packet.size,sizeof(int));
	read((unsigned char*)&_packet.stream_index,sizeof(int));
	read((unsigned char*)&_packet.flags,sizeof(int));
	read((unsigned char*)&_packet.duration,sizeof(int));
	read((unsigned char*)&_packet.pos,sizeof(int64_t));
	uint8_t * b=new uint8_t[_packet.size];
	_packet.data=b;
	read((unsigned char*)_packet.data,_packet.size);	
	return _packet;
}

int PacketInputStream::read(unsigned char * buffer, int length){
    return _source->read(buffer, length);
}

int PacketInputStream::read(){
    return _source->read();
}

int PacketInputStream::read(vector<unsigned char>&buffer){
    return _source->read(buffer);
}

int PacketInputStream::available(bool isBlocking){
    return _formatCtx->file_size;
}

uint64_t PacketInputStream::getDuration(){
    return _formatCtx->streams[_streamIndex]->duration;
}

Codec * PacketInputStream::getCodec(){
    return new Codec(_codecCtx);
}

void PacketInputStream::skip(long packets){
    av_seek_frame(_formatCtx,_streamIndex,packets,AVSEEK_FLAG_ANY);
}


