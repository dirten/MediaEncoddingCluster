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
	ServerSocket * serverSocket;

	~SocketThread2(){
	    delete serverSocket;	
	}

        void run(){
        cout << "Waiting for client"<<endl;
	    serverSocket=new ServerSocket(PORT);
	    serverSocket->bind();
	    if(Socket * clientSocket=serverSocket->accept()){
		    cerr << "Client here"<<endl;
		    ProtocolServer *protoServer=new ProtocolServer(clientSocket);
        	    protoServer->run();
//		    delete protoServer;
//		    Thread thread(protoServer);
//		    thread.start();
//		    thread.setAutoDelete(true);
//		    Thread::sleep(2000);
//		    delete clientSocket;
	    }
    }	
};

class TestProtocolServer: public CppUnit::TestFixture
{

    CPPUNIT_TEST_SUITE(TestProtocolServer);
    CPPUNIT_TEST(testSimple);
    CPPUNIT_TEST(testConnect);
    CPPUNIT_TEST(testShowConfig);
    CPPUNIT_TEST_SUITE_END();
    
    public:
	TestProtocolServer();
	~TestProtocolServer();
	void setUp();
	void tearDown();
	void testConnect();
	void testSimple();
	void testShowConfig();
	SocketThread2 * server;
	Thread *serverThread;

};


//CPPUNIT_TEST_SUITE_REGISTRATION(TestProtocolServer);

TestProtocolServer::TestProtocolServer(){
    Config::init("cluster.cfg");
}
TestProtocolServer::~TestProtocolServer(){
}

void TestProtocolServer::setUp(){
    
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
void TestProtocolServer::testSimple(){
    
}

void TestProtocolServer::testShowConfig(){
    Socket *client=new Socket("localhost",PORT);
    client->connect();
    Thread::sleep(1000);
    client->close();
    delete client;
}

