#include <cppunit/extensions/HelperMacros.h>
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
//class EchoServer;
class TestOrgEsbNet: public CppUnit::TestFixture
{

    CPPUNIT_TEST_SUITE(TestOrgEsbNet);
    CPPUNIT_TEST(testConnect);

    CPPUNIT_TEST_SUITE_END();
    
    public:
	void setUp();
	void tearDown();
	void testConnect();
	void testConnect2();
	Thread * thread;
};


CPPUNIT_TEST_SUITE_REGISTRATION(TestOrgEsbNet);

class EchoServer:public Runnable{
    public:
        void run(){
            ServerSocket server(20000);
            server.bind();
            if(Socket * client=server.accept()){
		string buffer;
                client->getInputStream()->read(buffer);
                client->getOutputStream()->write(buffer);
		Thread::sleep(1000);
		client->close();
                delete client;
            }
            server.close();
        }
};

void TestOrgEsbNet::setUp(){
    thread=new Thread(new EchoServer());
    thread->setAutoDelete(true);
    thread->start();
    Thread::sleep(1000);

}


void TestOrgEsbNet::tearDown(){
    thread->join();
    delete thread;
}

void TestOrgEsbNet::testConnect(){
    Socket socket("localhost",20000);
    socket.connect();
    string buffer="0123456789";
    string in;
    socket.getOutputStream()->write(buffer);
    socket.getInputStream()->read(in);
    socket.close();
    CPPUNIT_ASSERT(strcmp((char *)in.c_str(),"0123456789")==0);

}

