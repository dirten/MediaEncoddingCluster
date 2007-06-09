#include "ProtocolServer.h"
#include <iostream>
#include "org/esb/lang/Runnable.h"
#include "org/esb/net/Socket.h"
#include "ProtocolCommand.h"
#include "ProtocolServer.h"
#include <list>

/*all Protocols here*/
#include "protocol/Help.cpp"
#include "protocol/Disconnect.cpp"
#include "protocol/ShowConfig.cpp"
#include "protocol/ShutdownHive.cpp"
#include "protocol/StartupHive.cpp"
#include "protocol/Status.cpp"
#include "protocol/Unknown.cpp"




using namespace std;
using namespace org::esb::net;
using namespace org::esb::lang;
using namespace org::esb::util;
using namespace org::esb::hive;

ProtocolServer::~ProtocolServer() {
    cout << "Closing ProtocollServer"<<endl;
    list<ProtocolCommand*>::iterator i;
    for(i=l.begin();i!=l.end();++i) {
	    ProtocolCommand *tmp=(ProtocolCommand*)*i;
	    delete tmp;
    }
    l.clear();
    delete socket;
    socket=0;
}

ProtocolServer::ProtocolServer(Socket * socket) {
    this->socket=socket;
    l.push_back(new Help(socket));
    l.push_back(new Disconnect(socket));
    l.push_back(new ShowConfig(socket));
    l.push_back(new ShutdownHive(socket));
    l.push_back(new StartupHive(socket));
    l.push_back(new Status(socket));
    l.push_back(new Unknown(socket));

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
        list<ProtocolCommand*>::iterator i;
        for(i=l.begin();i!=l.end();++i) {
            ProtocolCommand *tmp=(ProtocolCommand*)*i;
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
