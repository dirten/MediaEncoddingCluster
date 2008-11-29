#include "Packet.h"
#include "AV.h"
#include "org/esb/util/Log.h"
#include <iostream>
using namespace std;
using namespace org::esb::av;

Packet::Packet(){
//	cout << "Packet()"<<endl;
	isCopy=false;
	packetPtr=boost::shared_ptr<AVPacket>(new AVPacket());
	packet=packetPtr.get();
	av_init_packet(packet);
	callDestruct=false;
	packet->data=NULL;
//	data=0;
//	size=0;

}

Packet::Packet(const Packet & p){
//	cout << "Packet(const Packet & p)"<<endl;
//	callDestruct=false;
    packetPtr=boost::shared_ptr<AVPacket>(new AVPacket(),&av_free_packet);
    packet=packetPtr.get();
    av_init_packet(packet);
    packet->data=0;
    packet->pts=p.packet->pts;
    packet->dts=p.packet->dts;
    if(p.packet->size>0){
    	packet->data=new uint8_t[p.packet->size + FF_INPUT_BUFFER_PADDING_SIZE];
    	memset(packet->data,0,packet->size+FF_INPUT_BUFFER_PADDING_SIZE);
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


Packet Packet::operator=(Packet & p){
//	cout << "Packet::operator="<<endl;
    packetPtr=boost::shared_ptr<AVPacket>(new AVPacket(),&av_free_packet);
    packet=packetPtr.get();
//	packet=new AVPacket();
    av_init_packet(packet);
    packet->pts=p.packet->pts;
    packet->dts=p.packet->dts;
    packet->data=0;
    packet->data=new uint8_t[p.packet->size + FF_INPUT_BUFFER_PADDING_SIZE ];
    memset(packet->data ,0,packet->size+FF_INPUT_BUFFER_PADDING_SIZE);
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

Packet::Packet(int s){
//	cout << "Packet(int s)"<<endl;
	isCopy=false;
	packetPtr=boost::shared_ptr<AVPacket>(new AVPacket(),&av_free_packet);
	packet=packetPtr.get();
	av_init_packet(packet);
	packet->size=s;
	if(s>0){
		packet->data=new uint8_t[s + FF_INPUT_BUFFER_PADDING_SIZE];
		memset(packet->data,0,s + FF_INPUT_BUFFER_PADDING_SIZE);
		callDestruct=true;
	}else{
	  logerror("Try to Allocate buffer <0")
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
long long int Packet::getPts(){return packet->pts;}
long long int Packet::getDts(){return packet->dts;}
int Packet::getFlags(){return packet->flags;}
int Packet::getStreamIndex(){return packet->stream_index;}
int Packet::getDuration(){return packet->duration;}
bool Packet::isKeyFrame(){return packet->flags & PKT_FLAG_KEY;}
void * Packet::getPriv(){return packet->priv;}
int64_t Packet::getPosition(){return packet->pos;}

