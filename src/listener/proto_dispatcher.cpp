#include <iostream>
#include "org/esb/lang/Runnable.h"
#include "org/esb/net/Socket.h"
#include "proto_dispatcher.h"
#include <list>
using namespace std;
using namespace org::esb::net;
using namespace org::esb::lang;
using namespace org::esb::util;

//list<ProtoCommand*>  l;
	ProtocolServer::~ProtocolServer(){
	    cout << "Closing socket"<<endl;
	    socket->close();
	}

	ProtocolServer::ProtocolServer(Socket * socket){
	    this->socket=socket;
//	    l=new list<ProtoCommand*>();
	    l.push_back((ProtoCommand*)new ProtoHelp(socket));
	}

	void ProtocolServer::run(){
	    while(!socket->isClosed()){

    		char * protoversion="MediaEncodingCluster ProtocolServer-1.0.0";
		SocketData *recvData=socket->read();

		char* command=(char*)StringUtil::trim(*new string(recvData->data),*new string("\n"));
		
		list<ProtoCommand*>::iterator i;
		for(i=l.begin();i!=l.end();++i){
	//	    cout << "test" << endl;
		ProtoCommand * tmp=(ProtoCommand *)*i;
		if(tmp->isResponsible(command)){
		    cout << "Command responsible"<<endl;
			tmp->process(command);
		    }

		}
		
		cout << "Command : "<<command<<endl;
		if(strcmp(command, "show version")==0){
		    cout << "version Command"<<endl;
		    socket->write((unsigned char *)protoversion,strlen(protoversion));
		}
		else if(strcmp(command, "disconnect")==0){
		    cout << "disconnect Command"<<endl;
		    socket->write((unsigned char *)"disconnecting",13);
		    socket->close();
		    break;
		}
		else{
		    string *error=new string("Unknown Command:");
		    error->append(command);
		    socket->write((unsigned char *)error->c_str(),error->length());
		    delete error;
		}
		bzero(command, strlen(command));
//		if(command)delete command;
		delete recvData;
	    }
	    cout << "Elvis has left the building"<<endl;
	}

