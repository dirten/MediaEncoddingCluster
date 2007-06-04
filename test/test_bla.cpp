#include "org/esb/io/File.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/av/FrameOutputStream.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/AVInputStream.h"
#include "org/esb/av/Frame.h"
#include "org/esb/av/Packet.h"
#include "org/esb/av/PacketInputStream.h"


using namespace std;
using namespace org::esb::io;
using namespace org::esb::av;

int main(){
//    File *file=new File("../Der Blutige Pfad Gottes - German (DVD-Quali).avi");
//    PacketInputStream *pis=new PacketInputStream(NULL);
    File *file=new File("test.avi");
    FormatInputStream *fis=new FormatInputStream(file);
//    AVInputStream * avis=new AVInputStream(fis,0);
    PacketInputStream *pis=new PacketInputStream(fis->getStream(0));
    


//    avis->getFrame(1);
    Codec * codec=pis->getCodec();
//    Codec * codec=avis->getCodec();
    int a=0;
    int duration=pis->getDuration();
//    int duration=avis->getDuration();
//    Frame * frame;
//    AVPacket  packet;
    Packet * packet=NULL;
    Frame * frame;
    while((packet=pis->readPacket())!=NULL&&a<100){
//        frame=avis->getNextFrame();
        frame=new Frame(packet, codec->getCodecContext());
//        delete frame;
//    while(a<100){
//        Packet * packet=pis->readPacket();
        
        if((packet->flags & PKT_FLAG_KEY)){
            cout << "KeyFrame@"<<a<<" from "<<duration<<endl;
//            cout << "KeyFrame@"<<a<<"Position"<<packet->getPosition()<<" from "<<duration<<endl;
        }

/*
	    frame=new Frame(&packet, codec->getCodecContext());
        if((packet->flags & PKT_FLAG_KEY)){
        if(frame->getFrame()->key_frame>0){
            cout << "KeyFrame@"<<a<<" from "<<duration<<endl;
        }
        delete frame;
        */
//        av_free_packet(packet);
        /*
        if(packet){
            free(packet->data);
            delete packet;
        }
        */
        a++;
            delete packet;
    }
//    delete codec;
    delete packet;
    delete codec;
    delete pis;
    delete fis;
    delete file;
}
int main2(){

//    File *file=new File("../Der Blutige Pfad Gottes - German (DVD-Quali).avi");
    File *file=new File("test.avi");
    if(typeid(file)==typeid(&File(""))){
        cout << "Type is file"<< endl;
    }
    cout << typeid(file).name()<<endl;
    FormatInputStream *fis=new FormatInputStream(file);

    AVInputStream * avis=new AVInputStream(fis,0);
    AVCodecContext * cc=avis->getCodec()->getCodecContext();
//    avis->setStreamIndex(0);


//    Frame * frame1=avis->getFrame(1700);
//    avis->getFrame(2540);
    Frame * frame;
    int i=0;
    while((frame=avis->getNextFrame())!=NULL){
        cerr <<"FrameCounter:"<<i<< "\tFrameWidth"<<frame->getWidth()<< "\tFrameHeight"<<frame->getHeight()<<endl;
        /*
        if(frame->getFrame()->linesize[0]==0||true){
            cout << "Error:"<<frame->getFrame()->error[0]<<endl;
            cout << "Error:"<<frame->getFrame()->error[1]<<endl;
            cout << "Error:"<<frame->getFrame()->error[2]<<endl;
            cout << "Error:"<<frame->getFrame()->error[3]<<endl;
        }*/
        i++;
        char filename[32];
        sprintf(filename,"test.%d.ppm",i);

        FileOutputStream *out=new FileOutputStream(filename);

        FrameOutputStream *fout=new FrameOutputStream(out);



        AVFrame *pFrameRGB=avcodec_alloc_frame();

        int numBytes=avpicture_get_size(PIX_FMT_RGB24, cc->width,cc->height);
        uint8_t * buffer=new uint8_t[numBytes];

    // Assign appropriate parts of buffer to image planes in pFrameRGB
        avpicture_fill((AVPicture *)pFrameRGB, buffer, PIX_FMT_RGB24, cc->width, cc->height);

        AVFrame * tmpFrame=frame->getFrame();
        if(frame->getHeight()==0)continue;
        img_convert((AVPicture *)pFrameRGB, PIX_FMT_RGB24, (AVPicture*)tmpFrame, cc->pix_fmt, cc->width, cc->height);
        frame->setFrame(pFrameRGB);
        int width=cc->width;
        int height=cc->height;
        cout <<"CCWidth="<<width;
        cout <<"CCHeight="<<height<<endl;
        int size=width*height;
        cout <<"FileSize="<<size<<endl;
        char header[200];
        sprintf(header, "P6\n%d %d\n255\n", width, height);

        fout->write(header, strlen(header));
        for(int y=0; y<height; y++)
            fout->write((char *)pFrameRGB->data[0]+y*pFrameRGB->linesize[0], width*3);
            
//        fwrite(frame->data[0]+y*frame->linesize[0], 1, codecCtx->width*3, pFile);

//        fout->writeFrame(frame);
//        fout->write((char *)pFrameRGB, 16000);
        delete fout;
        delete out;
        delete frame;
    }

    cout <<"While("<<i<<")"<<endl;

/*    cout << "FrameWidth"<<frame->getFrame()->linesize[0]<<endl;
    cout << "FrameHeight"<<frame->getFrame()->linesize[1]<<endl;
//    cout << "FrameData"<<frame1->getFrame()->data[0]<<endl;
    cout << "TimeBase"<<avis->getTimeBase()<<endl;
    cout << "dURATION"<<avis->getDuration()<<endl;
    Frame * frame1=avis->getFrame(100);
    */
//    Frame * frame1=avis->getNextFrame();
//    cout << "FrameSize"<<frame1->getSize()<<endl;

    delete frame;
    delete avis;
    delete fis;
    delete file;






}