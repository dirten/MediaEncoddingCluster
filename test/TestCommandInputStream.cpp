//#include <cppunit/extensions/HelperMacros.h>
#include "org/esb/io/FileInputStream.h"
#include "org/esb/hive/CommandInputStream.h"
using namespace std;
using namespace org::esb::io;
using namespace org::esb::hive;

class TestCommandInputStream: public CppUnit::TestFixture
{

    CPPUNIT_TEST_SUITE(TestCommandInputStream);
    CPPUNIT_TEST(testConstructor);
    CPPUNIT_TEST_SUITE_END();
    
    public:
	void setUp();
	void tearDown();
	void testConstructor();
};


CPPUNIT_TEST_SUITE_REGISTRATION(TestCommandInputStream);


void TestCommandInputStream::setUp(){
    

}


void TestCommandInputStream::tearDown(){

}



void TestCommandInputStream::testConstructor(){
    FileInputStream fis("test.command");
    CommandInputStream cis(&fis);
    Command * cmd=cis.readCommand();
    

}
