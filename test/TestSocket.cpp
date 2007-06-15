#include <cppunit/extensions/HelperMacros.h>
//#include "org/esb/config/config.h"
#include "org/esb/net/ServerSocket.h"
#include "org/esb/net/Socket.h"
#include "org/esb/io/InputStream.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/lang/Runnable.h"
//#define new new

using namespace std;
//using namespace org::esb::config;
using namespace org::esb::io;
using namespace org::esb::net;
using namespace org::esb::lang;

class TestSocket: public CppUnit::TestFixture
{

    CPPUNIT_TEST_SUITE(TestSocket);
    CPPUNIT_TEST(testConstructor);
    CPPUNIT_TEST(testSimpleConnect);
    CPPUNIT_TEST(testStream);
    CPPUNIT_TEST(testUnknownStreamLength);

    CPPUNIT_TEST_SUITE_END();
    
    public:
	void setUp();
	void tearDown();
	void testConstructor();
	void testSimpleConnect();
    void testStream();
    void testUnknownStreamLength();
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
                Thread::sleep(500);
                delete client;
            }
            server.close();
        }
};

void TestSocket::testSimpleConnect(){
//    char * port=Config::getProperty("port");
    SimpleConnectThread * server=new SimpleConnectThread();
    Thread * thread=new Thread(server);
    thread->start();
    Thread::sleep(500);
    Socket socket("localhost",20000);
    socket.connect();
    socket.close();
    thread->join();
    delete server;
    delete thread;
}

class StreamThread:public Runnable{
    public:
        void run(){
            ServerSocket server(20000);
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
    Socket socket("localhost",20000);
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

class UnknownStreamThread:public Runnable{
    public:
        void run(){
            ServerSocket server(20000);
            server.bind();
            if(Socket * client=server.accept()){
//                cout << "client here"<<endl;
                InputStream *is=client->getInputStream();
                char b;
                string tmp;
                Thread::sleep(100);
                while(is->available()>0){
                    is->read((unsigned char *)&b,1);
//                    cout << b << endl;
                    tmp+=b;
                }
//                cout << "alles da:"<<tmp.c_str()<<endl;
                CPPUNIT_ASSERT(strcmp((const char *)"die ist ein Stream test\nhier kommt noch was",tmp.c_str())==0);
                delete client;
            }
            server.close();
        }
};

void TestSocket::testUnknownStreamLength(){
    UnknownStreamThread* server=new UnknownStreamThread();
    Thread * thread=new Thread(server);
    thread->start();


    Thread::sleep(100);
    Socket socket("localhost",20000);
    socket.connect();
    string buffer="die ist ein Stream test\nhier kommt noch was";
    socket.getOutputStream()->write((char *)buffer.c_str(),buffer.length());
//	Thread::sleep(500);
    socket.close();
    thread->join();
//    server->close();
    delete server;
    delete thread;
}
