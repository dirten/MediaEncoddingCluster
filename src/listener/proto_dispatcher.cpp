#include <iostream>
#include "org/esb/lang/Runnable.h"
#include "org/esb/net/Socket.h"
#include "proto_dispatcher.h"
using namespace std;
using namespace org::esb::net;
using namespace org::esb::lang;

	const char* trim(string & s, string & drop = *new string(" ")){
	    string r=s.erase(s.find_last_not_of(drop)+1);
	    return r.erase(0,r.find_first_not_of(drop)).c_str();
	}
	
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

		char* command=(char*)trim(*new string(recvData->data),*new string("\n"));
		
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

