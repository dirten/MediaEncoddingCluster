#include <iostream>
#include "org/esb/lang/Runnable.h"
#include "org/esb/net/Socket.h"
#include "proto_dispatcher.h"
#include "cmd_help.cpp"
#include "cmd_unknown.cpp"
#include "cmd_startup.cpp"
#include "cmd_shutdown.cpp"
#include "cmd_disconnect.cpp"
#include <list>
using namespace std;
using namespace org::esb::net;
using namespace org::esb::lang;
using namespace org::esb::util;

ProtocolServer::~ProtocolServer() {
    cout << "Closing ProtocollServer"<<endl;
    list<ProtoCommand*>::iterator i;
    for(i=l.begin();i!=l.end();++i) {
	ProtoCommand *tmp=(ProtoCommand*)*i;
	delete tmp;
    }
    l.clear();
}

ProtocolServer::ProtocolServer(Socket * socket) {
    this->socket=socket;
    l.push_back(new ProtoDisconnect(socket));
    l.push_back(new ProtoHelp(socket));
    l.push_back(new ProtoStartup(socket));
    l.push_back(new ProtoShutdown(socket));
    l.push_back(new ProtoUnknown(socket));
}

void ProtocolServer::run() {
    while(!socket->isClosed()) {
	int dataLength=socket->getInputStream()->available(true);
	if(dataLength==0){
	    cout << "0 Byte empfangen"<< endl;
	    continue;
	}
        unsigned char buffer[dataLength];
	bzero(buffer, dataLength);
        int bytes=socket->getInputStream()->read(buffer, dataLength);
        char *command=strtok((char*)buffer,"\n\r");
	if(command==NULL||strlen(command)<=0)continue;
//        cout << "Command : "<<command<<":"<<strlen(command)<<endl;
        list<ProtoCommand*>::iterator i;
        for(i=l.begin();i!=l.end();++i) {
            ProtoCommand *tmp=(ProtoCommand*)*i;
            if(tmp->isResponsible(command)) {
                tmp->process(command);
		break;
            }
        }
    }
    cout << "Elvis has left the building"<<endl;
    delete socket;
    socket=0;
    /*uuuuuaaaaaa schöne scheisse*/
    delete this;
}
