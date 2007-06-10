//#include <cppunit/extensions/HelperMacros.h>
#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
//#include "org/esb/av/Codec.h"
//#include "avformat.h"
using namespace org::esb::io;
using namespace org::esb::av;
using namespace std;

class TestFormatInputStream: public CppUnit::TestFixture
{

    CPPUNIT_TEST_SUITE(TestFormatInputStream);
    CPPUNIT_TEST(testConstructor);
    CPPUNIT_TEST_SUITE_END();
    
    public:
	void setUp();
	void tearDown();
	void testConstructor();
    private:
	File * file;
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestFormatInputStream);


void TestFormatInputStream::setUp(){
//    file=new File("/media/video/Der Blutige Pfad Gottes - German (DVD-Quali).avi");
    file=new File("test.avi");
}


void TestFormatInputStream::tearDown(){
    delete file;

}



void TestFormatInputStream::testConstructor(){
    CPPUNIT_ASSERT(file);
    CPPUNIT_ASSERT(file->canRead());
    FormatInputStream *fis=new FormatInputStream(file);
    FormatInputStream *fis2=new FormatInputStream(file);
    CPPUNIT_ASSERT(fis->getStreamCount()==2);
/*
    AVInputStream * avis=fis->getStream(1);
    Codec * codec=avis->getCodec();
    cout << "CodecType:"<<codec->getCodecType()<<endl;
    cout << "CodecName:"<<codec->getCodecName()<<endl;
    delete avis;    
*/
    delete fis;
    delete fis2;

}
