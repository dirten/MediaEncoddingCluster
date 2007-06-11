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
    CPPUNIT_TEST_SUITE_END();
    
    public:
	void setUp();
	void tearDown();
	void testSimple();
	void testSingleByte();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestFileOutputStream);


void TestFileOutputStream::setUp(){
}


void TestFileOutputStream::tearDown(){

}



void TestFileOutputStream::testSimple(){

    FileOutputStream * out=new FileOutputStream("test.file");
    char * tmp="bla\ntest";
    out->write(tmp, strlen(tmp));
    delete out;


    FileInputStream * in=new FileInputStream(&File("test.file"));
    int bytes=in->available();

    char * buffer=new char[bytes+1];
    memset(buffer,0,bytes+1);
    in->read((unsigned char *)buffer, bytes);
    CPPUNIT_ASSERT(strcmp(tmp,buffer)==0);    
    delete []buffer;
    delete in;
    
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
