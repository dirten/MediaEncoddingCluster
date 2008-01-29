
#include "org/esb/io/File.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/av/FrameOutputStream.h"
#include "org/esb/av/FrameInputStream.h"
#include "org/esb/av/FormatInputStream.h"
//#include "org/esb/av/AVInputStream.h"
#include "org/esb/av/Frame.h"
#include "org/esb/av/Packet.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/CodecOutputStream.h"
#include "org/esb/av/CodecInputStream.h"


using namespace std;
using namespace org::esb::io;
using namespace org::esb::av;

int main(){
    cout << LIBAVCODEC_IDENT <<endl;

    File file("/tmp/Der Blutige Pfad Gottes - German (DVD-Quali).avi");
    FormatInputStream fis(&file);
    PacketInputStream pis(&fis);
    int count=0;
    while(true&&count < 200000){
	Packet packet;
	pis.readPacket(packet);
	count++;
	cout << count << endl;
    }

}

