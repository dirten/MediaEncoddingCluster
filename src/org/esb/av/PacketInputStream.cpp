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
    if(instanceOf(*is, FormatInputStream)){
        _formatCtx=((FormatInputStream*)is)->getFormatContext();
        _streamIndex=((FormatInputStream*)is)->selectedStream;
        _codecCtx=_formatCtx->streams[_streamIndex]->codec;
        _codec=avcodec_find_decoder(_codecCtx->codec_id);

        if(avcodec_open(_codecCtx, _codec)<0) {
            fprintf(stderr, "avcodec_open failed in PacketInputStream\n");
        }
    }else{
        throw Exception();
    }
}

PacketInputStream::~PacketInputStream(){
    avcodec_close(_codecCtx);
}

Packet * PacketInputStream::readPacket(){
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

int PacketInputStream::read(unsigned char * buffer, int length){
    return 0;
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


