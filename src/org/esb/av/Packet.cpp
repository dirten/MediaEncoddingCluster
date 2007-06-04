#include "avformat.h"
#include "Packet.h"
#include <iostream>
using namespace std;
using namespace org::esb::av;


Packet::Packet(AVPacket *packet){
    size=packet->size;
    data=new uint8_t[size];
    memset(data,0,size);
    memcpy(data, packet->data, size);
    pts=packet->pts;
    dts=packet->dts;
    flags=packet->flags;
    stream_index=packet->stream_index;
    duration=packet->duration;
    priv=packet->priv;
    pos=packet->pos;
}

Packet::~Packet(){
    if(data)
        delete []data;
}

uint8_t * Packet::getData(){return data;}
int Packet::getSize(){return size;}
int Packet::getPts(){return pts;}
int Packet::getDts(){return dts;}
int Packet::getFlags(){return flags;}
int Packet::getStreamIndex(){return stream_index;}
int Packet::getDuration(){return duration;}
void * Packet::getPriv(){return priv;}
int64_t Packet::getPosition(){return pos;}


