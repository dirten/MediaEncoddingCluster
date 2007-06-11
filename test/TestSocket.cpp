#include <cppunit/extensions/HelperMacros.h>
#include "org/esb/net/ServerSocket.h"
#include "org/esb/net/Socket.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/lang/Runnable.h"
//#define new new

using namespace std;
using namespace org::esb::net;
using namespace org::esb::lang;

class TestSocket: public CppUnit::TestFixture
{

    CPPUNIT_TEST_SUITE(TestSocket);
    CPPUNIT_TEST(testConstructor);
//    CPPUNIT_TEST(testSimpleConnect);
    CPPUNIT_TEST(testStream);
    CPPUNIT_TEST_SUITE_END();
    
    public:
	void setUp();
	void tearDown();
	void testConstructor();
	void testSimpleConnect();
    void testStream();
};


CPPUNIT_TEST_SUITE_REGISTRATION(TestSocket);


void TestSocket::setUp(){
    

}


void TestSocket::tearDown(){

}


void TestSocket::testConstructor(){
    ServerSocket server();
    CPPUNIT_ASSERT(server);

    Socket client();
    CPPUNIT_ASSERT(client);
}

class SimpleConnectThread:public Runnable{
    public:
        void run(){
            ServerSocket server(20000);
            server.bind();
            if(Socket * client=server.accept()){
                CPPUNIT_ASSERT(client);
                delete client;
            }
        }
};

void TestSocket::testSimpleConnect(){
    SimpleConnectThread  server;
    Thread thread(&server);
    thread.start();
    Thread::sleep(500);
    Socket socket("localhost",20000);
    socket.connect();
    thread.join();
}

class StreamThread:public Runnable{
    public:
        void run(){
            ServerSocket server(20001);
            server.bind();
            if(Socket * client=server.accept()){
                client->getOutputStream()->write("0123456789\0",11);
                unsigned char buffer[11];
                client->getInputStream()->read(buffer,11);
//                cout << "Buffer:"<<buffer<<":"<<endl;
                CPPUNIT_ASSERT(strcmp((char *)buffer,"0123456789")==0);
                delete client;
            }
            server.close();
        }
};

void TestSocket::testStream(){

    StreamThread * server=new StreamThread();
    Thread * thread=new Thread(server);
    thread->start();
    Thread::sleep(100);
    Socket socket("localhost",20001);
    socket.connect();
    unsigned char buffer[11];
    socket.getInputStream()->read(buffer,11);
    CPPUNIT_ASSERT(strcmp((char *)buffer,"0123456789")==0);
    socket.getOutputStream()->write((char *)buffer,11);
    socket.close();
    thread->join();
//    server->close();
    delete server;
    delete thread;
}
