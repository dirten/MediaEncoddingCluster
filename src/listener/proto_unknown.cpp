#include <iostream>
#include "proto_command.h"
#include "org/esb/net/Socket.h"

using namespace std;
using namespace org::esb::net;


class ProtoUnknown : public ProtoCommand{
    private:
	Socket * socket;
    public:
	ProtoUnknown(Socket * socket){
	    this->socket=socket;
	}
	
	~ProtoUnknown(){}
	bool isResponsible(char * command){
	    if(strlen(command)>0&&strcmp(command,"disconnect")!=0){
		return true;
	    }
	    return false;
	}
	void process(char * command){
	    string error="Unknown Command:";
            error+=command;
            error+="\n";
            socket->getOutputStream()->write((char*)error.c_str(),error.length());
//            delete error;
	}
};
