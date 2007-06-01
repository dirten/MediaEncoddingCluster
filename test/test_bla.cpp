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

//    avis->setStreamIndex(0);


//    Frame * frame1=avis->getFrame(1700);
    
    Frame * frame;
    int i=0;
    while((frame=avis->getNextFrame())!=NULL){
        cout <<"FrameCounter:"<<i<< "\tFrameWidth"<<frame->getFrame()->linesize[0]<<endl;
        i++;
        char filename[32];
        sprintf(filename,"test.%d.ppm",i);

        FileOutputStream *out=new FileOutputStream(filename);
        FrameOutputStream *fout=new FrameOutputStream(out);
        char header[200];
        sprintf(header, "P6\n%d %d\n255\n", frame->getFrame()->linesize[0], frame->getFrame()->linesize[1]);
        fout->write(header, strlen(header));
        fout->writeFrame(frame);
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
    
    */
//    Frame * frame1=avis->getNextFrame();
//    cout << "FrameSize"<<frame1->getSize()<<endl;

    delete frame;
    delete avis;
    delete fis;
    delete file;






}