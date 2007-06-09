#ifndef LISTENER_PROTO_HELP
#define LISTENER_PROTO_HELP
#include "../ProtocolCommand.h"
#include "org/esb/net/Socket.h"

using namespace org::esb::net;
using namespace org::esb::hive;

class Help:public ProtocolCommand{
    private:
	Socket * socket;
    public:
	~Help(){
	}

	Help(Socket * socket){
	    this->socket=socket;
	}

	int isResponsible(char * command){
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

