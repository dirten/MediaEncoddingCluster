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

	bool isResponsible(char * command){
	    return (strcmp(command,"help")==0);
	}

	void process(char * data){
	    string help="Using help from CommandLine\n";
	    help+="MediaEncodingCluster ProtocolServer-1.0.0";
    	    socket->getOutputStream()->write((char *)help.c_str(),help.length());
	}
};
#endif

