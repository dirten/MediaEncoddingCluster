#include "proto_command.h"



class ProtoStartup : public ProtoCommand{
    private:
	Socket * socket;
    public:
	ProtoStartup(Socket * socket){
	    this->socket=socket;
	}
	
	~ProtoStartup(){}
	bool isResponsible(char * command){
	    if(strcmp(command,"startup")==0){
		return true;
	    }
	    return false;
	}
	void process(char * command){
	    string error="StartupCommand:";
            error+=command;
            error+="\n";
            socket->getOutputStream()->write((char*)error.c_str(),error.length());
//            delete error;
	}
};
