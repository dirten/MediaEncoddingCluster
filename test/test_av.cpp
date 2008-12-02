#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/Packet.h"




using namespace org::esb::io;
using namespace org::esb::av;

using namespace std;
int main(int argc, char ** argv){
	int loop=3;
	while(loop-->0){
	std::cout <<"loop "<<loop<<endl;
//	File f("/media/share/20070401 0140 - PREMIERE 3 - Ein Duke kommt selten allein (The Dukes of Hazzard).ts");
	File f("m:\\video\\20070401 0140 - PREMIERE 3 - Ein Duke kommt selten allein (The Dukes of Hazzard).ts");
	FormatInputStream fis(&f);
	PacketInputStream pis(&fis);
	int a=0;
	while(a<10000){
		Packet p;
		if(pis.readPacket(p)<0)break;
		if(++a%1000==0){
			std::cout<<"\r"<<a;
			std::cout.flush();
		}
	}
	std::cout << endl;
	}
}

