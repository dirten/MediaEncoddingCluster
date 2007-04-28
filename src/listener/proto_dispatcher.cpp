#include <iostream>
#include "org/esb/lang/Runnable.h"
#include "org/esb/net/Socket.h"
#include "proto_dispatcher.h"
using namespace std;
using namespace org::esb::net;
using namespace org::esb::lang;
using namespace org::esb::util;

	
	ProtocolServer::~ProtocolServer(){
	    cout << "Closing socket"<<endl;
	    socket->close();
	}
	ProtocolServer::ProtocolServer(Socket * socket){
	    this->socket=socket;
	
	}
	void ProtocolServer::run(){
	    while(!socket->isClosed()){
    		char * protoversion="MediaEncodingCluster ProtocolServer-1.0.0";
		cout << "waiting for command"<<endl;
		SocketData *recvData=socket->read();
		cout << "command received "<<endl;

		char* command=(char*)StringUtil::trim(*new string(recvData->data),*new string("\n"));
		
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
		}else{
		    string error="Unknown Command:";
		    error+=command;
		    socket->write((unsigned char *)error.c_str(),strlen(error.c_str()));
		}
	    }
	    cout << "Elvis has left the building"<<endl;
	}

