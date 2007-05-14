#include "proto_command.h"



class ProtoDisconnect : public ProtoCommand{
    private:
	Socket * socket;
    public:
	ProtoDisconnect(Socket * socket){
	    this->socket=socket;
	}
	
	~ProtoDisconnect(){}
	bool isResponsible(char * command){
	    if(strcmp(command,"disconnect")==0||
	       strcmp(command,"quit")==0){
		return true;
	    }
	    return false;
	}
	void process(char * command){
	    string error="Disconnecting:";
            error+="\n";
            socket->getOutputStream()->write((char*)error.c_str(),error.length());
	    socket->close();
	}
};
