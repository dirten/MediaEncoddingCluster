#include "org/esb/io/File.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/av/Packet.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/PacketOutputStream.h"
#include "org/esb/av/PacketInputStream.h"

#include <iostream>

using namespace std;
using namespace org::esb::io;
using namespace org::esb::av;
 
int main(){

	
	File file("../Der Blutige Pfad Gottes - German (DVD-Quali).avi");
    FormatInputStream fis(&file);
    PacketInputStream pis(fis.getStream(0));
	pis.skip(155000);
		
	Packet packet;
	int a=0;
	while(a==0){
	    packet=pis.readPacket();
	    if(packet.data==NULL)break;
	    char filename[32];
        sprintf(filename,"/tmp/hive/test.%d.packet",a);
        FileOutputStream out(filename);
        PacketOutputStream pout(&out);
		pout.writePacket(&packet);
//		delete packet;
		a++;	
	}

//	delete packet;


	FileInputStream fileis("/tmp/hive/test.0.packet");
	PacketInputStream pis2(&fileis);
	Packet rp=pis2.readPacket();
        FileOutputStream out("/tmp/hive/test.test.packet");
        PacketOutputStream pout(&out);
		pout.writePacket(&rp);

//	delete rp;
	
	
}