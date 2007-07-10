#include "ProtocolServer.h"
#include <iostream>
#include "org/esb/lang/Runnable.h"
#include "org/esb/net/Socket.h"
#include "ProtocolCommand.h"
#include "ProtocolServer.h"
#include "Command.h"
#include "CommandInputStream.h"
#include <list>

/*all Protocols here*/
#include "protocol/Help.cpp"
#include "protocol/Kill.cpp"
#include "protocol/DataHandler.cpp"
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
//pthread_mutex_t ProtocolServer::mutex;

ProtocolServer::~ProtocolServer() {
    list<ProtocolCommand*>::iterator i;
    for(i=l.begin();i!=l.end();++i) {
	    ProtocolCommand *tmp=(ProtocolCommand*)*i;
	    delete tmp;
    }
    l.clear();
    delete socket;
    socket=0;
    delete _cis;
}

ProtocolServer::ProtocolServer(Socket * socket,pthread_mutex_t m) {
	mutex=m;
    this->socket=socket;
    _cis=new CommandInputStream(socket->getInputStream());
    l.push_back(new Help(socket->getInputStream(), socket->getOutputStream()));
    l.push_back(new DataHandler(socket->getInputStream(), socket->getOutputStream()));
    l.push_back(new Disconnect(socket));
    l.push_back(new Kill(socket));
    l.push_back(new ShowConfig(socket));
    l.push_back(new ShutdownHive(socket));
    l.push_back(new StartupHive(socket));
    l.push_back(new Status(socket));
    l.push_back(new Unknown(socket));

    string help="\n\nWelcome to the MediaEncodingCluster ProtocolServer-0.0.1\n";
    help+="Type 'help' for Help\n";
    help+="--------------------------------------------------------\n";
//    socket->getOutputStream()->write((char *)help.c_str(),help.length());
}

void ProtocolServer::run() {


    while(!socket->isClosed()) {
//        Command * cmd=_cis->readCommand();

	int dataLength=socket->getInputStream()->available(true);
	if(dataLength==0){
	    cout << "0 Byte empfangen, das ist nicht gut!!!"<< endl;
	    break;
	}
        unsigned char buffer[dataLength+1];
	memset(buffer,0, dataLength+1);
        socket->getInputStream()->read(buffer, dataLength);
        char *command=strtok((char*)buffer,"\n\r");
//	cout << "Command:"<<command<<endl;
	if(command==NULL||strlen(command)<=0)continue;
        list<ProtocolCommand*>::iterator i;
        for(i=l.begin();i!=l.end();++i) {
            ProtocolCommand *tmp=(ProtocolCommand*)*i;
            if(tmp->isResponsible(command)==CMD_PROCESS) {
				pthread_mutex_lock(&mutex);
                tmp->process(command);
				pthread_mutex_unlock(&mutex);
				break;
            }
            else
            if(tmp->isResponsible(command)==CMD_HELP) {
                tmp->printHelp();
            }
        }
	if(!socket->isClosed()){
    	    string line="--------------------------------------------------------\n";
//    	    socket->getOutputStream()->write((char *)line.c_str(),line.length());
	}
        
    }
    cout <<"Elvis has left the Building"<<endl;
}
