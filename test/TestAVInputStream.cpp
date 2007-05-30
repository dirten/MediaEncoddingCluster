#include <cppunit/extensions/HelperMacros.h>
#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/Codec.h"
#include "avformat.h"
using namespace org::esb::io;
using namespace org::esb::av;
using namespace std;

class TestAVInputStream: public CppUnit::TestFixture
{

    CPPUNIT_TEST_SUITE(TestAVInputStream);
    CPPUNIT_TEST(testConstructor);
    CPPUNIT_TEST_SUITE_END();
    
    public:
	void setUp();
	void tearDown();
	void testConstructor();
    private:
	File * file;
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestAVInputStream);


void TestAVInputStream::setUp(){
    file=new File("../Der Blutige Pfad Gottes - German (DVD-Quali).avi");
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
