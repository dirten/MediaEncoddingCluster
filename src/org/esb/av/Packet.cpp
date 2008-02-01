extern "C" {
#include "avformat.h"
}
#include "Packet.h"
#include <iostream>
using namespace std;
using namespace org::esb::av;

Packet::Packet(){
	isCopy=false;
	data=NULL;
//	av_init_packet(this);
}

Packet::Packet(Packet * packet){
	data=new uint8_t[packet->size+1];
	memset(data,0,packet->size+1);
	memcpy(data, packet->data, packet->size);
	size=packet->size;
	pts=packet->pts;
	dts=packet->dts;
	flags=packet->flags;
	stream_index=packet->stream_index;
	duration=packet->duration;
	pos=packet->pos;
	isCopy=true;
}

Packet::~Packet(){
    if(isCopy&&data&&size>0){
//	cout << "destruct copy data";
        delete []data;
    }else{
//	cout << "destruct av_free_packet";
//	av_free_packet(this);
    }
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

