#ifndef LISTENER_PROTO_HELP
#define LISTENER_PROTO_HELP
#include "proto_command.h"
#include "org/esb/net/Socket.h"

using namespace org::esb::net;

class ProtoHelp:public ProtoCommand{
    private:
	Socket * socket;
    public:
	~ProtoHelp(){
	}

	ProtoHelp(Socket * socket){
	    this->socket=socket;
	}

	int isResponsible(char * command){
	    cout << command <<":Command"<<endl;
	    if(strcmp(command,"help")==0)return CMD_HELP;
	    return CMD_NA;
	}

	void process(char * data){
	}
	void printHelp(){
	    string help="List of all Hive Commands\n";
	    help+="-------------------------\n";
    	    socket->getOutputStream()->write((char *)help.c_str(),help.length());	
	}
};
#endif

