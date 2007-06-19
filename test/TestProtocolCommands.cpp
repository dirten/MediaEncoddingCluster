#include <cppunit/extensions/HelperMacros.h>
#include "org/esb/hive/protocol/Help.cpp"
#include "org/esb/hive/protocol/Disconnect.cpp"
#include "org/esb/io/CharArrayInputStream.h"
#include "org/esb/io/CharArrayOutputStream.h"
using namespace std;
using namespace org::esb::io;

class TestProtocolCommands: public CppUnit::TestFixture
{

    CPPUNIT_TEST_SUITE(TestProtocolCommands);
    CPPUNIT_TEST(testHelp);
    CPPUNIT_TEST(testDisconnect);
    CPPUNIT_TEST_SUITE_END();
    
    public:
	void setUp();
	void tearDown();
	void testHelp();
	void testDisconnect();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestProtocolCommands);


void TestProtocolCommands::setUp(){

}


void TestProtocolCommands::tearDown(){

}



void TestProtocolCommands::testHelp(){
    InputStream * is=new CharArrayInputStream("help", strlen("help"));
    OutputStream * os=new CharArrayOutputStream();
    Help help(is, os);
    int r=help.isResponsible("help");
    CPPUNIT_ASSERT_EQUAL(CMD_HELP, r);
    delete is;
    delete os;
}
void TestProtocolCommands::testDisconnect(){
    InputStream * is=new CharArrayInputStream("help", strlen("help"));
    OutputStream * os=new CharArrayOutputStream();
    Disconnect cmd(is, os);
    int r=cmd.isResponsible("disconnect");
    CPPUNIT_ASSERT_EQUAL(CMD_PROCESS, r);
    cmd.process("");
    delete is;
    delete os;
}
