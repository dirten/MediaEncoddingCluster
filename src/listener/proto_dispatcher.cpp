#include <iostream>
#include "org/esb/lang/Runnable.h"
#include "org/esb/net/Socket.h"
#include "proto_dispatcher.h"
#include "proto_help.cpp"
#include "proto_unknown.cpp"
#include "proto_startup.cpp"
#include <list>
using namespace std;
using namespace org::esb::net;
using namespace org::esb::lang;
using namespace org::esb::util;

//list<ProtoCommand*>  l;
ProtocolServer::~ProtocolServer() {
    cout << "Closing socket"<<endl;
//    socket->close();
}

ProtocolServer::ProtocolServer(Socket * socket) {
    this->socket=socket;
    //      l=new list<ProtoCommand*>();
    l.push_back((ProtoCommand*)new ProtoHelp(socket));
    l.push_back((ProtoCommand*)new ProtoStartup(socket));
    l.push_back((ProtoCommand*)new ProtoUnknown(socket));
}

void ProtocolServer::run() {
    while(!socket->isClosed()) {




	int dataLength=socket->getInputStream()->available(true);

//	char  test[]=new char[10];

        unsigned char *buffer=new unsigned char[dataLength];



	bzero(buffer, dataLength+1);
	cout << "Buffer alloc:"<<dataLength<<endl;
	cout << "Buffer before:"<<buffer<<endl;
	cout << "Buffer length before:"<<strlen((char*)buffer)<<endl;

        int bytes=socket->getInputStream()->read(buffer, dataLength);

	cout << "Bytes Read:"<< bytes << endl;
	cout << "Buffer:"<<buffer<<endl;

	if(bytes<=0)continue;
        char *command=strtok((char*)buffer,"\n\r");
	if(command==NULL)continue;
        cout << "Command : "<<command<<":"<<strlen(command)<<endl;

	
        list<ProtoCommand*>::iterator i;
        for(i=l.begin();i!=l.end();++i) {
            ProtoCommand * tmp=(ProtoCommand *)*i;
            if(tmp->isResponsible(command)) {
                cout << "Command responsible:"<<command<<endl;
                tmp->process(command);
		break;
            }
        }

        
	
        if(strcmp(command, "disconnect")==0) {
            cout << "disconnect Command"<<endl;
//            socket->getOutputStream()->write((unsigned char *)"disconnecting",13);
//	    delete [] buffer;
//	    buffer =0 ;
            socket->close();
//	    delete socket;
            break;
        }
	delete [] buffer;
	buffer =0 ;
        
//        delete [] command;
//        command=0;
    }
    cout << "Elvis has left the building"<<endl;
    delete this;
}
