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
//    File *file=new File("/media/jh/Video/aufnahme/AV000001.ASF");
    File *file=new File("../Der Blutige Pfad Gottes - German (DVD-Quali).avi");
//    File *file=new File("test.dvd");
//    PacketInputStream *pis=new PacketInputStream(NULL);
//    File *file=new File("test.avi");
    FormatInputStream *fis=new FormatInputStream(file);
    PacketInputStream *pis=new PacketInputStream(fis->getStream(0));
    pis->skip(200);
    Codec * codec=pis->getCodec();

    FileOutputStream focs("/tmp/hive/1.codec.test");
    CodecOutputStream cos(&focs);
    cos.writeCodec(codec);
    focs.flush();


    FileInputStream fics("/tmp/hive/1.codec.test");
    CodecInputStream cis(&fics);
    Codec * c=cis.readCodec();
    
    c->open();
    
    Codec * c2=new Codec((CodecID)c->getCodecId());
    cout << c->getCodecContext()->bit_rate<<endl;
    int a=0;
//    int duration=pis->getDuration();
    Packet packet;
    Frame * frame;
    while(a<400){
        packet=pis->readPacket();
        if(packet.data==NULL)break;
        if(a%100==0)cout <<"A:"<<a<<endl;
	cout << "PacketSize:"<<packet.getSize()<<endl;
        frame=new Frame(&packet, codec);
	cout <<"FrameFormat"<< frame->getFormat()<<endl;
	cout << "RGBFormat"<<PIX_FMT_RGB24<<endl;
        Frame* rgb=frame->getFrame(PIX_FMT_RGB24);
        /*
//        cout << "FrameSize:"<<frame->getSize()<<endl;
//        cout << "RGBFrameSize:"<<rgb->getSize()<<endl;
        
        */

        char filename[32];
        sprintf(filename,"/tmp/hive/test.%d.ppm",a);

        FileOutputStream *out=new FileOutputStream(filename);
        FrameOutputStream *fout=new FrameOutputStream(out);

        char header[200];
        sprintf(header, "P6\n%d %d\n255\n", rgb->getWidth(), rgb->getHeight());
        fout->write(header, strlen(header));
        fout->writeFrame(rgb);
        delete fout;
        delete out;
        
        a++;
        delete rgb;
        delete frame;
//        delete packet;
    }
	
    FileInputStream * is =new FileInputStream("/tmp/hive/test.70.ppm");
    FrameInputStream * fris=new FrameInputStream(is);
    Frame * fr=fris->readFrame();

    FileOutputStream *out2=new FileOutputStream("/tmp/hive/test.0.ppm");
    FrameOutputStream *fout2=new FrameOutputStream(out2);
    char header[200];
    sprintf(header, "P6\n%d %d\n255\n", fr->getWidth(), fr->getHeight());
    fout2->write(header, strlen(header));
    fout2->writeFrame(fr);
    delete fout2;
    delete out2;
	delete is;
	delete fris;
	delete fr;
	
	    
//    delete packet;
    delete codec;
    delete pis;
    delete fis;
    delete file;
}

