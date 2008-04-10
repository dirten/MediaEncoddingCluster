#include <cppunit/extensions/HelperMacros.h>
#include "org/esb/hive/protocol/Help.cpp"
#include "org/esb/hive/protocol/Disconnect.cpp"
#include "org/esb/hive/protocol/ShowConfig.cpp"
#include "org/esb/hive/protocol/ShutdownHive.cpp"
#include "org/esb/hive/protocol/StartupHive.cpp"
#include "org/esb/hive/protocol/Status.cpp"
#include "org/esb/hive/protocol/Unknown.cpp"
#include "org/esb/io/CharArrayInputStream.h"
#include "org/esb/io/CharArrayOutputStream.h"
using namespace std;
using namespace org::esb::io;

class TestProtocolCommands: public CppUnit::TestFixture
{

    CPPUNIT_TEST_SUITE(TestProtocolCommands);
    CPPUNIT_TEST(testHelp);
    CPPUNIT_TEST(testDisconnect);
    CPPUNIT_TEST(testShowConfig);
//    CPPUNIT_TEST(testShutdownHive);
    CPPUNIT_TEST(testStartupHive);
    CPPUNIT_TEST(testStatus);
    CPPUNIT_TEST(testUnknown);
    CPPUNIT_TEST_SUITE_END();
    
    public:
	void setUp();
	void tearDown();
	void testHelp();
	void testDisconnect();
	void testShowConfig();
	void testShutdownHive();
	void testStartupHive();
	void testStatus();
	void testUnknown();
};

//CPPUNIT_TEST_SUITE_REGISTRATION(TestProtocolCommands);


void TestProtocolCommands::setUp(){

}


void TestProtocolCommands::tearDown(){

}



void TestProtocolCommands::testHelp(){
    CharArrayInputStream is("help", strlen("help"));
    CharArrayOutputStream os;
    Help help(&is, &os);
    CPPUNIT_ASSERT_EQUAL(CMD_HELP, help.isResponsible("help"));
    CPPUNIT_ASSERT_EQUAL(CMD_NA, help.isResponsible("bla"));
}

void TestProtocolCommands::testDisconnect(){
    CharArrayInputStream is("help", strlen("help"));
    CharArrayOutputStream os;
    Disconnect cmd(&is, &os);
    CPPUNIT_ASSERT_EQUAL(CMD_PROCESS, cmd.isResponsible("disconnect"));
    CPPUNIT_ASSERT_EQUAL(CMD_HELP, cmd.isResponsible("help"));
    CPPUNIT_ASSERT_EQUAL(CMD_NA, cmd.isResponsible("sülz"));
    cmd.process("bla sjgfj sdjfg sd");
}

void TestProtocolCommands::testShowConfig(){
    Config::init("cluster.cfg");
    CharArrayInputStream is("help", strlen("help"));
    CharArrayOutputStream os;
    ShowConfig cmd(&is, &os);
    CPPUNIT_ASSERT_EQUAL(CMD_HELP, cmd.isResponsible("help"));
    CPPUNIT_ASSERT_EQUAL(CMD_NA, cmd.isResponsible("bla"));
    cmd.process("bla sjgfj sdjfg sd");
    Config::close();
}

void TestProtocolCommands::testStartupHive(){
    Config::init("cluster.cfg");
    CharArrayInputStream is("help", strlen("help"));
    CharArrayOutputStream os;
    StartupHive cmd(&is, &os);
    CPPUNIT_ASSERT_EQUAL(CMD_HELP, cmd.isResponsible("help"));
    CPPUNIT_ASSERT_EQUAL(CMD_PROCESS, cmd.isResponsible("startup"));
    CPPUNIT_ASSERT_EQUAL(CMD_NA, cmd.isResponsible("bla"));
//    cmd.process("bla sjgfj sdjfg sd");
    Config::close();
}
void TestProtocolCommands::testShutdownHive(){
    Config::init("cluster.cfg");
    CharArrayInputStream is("help", strlen("help"));
    CharArrayOutputStream os;
    ShutdownHive cmd(&is, &os);
    CPPUNIT_ASSERT_EQUAL(CMD_HELP, cmd.isResponsible("help"));
    CPPUNIT_ASSERT_EQUAL(CMD_PROCESS, cmd.isResponsible("shutdown"));
    CPPUNIT_ASSERT_EQUAL(CMD_NA, cmd.isResponsible("bla"));
    cmd.process("bla sjgfj sdjfg sd");
    Config::close();
}
void TestProtocolCommands::testStatus(){
    Config::init("cluster.cfg");
    CharArrayInputStream is("help", strlen("help"));
    CharArrayOutputStream os;
    Status cmd(&is, &os);
    CPPUNIT_ASSERT_EQUAL(CMD_HELP, cmd.isResponsible("help"));
    CPPUNIT_ASSERT_EQUAL(CMD_PROCESS, cmd.isResponsible("show status"));
    CPPUNIT_ASSERT_EQUAL(CMD_NA, cmd.isResponsible("bla"));
//    cmd.process("bla sjgfj sdjfg sd");
    Config::close();
}
void TestProtocolCommands::testUnknown(){
    Config::init("cluster.cfg");
    CharArrayInputStream is("help", strlen("help"));
    CharArrayOutputStream os;
    Unknown cmd(&is, &os);
    CPPUNIT_ASSERT_EQUAL(CMD_PROCESS, cmd.isResponsible("show status"));
    CPPUNIT_ASSERT_EQUAL(CMD_NA, cmd.isResponsible("help"));
    cmd.process("bla sjgfj sdjfg sd");
    Config::close();
}
