#include "org/esb/io/File.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/av/Packet.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/PacketOutputStream.h"

#include <iostream>

using namespace std;
using namespace org::esb::io;
using namespace org::esb::av;
 
int main(){

	uint64_t tmp;

	memset(&tmp,9,sizeof(uint64_t));
	
	cout <<"sizeof:"<<sizeof(long double)<<endl;
	
	File file("../Der Blutige Pfad Gottes - German (DVD-Quali).avi");
    FormatInputStream fis(&file);
    PacketInputStream pis(fis.getStream(0));
//	pis.skip(155000);
		
	Packet * packet;
	int a=0;
	while((packet=pis.readPacket())/*&&a==0*/){
		
		


	    char filename[32];
        sprintf(filename,"/tmp/hive/test.%d.packet",a);
        FileOutputStream out(filename);
        PacketOutputStream pout(&out);
		pout.writePacket(packet);
		delete packet;
		a++;	
	}
}