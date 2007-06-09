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
    if(_readFrom==1)
  		avcodec_close(_codecCtx);
}

Packet * PacketInputStream::readPacket(){
    if(_readFrom==1)
    	return readPacketFromFormatIS();
   	return readPacketFromIS();
    
}

Packet * PacketInputStream::readPacketFromFormatIS(){
    Packet * packet=new Packet();
    packet->data=NULL;

    do {
        if(packet->data!=NULL)
            av_free_packet(packet);
            if(av_read_packet(_formatCtx, packet)<0){
                delete packet;
                return NULL;
            }
        } while(packet->stream_index!=_streamIndex);
    return packet;
}

Packet * PacketInputStream::readPacketFromIS(){
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
	Packet * packet=new Packet();
	packet->pts=atoi(pts);
	packet->dts=atoi(dts);
	packet->size=atoi(size);
	packet->stream_index=atoi(sIndex);
	packet->flags=atoi(flags);
	packet->duration=atoi(duration);
	packet->pos=atoi(pos);
	packet->data=data;
	
	return packet;
}

int PacketInputStream::read(unsigned char * buffer, int length){
    return _source->read(buffer, length);
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


