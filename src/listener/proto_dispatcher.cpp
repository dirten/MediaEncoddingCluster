#include <iostream>
#include "org/esb/lang/Runnable.h"
#include "org/esb/net/Socket.h"
#include "proto_dispatcher.h"
#include "proto_help.cpp"
#include "proto_unknown.cpp"
#include <list>
using namespace std;
using namespace org::esb::net;
using namespace org::esb::lang;
using namespace org::esb::util;

//list<ProtoCommand*>  l;
ProtocolServer::~ProtocolServer() {
    cout << "Closing socket"<<endl;
    socket->close();
}

ProtocolServer::ProtocolServer(Socket * socket) {
    this->socket=socket;
    //      l=new list<ProtoCommand*>();
    l.push_back((ProtoCommand*)new ProtoHelp(socket));
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

        char command[dataLength];
	bzero(command, dataLength);
	memcpy(command, buffer, dataLength-1);


        cout << "Command : "<<command<<":"<<strlen(command)<<endl;
//	socket->getOutputStream()->write((char*)command,dataLength-1);

	
        list<ProtoCommand*>::iterator i;
        for(i=l.begin();i!=l.end();++i) {
            ProtoCommand * tmp=(ProtoCommand *)*i;
            if(tmp->isResponsible((char*)command)) {
                cout << "Command responsible"<<endl;
                tmp->process((char*)buffer);
		break;
            }
        }
	delete [] buffer;
	buffer =0 ;
        
	
        if(strcmp(command, "disconnect")==0) {
            cout << "disconnect Command"<<endl;
//            socket->getOutputStream()->write((unsigned char *)"disconnecting",13);
            socket->close();
            break;
        }
        
//        delete [] command;
//        command=0;
    }
    cout << "Elvis has left the building"<<endl;
}
