#include "avformat.h"
#include "Packet.h"
#include <iostream>
using namespace std;
using namespace org::esb::av;


Packet::Packet(AVPacket *packet){
    int s=packet->size;
//    cout << "PacketSize:"<<s<<endl;
    data=new uint8_t[s];
    memset(data,1,s);
    memcpy(data, packet->data, s);
    size=packet->size;
    flags=packet->flags;
}

Packet::~Packet(){
    if(data)
        delete []data;
}
