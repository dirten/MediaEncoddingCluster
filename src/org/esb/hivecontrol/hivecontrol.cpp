#include "hivecontrol.h"

//using namespace org::esb::hive;
using namespace org::esb::config;
using namespace org::esb::net;
using namespace org::esb::lang;

namespace org{
namespace esb{
namespace hive{
HiveControl::HiveControl(){
    Config::init("cluster.cfg");
    _stop=false;
}

void HiveControl::start(){
    int port=atoi(Config::getProperty("hive.control.port"));
    ServerSocket *server_socket=new ServerSocket();
    server_socket->setPort(port);
    server_socket->bind();
    for(;_stop;){
	/*
	ClientData *data=(ClientData*)socket->accept();
//	ClientHandler * handler=new ClientHandler(data);
	Thread *listener=new Thread(new ClientHandler(data));
	listener->start();
	*/
    }
}
void HiveControl::stop(){
    _stop=true;
}

}}}

