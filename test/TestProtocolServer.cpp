#include <cppunit/extensions/HelperMacros.h>
#include "org/esb/io/FileInputStream.h"
#include "org/esb/net/ServerSocket.h"
#include "org/esb/net/Socket.h"
#include "org/esb/lang/Runnable.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/hive/ProtocolServer.h"
using namespace std;
using namespace org::esb::lang;
using namespace org::esb::io;
using namespace org::esb::net;
using namespace org::esb::hive;

class SocketThread:public Runnable{
    public:
	ServerSocket * server;
        void run(){
	    server=new ServerSocket(20000);
	    server->bind();
	    if(Socket * clientSocket=server->accept()){
		cout << "Client here"<<endl;
		ProtocolServer *protoServer=new ProtocolServer(clientSocket);
		Thread thread(protoServer);
		thread.start();
	    }

	    delete server;
	    Thread::sleep(1000);
        }	
};

class TestProtocolServer: public CppUnit::TestFixture
{

    CPPUNIT_TEST_SUITE(TestProtocolServer);
    CPPUNIT_TEST(testConnect);
    CPPUNIT_TEST(testShowConfig);
    CPPUNIT_TEST_SUITE_END();
    
    public:
	TestProtocolServer();
	~TestProtocolServer();
	void setUp();
	void tearDown();
	void testConnect();
	void testShowConfig();
	SocketThread * server;
	Thread *serverThread;

};


CPPUNIT_TEST_SUITE_REGISTRATION(TestProtocolServer);

TestProtocolServer::TestProtocolServer(){
}
TestProtocolServer::~TestProtocolServer(){
}

void TestProtocolServer::setUp(){
    server=new SocketThread();
    serverThread=new Thread(server);
    serverThread->start();
    Thread::sleep(500);
}


void TestProtocolServer::tearDown(){
    delete server;
    delete serverThread;

}



void TestProtocolServer::testConnect(){
    Socket *client=new Socket("localhost",20000);
    client->close();
}

void TestProtocolServer::testShowConfig(){
    Socket *client=new Socket("localhost",20000);
    client->close();
}

