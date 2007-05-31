#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/AVInputStream.h"
#include "org/esb/av/Frame.h"


using namespace std;
using namespace org::esb::io;
using namespace org::esb::av;

int main(){

    File *file=new File("../Der Blutige Pfad Gottes - German (DVD-Quali).avi");

    FormatInputStream *fis=new FormatInputStream(file);    
    AVInputStream * avis=fis->getStream(0);
//    Frame * frame1=avis->getFrame(1700);
    Frame * frame1=avis->getNextFrame();
    cout << "FrameData"<<frame1->getFrame()->linesize[0]<<endl;
    cout << "FrameData"<<frame1->getFrame()->data[0]<<endl;
//    cout << "FrameSize"<<frame1->getSize()<<endl;

//    delete frame1;
    delete avis;
    delete fis;
    delete file;






}