#include "proto_command.h"



class ProtoDisconnect : public ProtoCommand{
    private:
	Socket * socket;
    public:
	ProtoDisconnect(Socket * socket){
	    this->socket=socket;
	}
	
	~ProtoDisconnect(){}
	int isResponsible(char * command){
	    if(strcmp(command,"disconnect")==0||
	       strcmp(command,"quit")==0){
		return CMD_PROCESS;
	    }else
	    if(strcmp(command,"help")==0){
		return CMD_HELP;
	    }
	    return CMD_NA;
	}
	void process(char * command){
	    string error="Disconnecting:";
            error+="\n";
            socket->getOutputStream()->write((char*)error.c_str(),error.length());
	    socket->close();
	}

	void printHelp(){
	    string msg="disconnect | quit\t[Disconnecting from the server]\n";
            socket->getOutputStream()->write((char*)msg.c_str(),msg.length());	
	}
};
