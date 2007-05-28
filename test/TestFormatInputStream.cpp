#include <cppunit/extensions/HelperMacros.h>
#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
using namespace org::esb::io;

class TestFormatInputStream: public CppUnit::TestFixture
{

    CPPUNIT_TEST_SUITE(TestFormatInputStream);
    CPPUNIT_TEST(testConstructor);
    CPPUNIT_TEST_SUITE_END();
    
    public:
//	void setUp();
//	void tearDown();
	void testConstructor();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestFormatInputStream);

/*
void TestFormatInputStream::setUp(){


}


void TestFormatInputStream::tearDown(){


}
*/


void TestFormatInputStream::testConstructor(){
    File * file=new File("test/test_test.cpp");
    CPPUNIT_ASSERT(file);
    CPPUNIT_ASSERT(file->getPath()=="test/test_test.cpp");
    FormatInputStream *fis=new FormatInputStream(file);
    
    
    delete fis;

    delete file;
}
