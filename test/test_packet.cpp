#include "org/esb/io/File.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/av/Packet.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/PacketOutputStream.h"
#include "org/esb/av/CodecOutputStream.h"
#include "org/esb/av/Codec.h"
#include "org/esb/av/PacketInputStream.h"
//#include "framehive/FrameHive.h"

#include <iostream>

using namespace std;
using namespace org::esb::io;
using namespace org::esb::av;
 
int main(){

	
	File file("../Der Blutige Pfad Gottes - German (DVD-Quali).avi");
    FormatInputStream fis(&file);
    PacketInputStream pis(fis.getStream(0));
    Codec * codec=pis.getCodec();

//	pis.skip(155000);
			
	Packet packet;
	int a=0, packetCounter=0;
	
//	FrameHive * hive=new FrameHive("test");
	FileOutputStream * fos=NULL;
	PacketOutputStream * pos=NULL;
	CodecOutputStream * cos=NULL;
	while(true){
	    packet=pis.readPacket();
		if(packet.isKeyFrame()){
			packetCounter++;
			char filename[100];
			sprintf(filename,"/tmp/hive/con%04d.data",packetCounter);
			cout << filename<<endl;
			if(fos!=NULL)
				delete fos;
			fos=new FileOutputStream(filename);
			if(pos!=NULL)
				delete pos;
			pos=new PacketOutputStream(fos);
			if(cos!=NULL)
				delete cos;
				
			cos=new CodecOutputStream(fos);
			cos->writeCodec(codec);
			cos->writeCodec(codec);
		}
		pos->writePacket(&packet);
	    if(packet.data==NULL)break;
		
//		hive->putPacket(&packet);
//		if(a%10==0)cout<<"Packet:"<<a<<endl;
		a++;	
	}



	
	
}