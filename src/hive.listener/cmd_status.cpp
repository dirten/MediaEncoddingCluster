#include "proto_command.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/lang/Exception.h"
#include "hive.server/HiveControl.h"

class ProtoStatus : public ProtoCommand{
    private:
	Socket * socket;
    public:
	ProtoStatus(Socket * socket){
	    this->socket=socket;
	}
	
	~ProtoStatus(){}


	int isResponsible(char * command){
	    if(strcmp(command,"show status")==0){
		return CMD_PROCESS;
	    }else
	    if(strcmp(command,"help")==0){
		return CMD_HELP;
	    }
	    return CMD_NA;
	}

	void process(char * command){
	    Properties * status=HiveControl::getInstance()->getStatus();
	    int size=status->toArray().size();
	    string msg="";
	    for(int a=0;a<size;a++){
		msg+=status->toArray()[a].first;
		msg+="\t=\t";
		msg+=status->toArray()[a].second;
		msg+="\n";
	    }
            socket->getOutputStream()->write((char*)msg.c_str(),msg.length());	    
	}

	void printHelp(){
	    string msg="show status\t\t[Status from the server]\n";
            socket->getOutputStream()->write((char*)msg.c_str(),msg.length());	
	}
};
