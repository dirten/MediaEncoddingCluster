#include <cppunit/extensions/HelperMacros.h>
#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/av/FrameOutputStream.h"

#include "org/esb/av/Codec.h"
#include "avformat.h"
using namespace org::esb::io;
using namespace org::esb::av;
using namespace std;

class TestAVInputStream: public CppUnit::TestFixture
{

    CPPUNIT_TEST_SUITE(TestAVInputStream);
    CPPUNIT_TEST(testConstructor);
    CPPUNIT_TEST(testGetFrame);
    CPPUNIT_TEST_SUITE_END();
    
    public:
	void setUp();
	void tearDown();
	void testConstructor();
	void testGetFrame();
    private:
	File * file;
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestAVInputStream);


void TestAVInputStream::setUp(){
    file=new File("/media/video/Der Blutige Pfad Gottes - German (DVD-Quali).avi");
    CPPUNIT_ASSERT(file);
    CPPUNIT_ASSERT(file->canRead());
}


void TestAVInputStream::tearDown(){
    delete file;

}



void TestAVInputStream::testConstructor(){
    FormatInputStream *fis=new FormatInputStream(file);
    AVInputStream * avis=fis->getStream(0);
    CPPUNIT_ASSERT(avis);
//    cout << "Duration:"<<fis->getFileSize();

//    cout << "Duration:"<<avis->getDuration();
//    cout << "FrameCount:"<<avis->getNumberFrames();
//    cout << "TimeBase:"<<avis->getTimeBase();
/*
    AVInputStream * avis=fis->getStream(1);
    Codec * codec=avis->getCodec();
    cout << "CodecType:"<<codec->getCodecType()<<endl;
    cout << "CodecName:"<<codec->getCodecName()<<endl;
    delete avis;    
*/
    delete avis;
    delete fis;

}

void TestAVInputStream::testGetFrame(){
    FormatInputStream *fis=new FormatInputStream(file);    
    AVInputStream * avis=fis->getStream(0);
    CPPUNIT_ASSERT(avis);
//    Frame * frame1=avis->getFrame(1700);
    Frame * frame1=avis->getNextFrame();
    CPPUNIT_ASSERT(frame1);

    cout << "FrameData"<<frame1->getFrame()->linesize[0]<<endl;
    FileOutputStream * out=new FileOutputStream("test.frame");
    FrameOutputStream *fous=new FrameOutputStream(out);
    fous->writeFrame(frame1);
    
    delete fous;
    delete out;
//    cout << "FrameData"<<frame1->getFrame()->data[0]<<endl;
//    cout << "FrameSize"<<frame1->getSize()<<endl;

//    delete frame1;
    delete avis;
    delete fis;

}


