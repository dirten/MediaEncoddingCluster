#include <cppunit/extensions/HelperMacros.h>
#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include <iostream>
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

//CPPUNIT_TEST_SUITE_REGISTRATION(TestFormatInputStream);


void TestFormatInputStream::setUp(){
    file=new File("863_spongebong3-illustrated.wmv");
}


void TestFormatInputStream::tearDown(){
    delete file;

}



void TestFormatInputStream::testConstructor(){
    CPPUNIT_ASSERT(file);
    CPPUNIT_ASSERT(file->canRead());
    FormatInputStream fis(file);
//    FormatInputStream *fis2=new FormatInputStream(file);
    CPPUNIT_ASSERT(fis.getStreamCount()==2);
/*
    PacketInputStream pis(fis.getStream(1));
    int a=0;
    Packet p;
    while(true){
	pis.readPacket(p);
	if(p.data==NULL){
	    break;
	}
	++a;
//	if(a%100==0)
//	    cout <<"\r"<< a << "Packet";
//	cout.flush();
    }
*/
}
