#include "Packet.h"
#include <iostream>
using namespace std;
using namespace org::esb::av;

Packet::Packet(){
	isCopy=false;
	packetPtr=boost::shared_ptr<AVPacket>(new AVPacket());
	packet=packetPtr.get();
	av_init_packet(packet);
	callDestruct=false;
//	data=0;
//	size=0;

}

Packet::Packet(const Packet & p){
//	cout << "CopyConstruct"<<endl;
//	callDestruct=false;
	packetPtr=boost::shared_ptr<AVPacket>(new AVPacket());
	packet=packetPtr.get();
    av_init_packet(packet);
    packet->data=0;
    packet->pts=p.packet->pts;
    packet->dts=p.packet->dts;
	if(p.packet->size>0){
    	packet->data=new uint8_t[p.packet->size];
    	memcpy(packet->data,p.packet->data,p.packet->size);
    }
    packet->size=p.packet->size;
    packet->stream_index=p.packet->stream_index;
    packet->flags=p.packet->flags;
    packet->duration=p.packet->duration;
    packet->destruct=p.packet->destruct;
    packet->priv=p.packet->priv;
    packet->pos=p.packet->pos;
    callDestruct=true;
    
}

/*
Packet Packet::operator=(Packet & p){
	cout << "=Construct"<<endl;
	packet=new AVPacket();
    av_init_packet(packet);
    packet->pts=p.packet->pts;
    packet->dts=p.packet->dts;
    packet->data=0;
    packet->data=new uint8_t[p.packet->size];
    memcpy(packet->data,p.packet->data,p.packet->size);
    packet->size=p.packet->size;
    packet->stream_index=p.packet->stream_index;
    packet->flags=p.packet->flags;
    packet->duration=p.packet->duration;
    packet->destruct=p.packet->destruct;
    packet->priv=p.packet->priv;
    packet->pos=p.packet->pos;
    callDestruct=true;
    return *this;
}
*/
Packet::Packet(int s){
	isCopy=false;
	packetPtr=boost::shared_ptr<AVPacket>(new AVPacket());
	packet=packetPtr.get();
	av_init_packet(packet);
	packet->size=s;
	if(s>0){
		packet->data=new uint8_t[s];
		memset(packet->data,0,s);
		callDestruct=true;
	}
}

Packet::~Packet(){
//    cout << "delete Packet"<<endl;
//  	av_free_packet(packetPtr.get());
	
	if(callDestruct)
    	delete [] packet->data;
    else
  		av_free_packet(packet);

//    packet->data=0;
//    delete packet;
//    packet=0;
}

uint8_t * Packet::getData(){return packet->data;}
int Packet::getSize(){return packet->size;}
int Packet::getPts(){return packet->pts;}
int Packet::getDts(){return packet->dts;}
int Packet::getFlags(){return packet->flags;}
int Packet::getStreamIndex(){return packet->stream_index;}
int Packet::getDuration(){return packet->duration;}
bool Packet::isKeyFrame(){return packet->flags & PKT_FLAG_KEY;}
void * Packet::getPriv(){return packet->priv;}
int64_t Packet::getPosition(){return packet->pos;}

