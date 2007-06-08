#include "proto_command.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/lang/Exception.h"
#include "hive.server/HiveControl.h"

class ProtoStartup : public ProtoCommand{
    private:
	Socket * socket;
    public:
	ProtoStartup(Socket * socket){
	    this->socket=socket;
	}
	
	~ProtoStartup(){}

	int isResponsible(char * command){
	    if(strcmp(command,"startup")==0){
		return CMD_PROCESS;
	    }else
	    if(strcmp(command,"help")==0){
		return CMD_HELP;
	    }
	    return CMD_NA;
	}

	void process(char * command){
	    string msg="Please wait while HiveControl startup\n";
            socket->getOutputStream()->write((char*)msg.c_str(),msg.length());
	    try{
		HiveControl::getInstance()->startup();
		msg="HiveControl is running\n";
	    }catch(Exception *ex){
		msg=ex->getStackTraceString();
	    }
    	    socket->getOutputStream()->write((char*)msg.c_str(),msg.length());
	}

	void printHelp(){
	    string msg="startup\t\t\t[Startup the server]\n";
            socket->getOutputStream()->write((char*)msg.c_str(),msg.length());	
	}

};
