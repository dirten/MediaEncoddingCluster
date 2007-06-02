#include "org/esb/io/File.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/av/FrameOutputStream.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/AVInputStream.h"
#include "org/esb/av/Frame.h"


using namespace std;
using namespace org::esb::io;
using namespace org::esb::av;

int main(){

//    File *file=new File("../Der Blutige Pfad Gottes - German (DVD-Quali).avi");
    File *file=new File("test.avi");

    FormatInputStream *fis=new FormatInputStream(file);

    AVInputStream * avis=new AVInputStream(fis,0);
    AVCodecContext * cc=avis->getCodec()->getCodecContext();
//    avis->setStreamIndex(0);


//    Frame * frame1=avis->getFrame(1700);
//    avis->getFrame(2550);
    Frame * frame;
    int i=0;
    while((frame=avis->getNextFrame())!=NULL){
        cerr <<"FrameCounter:"<<i<< "\tFrameWidth"<<frame->getFrame()->linesize[0]<<endl;
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
        img_convert((AVPicture *)pFrameRGB, PIX_FMT_RGB24, (AVPicture*)tmpFrame, cc->pix_fmt, cc->width, cc->height);
//        frame->setFrame(pFrameRGB);
        int width=cc->width;
        int height=cc->height;
        cout <<"CCWidth="<<width;
        cout <<"CCHeight="<<height<<endl;
        int size=width*height;
        cout <<"FileSize="<<size<<endl;

        cout <<"LineSize[0]="<<pFrameRGB->linesize[0]<<endl;
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