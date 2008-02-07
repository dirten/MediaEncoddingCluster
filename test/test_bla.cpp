#include "org/esb/net/Socket.h"
#include "org/esb/io/File.h"
#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/AVInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/Packet.h"
#include "org/esb/av/Frame.h"
#include "org/esb/av/Encoder.h"
#include "org/esb/av/Decoder.h"
#include "org/esb/hive/job/ProcessUnit.h"
#include <iostream>
#include <stdexcept>
#include "org/esb/sql/sqlite3x.hpp"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/ResultSet.h"
#include <boost/shared_ptr.hpp>
using namespace std;
using namespace org::esb;
using namespace org::esb::net;
using namespace org::esb::av;
using namespace org::esb::hive::job;
using namespace sqlite3x;


int main(){


    Socket sock("localhost", 20000);
    sock.connect();
    ObjectInputStream ois(sock.getInputStream());
    int size=0;
    File f("/tmp/Der Blutige Pfad Gottes - German (DVD-Quali).avi");
//    FileInputStream is(&f);
    FormatInputStream fis(&f);
    AVInputStream * ais=fis.getAVStream(0);
    Decoder *decoder=(Decoder*)ais->getCodec();
/*
    cout << "CodecId MPEG:"<<CODEC_ID_MPEG2VIDEO<<endl;
    Codec *encoder=new Codec(CODEC_ID_MPEG2VIDEO,2);
    encoder->width=decoder->width;
    encoder->height=decoder->height;
    encoder->flags|= CODEC_FLAG_GLOBAL_HEADER;
    encoder->open(Codec::ENCODER);
//    encoder->initDefaults();
    cout << "width:"<<encoder->coded_width<<endl;
    cout << "height:"<<encoder->coded_height<<endl;
    cout << "width:"<<encoder->width<<endl;
    cout << "height:"<<encoder->height<<endl;
*/
    

//    Codec *codec=ais->getCodec();
//    codec->open(Codec::DECODER);
//    cout << "CodecInit:"<<decoder->codec->id<<endl;
    decoder->open(Codec::DECODER);
    
    PacketInputStream pis(&fis);
    Packet p;
    while(true){
	pis.readPacket(p);
	if(p.getStreamIndex()==0){
	    if(p.pts>100){
		Frame * f=decoder->decode(p);
/*		if(strlen((char*)f->getData())>0)
		    cout << "FrameData:"<<f->getData()<<endl;
*/	    
//		Packet * pac=encoder->encodeFrame(*f);
		if(p.pts<200)
    		    break;
    	    }
	}
    }
/*    
    while(true){
	char * text="get process_unit";
	sock.getOutputStream()->write(text, strlen(text));
//	cout << "hier"<<endl;
	ProcessUnit unit;
	ois.readObject(unit);
	list< boost::shared_ptr<Packet> >::iterator it; 
	if(unit._input_packets.size()==0)break;
//	boost::shared_ptr<Packet> p=unit._input_packets.front();
//	p->size;
	for(it=unit._input_packets.begin();it!=unit._input_packets.end();it++){
	    boost::shared_ptr<Packet> p=*it;
	    size+=p->size;
	    cout << "new Frame size:"<<p->size<<endl;
	    Frame * f=codec->decode(*p.get());
	    Packet * pac=encoder->encodeFrame(*f);
//	    encoder->encodeFrame(*p.get());
	}
	
	cout << "\rDataSize="<<size/1024/1024;
	cout.flush();
    }
    */
}

