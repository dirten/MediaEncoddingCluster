extern "C" {
#include "avformat.h"
}
#include "Packet.h"
#include <iostream>
using namespace std;
using namespace org::esb::av;

Packet::Packet(){
	isCopy=false;
	av_init_packet(this);
	callDestruct=false;
	data=0;
	size=0;

}

Packet::Packet(const Packet & packet){
	av_init_packet(this);
	data=0;
    pts=packet.pts;
    dts=packet.dts;
   	data=new uint8_t[packet.size];
   	memcpy(data,packet.data,packet.size);
    size=packet.size;
    stream_index=packet.stream_index;
    flags=packet.flags;
    duration=packet.duration;
    destruct=packet.destruct;
    priv=packet.priv;
    pos=packet.pos;
    callDestruct=true;
}

Packet & Packet::operator=(Packet & packet){
	av_init_packet(this);
    pts=packet.pts;
    dts=packet.dts;
    data=0;
   	data=new uint8_t[packet.size];
   	memcpy(data,packet.data,packet.size);
    size=packet.size;
    stream_index=packet.stream_index;
    flags=packet.flags;
    duration=packet.duration;
    destruct=packet.destruct;
    priv=packet.priv;
    pos=packet.pos;
    callDestruct=true;
    return *this;
}

Packet::Packet(int s){
	isCopy=false;
	av_init_packet(this);
	size=s;
	data=new uint8_t[s];
	memset(data,0,s);
	callDestruct=true;
}

Packet::~Packet(){
	if(callDestruct){
        delete []data;	
	}
	av_free_packet(this);
    data=0;
    size=0;
}

uint8_t * Packet::getData(){return data;}
int Packet::getSize(){return size;}
int Packet::getPts(){return pts;}
int Packet::getDts(){return dts;}
int Packet::getFlags(){return flags;}
int Packet::getStreamIndex(){return stream_index;}
int Packet::getDuration(){return duration;}
bool Packet::isKeyFrame(){return flags & PKT_FLAG_KEY;}
void * Packet::getPriv(){return priv;}
int64_t Packet::getPosition(){return pos;}

