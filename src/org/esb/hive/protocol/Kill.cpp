#ifndef LISTENER_PROTO_KILL
#define LISTENER_PROTO_KILL
#include "../ProtocolCommand.h"
#include "org/esb/net/Socket.h"
#include <signal.h>
using namespace org::esb::net;
using namespace org::esb::hive;

class Kill:public ProtocolCommand{
    private:
	Socket * socket;
    public:
	~Kill(){
	}

	Kill(Socket * socket){
	    this->socket=socket;
	    this->is=socket->getInputStream();
	    this->os=socket->getOutputStream();
	}
	
	Kill(InputStream * is, OutputStream * os){
	    this->socket=0;
	    this->is=is;
	    this->os=os;
	}
	int isResponsible (cmdId & cmid) {
	
	}

	int isResponsible(char * command){
	    if(strcmp(command,"kill")==0){
		    return CMD_PROCESS;
	    }else
	    if(strcmp(command,"help")==0){
		    return CMD_HELP;
	    }
	    return CMD_NA;
	}

	void process(char * data){
	    socket->close();
//	    main_nextLoop=false;
//    	    raise(SIGINT);
//        exit(0);
	}

	void printHelp(){
	    string help="kill\t\t\t[Kill the Listener]\n";
//	    help+="-------------------------\n";
        socket->getOutputStream()->write((char *)help.c_str(),help.length());
	}
};
#endif

