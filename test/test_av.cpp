#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/Packet.h"




using namespace org::esb::io;
using namespace org::esb::av;

using namespace std;
int main(int argc, char ** argv){
	int loop=15;
	while(loop-->0){
	std::cout <<"loop "<<loop<<endl;

//	File f("/media/TREKSTOR/videos/20070401 0140 - PREMIERE 3 - Ein Duke kommt selten allein (The Dukes of Hazzard).ts");
//	File f("/media/TREKSTOR/videos/20070401 1825 - PREMIERE 3 - Wes Craven prÃ¤sentiert Dracula III_ Legacy (Dracula III_ Legacy).ts");
	File f("/media/TREKSTOR/videos/ChocolateFactory.ts");
//	File f("m:\\video\\20070401 0140 - PREMIERE 3 - Ein Duke kommt selten allein (The Dukes of Hazzard).ts");
	FormatInputStream fis(&f);
	PacketInputStream pis(&fis);
	int a=0;
        bool print=false;
        Packet p;
        while(true||a<10000){
		if(pis.readPacket(p)<0)break;
		if(++a%1000==0){
			std::cout<<"\r"<<a;
			std::cout.flush();
		}
//                if(p.packet->dts<0)
                if(p.packet->size>1000000)
                  print=true;
                if(print){
                  std::cout<<"Packet DTS"<<p.packet->dts;
                  std::cout<<"Packet PTS"<<p.packet->pts;
                  std::cout<<std::endl;
                }
	}
	std::cout << endl;
	}
}

