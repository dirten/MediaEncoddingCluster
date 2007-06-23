#include <cppunit/extensions/HelperMacros.h>
#include "org/esb/io/FileInputStream.h"
#include "org/esb/net/ServerSocket.h"
#include "org/esb/net/Socket.h"
#include "org/esb/lang/Runnable.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/hive/CommandInputStream.h"
using namespace std;
using namespace org::esb::lang;
using namespace org::esb::io;
using namespace org::esb::net;
using namespace org::esb::hive;

class TestCommandInputStream: public CppUnit::TestFixture
{

    CPPUNIT_TEST_SUITE(TestCommandInputStream);
    CPPUNIT_TEST(testOverFileInputStream);
//    CPPUNIT_TEST(testOverSocketInputStream);
    CPPUNIT_TEST_SUITE_END();
    
    public:
	void setUp();
	void tearDown();
	void testOverFileInputStream();
	void testOverSocketInputStream();
};


CPPUNIT_TEST_SUITE_REGISTRATION(TestCommandInputStream);


void TestCommandInputStream::setUp(){
    

}


void TestCommandInputStream::tearDown(){

}



void TestCommandInputStream::testOverFileInputStream(){
    FileInputStream fis("test.command");
    CommandInputStream cis(&fis);
    Command cmd=cis.readCommand();
    string c=cmd.getCommand();
    CPPUNIT_ASSERT_EQUAL(string("show config"),c);
//    delete c;
}


class SocketThread:public Runnable{
    public:
        void run(){
            ServerSocket server(20002);
            server.bind();
            if(Socket * client=server.accept()){
                CPPUNIT_ASSERT(client);
		CommandInputStream cis(client->getInputStream());
		Command cmd=cis.readCommand();
		string c=cmd.getCommand();
//		CPPUNIT_ASSERT(c!=NULL);
		CPPUNIT_ASSERT_EQUAL(string("show config"),c);
//				delete cmd;
		client->close();
                delete client;
            }
            server.close();
        }
	~SocketThread(){}
};

void TestCommandInputStream::testOverSocketInputStream(){
    SocketThread server;
    Thread thread(&server);
    thread.start();
    Thread::sleep(1000);
	Socket socket("localhost",20002);
	socket.connect();
	string temp="show config";
	socket.getOutputStream()->write((char *)temp.c_str(),temp.length());
	socket.close();
	thread.join();

}
