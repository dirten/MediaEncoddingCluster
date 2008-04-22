#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/FormatOutputStream.h"
#include "org/esb/av/PacketOutputStream.h"
#include "org/esb/av/Decoder.h"
#include "org/esb/av/Encoder.h"
#include "org/esb/av/Frame.h"
#include "org/esb/io/File.h"
#include <iostream>
using namespace org::esb::av;
using namespace org::esb::io;
using namespace std;



int main(int argc, char ** argv){

    File f(argv[1]);
    FormatInputStream fis(&f);
    PacketInputStream pis(&fis);

    File fout("/tmp/test.mp2");
    FormatOutputStream fos(&fout);
    PacketOutputStream pos(&fos);

    Decoder dec(CODEC_ID_MP3);
    dec.setSampleRate(44100);
    dec.setBitRate(128000);
    dec.setChannels(2);
    dec.open();
//    cout << "Sample Size"<<dec.frame_size<<endl;

    Encoder enc(CODEC_ID_MP2);
    enc.setSampleRate(44100);
    enc.setBitRate(128000);
    enc.setChannels(2);
    enc.open();

    pos.setEncoder(enc);
    pos.init();    
//    cout << "Sample Size"<<enc.frame_size<<endl;
    Packet pkt;
    while(true){
      if(pis.readPacket(pkt)<0)break;
    	
	  if(pkt.getStreamIndex()==1){
//	    cout << "Decode"<<endl;
	    Frame f=dec.decode(pkt);
        Frame tmp;
//        tmp=f;


//	    cout << "DecodedSize:"<<f._size<<endl;
	/*
	Encoder enc2(CODEC_ID_MP2);
	enc2.setSampleRate(44100);
	enc2.setBitRate(128000);
	enc2.setChannels(2);
	enc2.open();
*/
	    Packet p=enc.encode(f);
//	    cout << "PacketPts:"<<p.pts<<"\tPacketDts:"<<p.dts<<endl;
	    pos.writePacket(p);
	}
    }


    return 0;
}
