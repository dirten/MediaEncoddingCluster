#include <iostream>
#include "org/esb/lang/Runnable.h"
#include "org/esb/net/Socket.h"
#include "proto_command.h"
#include "proto_dispatcher.h"
#include "cmd_help.cpp"
#include "cmd_unknown.cpp"
#include "cmd_status.cpp"
#include "cmd_show_config.cpp"
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
    delete socket;
    socket=0;
}

ProtocolServer::ProtocolServer(Socket * socket) {
    this->socket=socket;
    
    l.push_back(new ProtoHelp(socket));
    l.push_back(new ProtoDisconnect(socket));
    l.push_back(new ProtoStatus(socket));
    l.push_back(new ProtoShowConfig(socket));
    l.push_back(new ProtoStartup(socket));
    l.push_back(new ProtoShutdown(socket));
    l.push_back(new ProtoUnknown(socket));
    
    string help="\n\nWelcome to the MediaEncodingCluster ProtocolServer-0.0.1\n";
    help+="Type 'help' for Help\n";
    help+="--------------------------------------------------------\n";
    socket->getOutputStream()->write((char *)help.c_str(),help.length());
}

void ProtocolServer::run() {
    while(!socket->isClosed()) {
	int dataLength=socket->getInputStream()->available(true);
	if(dataLength==0){
	    cout << "0 Byte empfangen, das ist nicht gut!!!"<< endl;
	    break;
	}
        unsigned char buffer[dataLength];
		bzero(buffer, dataLength);
        int bytes=socket->getInputStream()->read(buffer, dataLength);
        char *command=strtok((char*)buffer,"\n\r");
	if(command==NULL||strlen(command)<=0)continue;
        list<ProtoCommand*>::iterator i;
        for(i=l.begin();i!=l.end();++i) {
            ProtoCommand *tmp=(ProtoCommand*)*i;
            if(tmp->isResponsible(command)==CMD_PROCESS) {
                tmp->process(command);
		break;
            }
            else
            if(tmp->isResponsible(command)==CMD_HELP) {
                tmp->printHelp();
            }
        }
    }
}
