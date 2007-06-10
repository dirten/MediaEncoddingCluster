#include <cppunit/extensions/HelperMacros.h>
#include "org/esb/io/File.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/io/FileInputStream.h"
using namespace org::esb::io;
using namespace std;

class TestFileOutputStream: public CppUnit::TestFixture
{

    CPPUNIT_TEST_SUITE(TestFileOutputStream);
    CPPUNIT_TEST(testConstructor);
    CPPUNIT_TEST_SUITE_END();
    
    public:
	void setUp();
	void tearDown();
	void testConstructor();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestFileOutputStream);


void TestFileOutputStream::setUp(){
}


void TestFileOutputStream::tearDown(){

}



void TestFileOutputStream::testConstructor(){
/*
    FileOutputStream * out=new FileOutputStream("test.file");
    char * tmp="bla\ntest";
    out->write(tmp, strlen(tmp));
    delete out;
*/
/*
    FileInputStream * in=new FileInputStream(&File("test.file"));
    int bytes=in->available();
    char * buffer=new char[bytes];
    in->read((unsigned char *)buffer, bytes);
    CPPUNIT_ASSERT(strcmp(tmp,buffer)==0);    
    delete buffer;
    delete in;
    */
}
