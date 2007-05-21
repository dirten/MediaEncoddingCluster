#ifndef LISTENER_PROTO_SHOW_CONFIG
#define LISTENER_PROTO_SHOW_CONFIG
#include "proto_command.h"
#include "org/esb/net/Socket.h"
#include "org/esb/config/config.h"

using namespace org::esb::net;
using namespace org::esb::config;

class ProtoShowConfig:public ProtoCommand{
    private:
	Socket * socket;
    public:
	~ProtoShowConfig(){
	}

	ProtoShowConfig(Socket * socket){
	    this->socket=socket;
	}


	int isResponsible(char * command){
	    if(strstr(command,"show config")>0){
		return CMD_PROCESS;
	    }else
	    if(strcmp(command,"help")==0){
		return CMD_HELP;
	    }
	    return CMD_NA;
	}

	void process(char * command){
	    string msg="";
	    if(strlen(command+11)>0){
		msg=Config::getProperty(command+12);
		msg+="\n";
	    }else{
		Properties * status=Config::getProperties();
		int size=status->toArray().size();
		for(int a=0;a<size;a++){
		    msg+=status->toArray()[a].first;
		    msg+="\t=\t";
		    msg+=status->toArray()[a].second;
		    msg+="\n";
		}
	    }
            socket->getOutputStream()->write((char*)msg.c_str(),msg.length());
	}

	void printHelp(){
	    string msg="show config [key]\t[Shows the Configuration from the server]\n";
            socket->getOutputStream()->write((char*)msg.c_str(),msg.length());	
	}

};
#endif

