
#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/FormatOutputStream.h"
#include "org/esb/av/Frame.h"
#include "org/esb/av/Packet.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/PacketOutputStream.h"
#include "org/esb/av/CodecOutputStream.h"
#include "org/esb/av/CodecInputStream.h"
#include "org/esb/av/AVInputStream.h"
#include <avcodec.h>

using namespace std;
using namespace org::esb::io;
using namespace org::esb::av;




int main(){
    cout << LIBAVCODEC_IDENT <<endl;

    File file_in("/tmp/Der Blutige Pfad Gottes - German (DVD-Quali).avi");
    File file_out("/tmp/test.avi");

    FormatInputStream fis(&file_in);
    PacketInputStream pis(&fis);



    AVFormatContext * c=fis.getFormatContext();


    AVInputStream *avis=fis.getAVStream(1);
    AVCodecContext * ctx=avis->getCodec();
//    avcodec_open(c->streams[0]->codec);
//    cout << "Codec:"<<c->streams[1]->codec->codec_id<<endl;


    cout << "Codec:"<<av_q2d(avis->r_frame_rate)<<endl;
    cout << "Codec:"<<c->duration<<endl;
    cout << "Codec:"<<c->file_size<<endl;
    cout << "Codec:"<<c->bit_rate<<endl;
    cout << "Codec:"<<ctx->bit_rate<<endl;
    


    

//    FormatOutputStream fos(&file_out);
//    PacketOutputStream pos(&fos);
    
    int count=0;
/*
    while(true&&count < 200000){
	Packet packet;

	pis.readPacket(packet);
	pos.writePacket(packet);

	if(++count%10000==0)
	    cout << "Packets:"<<count<<endl;
    }
*/
//    fis.close();
//    fos.close();
}

