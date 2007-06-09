#include "PacketOutputStream.h"
#include "Packet.h"

using namespace org::esb::av;
PacketOutputStream::PacketOutputStream(OutputStream * os){
	_target=os;
}
PacketOutputStream::~PacketOutputStream(){
}

void PacketOutputStream::writePacket(Packet * packet){

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

	snprintf(pts,sizeof(pts),"%.20d",packet->pts);
	snprintf(dts,sizeof(dts),"%.20d",packet->dts);
	snprintf(size,sizeof(size),"%.10d",packet->size);
	snprintf(sIndex,sizeof(sIndex),"%.10d",packet->stream_index);
	snprintf(flags,sizeof(flags),"%.10d",packet->flags);
	snprintf(duration,sizeof(duration),"%.10d",packet->duration);
	snprintf(pos,sizeof(pos),"%.20d",packet->pos);

	write(pts,sizeof(pts)-1);
	write(dts,sizeof(dts)-1);
	write(size,sizeof(size)-1);
	write(sIndex,sizeof(sIndex)-1);
	write(flags,sizeof(flags)-1);
	write(duration,sizeof(duration)-1);
	write(pos,sizeof(pos)-1);
	write((char*)packet->data,packet->size);

}
void PacketOutputStream::write(char * buffer, int length){
	_target->write(buffer, length);
}


