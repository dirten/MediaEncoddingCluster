#ifndef LISTENER_PROTO_SHOW_CONFIG
#define LISTENER_PROTO_SHOW_CONFIG
#include "../ProtocolCommand.h"
#include "org/esb/net/Socket.h"
#include "org/esb/config/config.h"

using namespace org::esb::net;
using namespace org::esb::config;

class ShowConfig:public ProtocolCommand{
    private:
	Socket * socket;
    public:
	~ShowConfig(){
	}

	ShowConfig(Socket * socket){
	    this->socket=socket;
	    this->is=socket->getInputStream();
	    this->os=socket->getOutputStream();
	}
	ShowConfig(InputStream * is, OutputStream * os){
	    this->is=is;
	    this->os=os;
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
	    string msg="property not found ";
		msg+=command+12;
		msg+="\n";
	    if(strlen(command+11)>0){
	    	if(Config::getProperties()->hasProperty(command+12)){
				msg=Config::getProperty(command+12);
				msg+="\n";
	    	}
	    }else{
        msg.clear();
		Properties * status=Config::getProperties();
		int size=status->toArray().size();
		for(int a=0;a<size;a++){
		    msg+=status->toArray()[a].first;
		    msg+="\t=\t";
		    msg+=status->toArray()[a].second;
		    msg+="\n";
		}
	    }
            os->write((char*)msg.c_str(),msg.length());
	}

	void printHelp(){
	    string msg="show config [key]\t[Shows the Configuration from the server]\n";
            os->write((char*)msg.c_str(),msg.length());	
	}

};
#endif

