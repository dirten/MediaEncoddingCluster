#include "../ProtocolCommand.h"
#include "org/esb/lang/Thread.h"
#include "hive.server/HiveControl.h"

class ShutdownHive : public ProtocolCommand{
    private:
	Socket * socket;
    public:
	ShutdownHive(Socket * socket){
	    this->socket=socket;
	}
	

	~ShutdownHive(){}

	int isResponsible(char * command){
	    if(strcmp(command,"shutdown")==0){
		return CMD_PROCESS;
	    }else
	    if(strcmp(command,"help")==0){
		return CMD_HELP;
	    }
	    return CMD_NA;
	}

	void process(char * command){
	    string msg="Please wait while stopping HiveControl\n";
            socket->getOutputStream()->write((char*)msg.c_str(),msg.length());
	    try{
		HiveControl::getInstance()->shutdown();
		msg="HiveControl shutdown\n";
	    }catch(Exception *ex){
		msg=ex->getStackTraceString();
	    }
    	    socket->getOutputStream()->write((char*)msg.c_str(),msg.length());
	}

	void printHelp(){
	    string msg="shutdown\t\t[Stopps the server]\n";
            socket->getOutputStream()->write((char*)msg.c_str(),msg.length());	
	}

};
