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
	//_codec=_codecCtx->codec;
	
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
	char pts[21];
	char dts[21];
	char size[11];
	char sIndex[11];
	char flags[11];
	char duration[11];
	char pos[21];

	memset(pts,0,sizeof(pts));
	memset(dts,0,sizeof(dts));
	memset(size,0,sizeof(size));
	memset(sIndex,0,sizeof(sIndex));
	memset(flags,0,sizeof(flags));
	memset(duration,0,sizeof(duration));
	memset(pos,0,sizeof(pos));

	read((unsigned char *)pts,sizeof(pts)-1);
	read((unsigned char *)dts,sizeof(dts)-1);
	read((unsigned char *)size,sizeof(size)-1);
	read((unsigned char *)sIndex,sizeof(sIndex)-1);
	read((unsigned char *)flags,sizeof(flags)-1);
	read((unsigned char *)duration,sizeof(duration)-1);
	read((unsigned char *)pos,sizeof(pos)-1);

	uint8_t * data=(uint8_t*)malloc(atoi(size));
	memset(data,0,atoi(size));
	read((unsigned char *)data,atoi(size));
//	Packet * packet=new Packet();
	_packet.pts=atoi(pts);
	_packet.dts=atoi(dts);
	_packet.size=atoi(size);
	_packet.stream_index=atoi(sIndex);
	_packet.flags=atoi(flags);
	_packet.duration=atoi(duration);
	_packet.pos=atoi(pos);
	_packet.data=data;
	
	return _packet;
}

int PacketInputStream::read(unsigned char * buffer, int length){
//    if(_source->available(true))
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


