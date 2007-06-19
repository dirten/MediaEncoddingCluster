#include <cppunit/extensions/HelperMacros.h>
#include "org/esb/io/FileInputStream.h"
#include "org/esb/net/ServerSocket.h"
#include "org/esb/net/Socket.h"
#include "org/esb/lang/Runnable.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/config/config.h"
#include "org/esb/hive/ProtocolServer.h"
using namespace std;
using namespace org::esb::config;
using namespace org::esb::lang;
using namespace org::esb::io;
using namespace org::esb::net;
using namespace org::esb::hive;
int PORT=20005;
class SocketThread2:public Runnable{
    public:
	ServerSocket * server;
	~SocketThread2(){
	    delete server;	
	}
        void run(){
        cout << "Waiting for client"<<endl;
	    server=new ServerSocket(PORT);
	    server->bind();
	    if(Socket * clientSocket=server->accept()){
		cerr << "Client here"<<endl;
		ProtocolServer *protoServer=new ProtocolServer(clientSocket);
		Thread thread(protoServer);
		thread.start();
//		thread.setAutoDelete(true);
		Thread::sleep(2000);
//		delete clientSocket;
		delete protoServer;
	    }
        }	
};

class TestProtocolServer: public CppUnit::TestFixture
{

    CPPUNIT_TEST_SUITE(TestProtocolServer);
//    CPPUNIT_TEST(testConnect);
    CPPUNIT_TEST(testShowConfig);
    CPPUNIT_TEST_SUITE_END();
    
    public:
	TestProtocolServer();
	~TestProtocolServer();
	void setUp();
	void tearDown();
	void testConnect();
	void testShowConfig();
	SocketThread2 * server;
	Thread *serverThread;

};


CPPUNIT_TEST_SUITE_REGISTRATION(TestProtocolServer);

TestProtocolServer::TestProtocolServer(){
}
TestProtocolServer::~TestProtocolServer(){
}

void TestProtocolServer::setUp(){
    Config::init("cluster.cfg");
    cerr << "SetUpStart"<<endl;
    server=new SocketThread2();
    serverThread=new Thread(server);
    serverThread->start();
    cerr << "SetUpEnd"<<endl;
    Thread::sleep(1500);

}


void TestProtocolServer::tearDown(){
    delete serverThread;
    delete server;
    Thread::sleep(1500);
}



void TestProtocolServer::testConnect(){
    
    Socket *client=new Socket("localhost",PORT);
    client->connect();
    char * cmd="disconnect";
    client->getOutputStream()->write(cmd, strlen(cmd));
    Thread::sleep(1000);
    client->close();
    delete client;
}

void TestProtocolServer::testShowConfig(){
    Socket *client=new Socket("localhost",PORT);
    client->connect();
    Thread::sleep(1000);
    client->close();
    delete client;
}

