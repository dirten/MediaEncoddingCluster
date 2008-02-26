#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/Decoder.h"
#include "org/esb/av/Encoder.h"
#include "org/esb/av/Frame.h"
#include "org/esb/io/File.h"
#include <iostream>
using namespace org::esb::av;
using namespace org::esb::io;
using namespace std;



int main(){
    File f("/tmp/Der Blutige Pfad Gottes - German (DVD-Quali).avi");
    FormatInputStream fis(&f);
    PacketInputStream pis(&fis);

    Decoder dec(CODEC_ID_MP3);
    dec.setSampleRate(44100);
    dec.setChannels(2);
    dec.open();

    Encoder enc(CODEC_ID_MP2);
    enc.setSampleRate(44100);
    enc.setChannels(2);
    enc.open();

    Packet pkt;
    while(true){
    	pis.readPacket(pkt);
	if(pkt.getStreamIndex()==1){
//	    cout << "Decode"<<endl;
	    Frame f=dec.decode(pkt);
	    Packet p=enc.encode(f);
	}
    }


    return 0;
}
