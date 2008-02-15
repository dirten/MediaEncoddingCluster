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
	data=NULL;
	size=0;

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
    if(isCopy&&data&&size>0){
        delete []data;
    }else{
	if(callDestruct)
    	    delete  data;
	av_free_packet(this);
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

