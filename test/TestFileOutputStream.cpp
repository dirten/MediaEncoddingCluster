#include <cppunit/extensions/HelperMacros.h>
#include "org/esb/io/File.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/io/FileInputStream.h"
using namespace org::esb::io;
using namespace std;

class TestFileOutputStream: public CppUnit::TestFixture
{

    CPPUNIT_TEST_SUITE(TestFileOutputStream);
    CPPUNIT_TEST(testSimple);
    CPPUNIT_TEST(testSingleByte);
    CPPUNIT_TEST(writeTestConfig);
    CPPUNIT_TEST_SUITE_END();
    
    public:
	void setUp();
	void tearDown();
	void testSimple();
	void testSingleByte();
	void writeTestConfig();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestFileOutputStream);


void TestFileOutputStream::setUp(){
}


void TestFileOutputStream::tearDown(){

}



void TestFileOutputStream::testSimple(){

    FileOutputStream out("test.file");
    char * tmp="bla\ntest";
    out.write(tmp, strlen(tmp));
    out.close();


    FileInputStream in("test.file");
    int bytes=in.available();
    char buffer[bytes+1];
    memset(&buffer,0,bytes+1);
    in.read((unsigned char *)&buffer, bytes);
    CPPUNIT_ASSERT_EQUAL(string(tmp),string(buffer));    
}

void TestFileOutputStream::testSingleByte(){

    FileOutputStream * out=new FileOutputStream("test.file");
    char * tmp="bla\ntest";
    out->write(tmp, strlen(tmp));
    delete out;


    FileInputStream * in=new FileInputStream(&File("test.file"));

    char  buffer;
    string read;
    while(in->available()>0){
        in->read((unsigned char *)&buffer, 1);
        read+=buffer;
    }

    CPPUNIT_ASSERT(strcmp((char *)read.c_str(),tmp)==0);    
    delete in;
    
}
void TestFileOutputStream::writeTestConfig(){
    FileOutputStream out("test.conf");
    char * tmp="src.path=test/path";
    out.write(tmp, strlen(tmp));
    out.close();


}



