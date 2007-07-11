#include "PacketOutputStream.h"
#include "org/esb/io/BufferedOutputStream.h"
#include "Packet.h"

using namespace org::esb::av;
using namespace org::esb::io;

PacketOutputStream::PacketOutputStream(OutputStream * os){
	_target=new BufferedOutputStream(os,32000);
}

PacketOutputStream::~PacketOutputStream(){
	delete _target;
}

void PacketOutputStream::writePacket(Packet * packet){
	write((char*)&packet->pts,sizeof(int64_t));
	write((char*)&packet->dts,sizeof(int64_t));
	write((char*)&packet->size,sizeof(int));
	write((char*)&packet->stream_index,sizeof(int));
	write((char*)&packet->flags,sizeof(int));
	write((char*)&packet->duration,sizeof(int));
	write((char*)&packet->pos,sizeof(int64_t));
	write((char*)packet->data,packet->size);
	flush();
}

void PacketOutputStream::write(char * buffer, int length){
	_target->write(buffer, length);
}

void PacketOutputStream::write(vector<unsigned char>&buffer){
	_target->write(buffer);
}

void PacketOutputStream::flush(){
	_target->flush();
}
