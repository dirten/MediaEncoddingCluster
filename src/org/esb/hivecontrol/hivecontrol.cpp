#include "hivecontrol.h"

//using namespace org::esb::hive;
using namespace org::esb::config;
using namespace org::esb::socket;
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
    Socket *socket=new Socket();
    socket->setPort(port);
    socket->Listen();
    for(;_stop;){
	ClientData *data=(ClientData*)socket->Accept();
//	ClientHandler * handler=new ClientHandler(data);
	Thread *listener=new Thread(new ClientHandler(data));
	listener->start();
    }
}
void HiveControl::stop(){
    _stop=true;
}

}}}

